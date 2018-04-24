#include <avr/interrupt.h>
#include <SPI.h>

#include "Probes.h"
#include "TemperatureProbe.h"

const byte sendingLED = 3;
const byte readingLED = 4;
const byte waitingLED = 5;

Probes probes;
TemperatureProbe tempProbe;

void setup(void)
{
  Serial.begin(57600);
  printf_begin();
  probes.start();

  for (int i = 0; i < 6; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW); 
  }
}

bool status = 0;
double temp = 0;
unsigned long message = 0;
void loop(void)
{
  digitalWrite(waitingLED, HIGH);
  probes.waitForMessage(&message, 3600000);
  digitalWrite(waitingLED, LOW);

  switch (message)
  {
    case 1:
      Serial.println("Controller requested temperature...");
      
      digitalWrite(readingLED, HIGH);
      temp = tempProbe.getTemp();
      digitalWrite(readingLED, LOW);

      digitalWrite(sendingLED, HIGH);
      Serial.print("Sending temperature: ");
      Serial.print(temp);
      Serial.print(": ");
      status =  probes.sendTemp(tempProbe.getTemp());
      Serial.println(status);

      if (!status)
      {
        Serial.println(probes.lastMessage);
      }
      
      digitalWrite(sendingLED, LOW);
      break;
    default:
      Serial.print("Error - callcode switch defaulted (main loop) ");
      Serial.println(message);
  }
}
