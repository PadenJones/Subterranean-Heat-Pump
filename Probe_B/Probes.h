#ifndef PROBES_H
#define PROBES_H

#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

const uint64_t nodeA = 0x000000AALL;
const uint64_t nodeB = 0x000000BBLL;
const uint64_t nodeC = 0x000000CCLL;
const uint64_t myself = nodeC;

class Probes
{
  private:
    RF24* radio;

  public:
    String lastMessage;
  
    Probes();
    void start();
    double getTemp(uint64_t target);
    bool sendTemp(double temp);
    bool sendMessage(uint64_t target, unsigned long msg);
    bool sendMessage(uint64_t target, unsigned long msg, int retries);
    bool waitForMessage(unsigned long* message, long waitPeriod);
    ~Probes();
};

Probes::Probes()
{
  int CE = 9;
  int CSN = (myself == nodeA) ? 53 : 10;
  this->radio = new RF24(CE, CSN);
}

void Probes::start()
{
  radio->begin();
  radio->setDataRate(RF24_250KBPS);
  radio->setPALevel(RF24_PA_MIN); // MIN, LOW, HIGH, MAX
  radio->setPayloadSize(8);
  radio->setRetries(15, 15);

  // This is how pipes should be set.
  radio->openWritingPipe(nodeA);
  radio->openReadingPipe(0, myself);
  for (int i = 1; i < 6; i++)
  {
    radio->openReadingPipe(i, 0);
  }

  radio->startListening();
}

bool Probes::sendMessage(uint64_t target, unsigned long message, int retries)
{
  radio->stopListening();

  delay(15); // For stability purposes...
  
  radio->openWritingPipe(target);

  bool status = false;
  for (int i = 0; i < retries; i++)
  {
    status = radio->write(&message, sizeof(unsigned long));
    if (status)
    {
      break;
    }
    delay(250);
  }

  radio->startListening();
  return status;
}

bool Probes::waitForMessage(unsigned long* message, long waitPeriod)
{
  bool timeout = false;
  long start = millis();
  while (!timeout)
  {
    if (radio->available())
    {
      bool done = false;
      while (!done)
      {
        done = radio->read(message, sizeof(unsigned long));
      }
      
      return true;
    }

    if (millis() - start > waitPeriod)
    {
      timeout = true;
    }
  }

  return false;
}

double Probes::getTemp(uint64_t target)
{
  bool initialRequest = this->sendMessage(target, 1, 4);
  if (initialRequest)
  {
    unsigned long longTemp = 0;
    bool status = this->waitForMessage(&longTemp, 2500);

    if (status)
    {
      return longTemp / 100.00;
    }
    else
    {
      lastMessage = "Probes::getTemp() Error: response from probe timed out";
      return longTemp;
    }
  }
  else
  {
    lastMessage = "Probes::getTemp() Error: initial request did not send (controller could not reach probe)";
    return initialRequest;
  }
}

bool Probes::sendTemp(double temp)
{
  unsigned long longTemp = temp * 100;
  lastMessage = "Probes::sendTemp() - sending temp";
  lastMessage += temp;

  bool status = this->sendMessage(nodeA, longTemp, 4);
  if (status)
  {
    lastMessage += " - Success!";
  }
  else
  {
    lastMessage += " - Failed...";
  }

  return status;
}

Probes::~Probes()
{
  delete radio;
}

#endif
