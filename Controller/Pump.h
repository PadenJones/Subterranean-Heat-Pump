#ifndef PUMP_H
#define PUMP_H

const byte PUMP_A = 2;
const byte PUMP_B = 3;

class Pump
{
  public:
    Pump();
    int getCurrentZone();
    void runZone(int zone);
    void runZone1();
    void runZone2();
    void runAllZones();
    void kill();

  private:
    int currentZone;
};

Pump::Pump()
{
  currentZone = 0;
  pinMode(PUMP_A, OUTPUT);
  pinMode(PUMP_B, OUTPUT);
  digitalWrite(PUMP_A, HIGH);
  digitalWrite(PUMP_B, HIGH);
}

int Pump::getCurrentZone()
{
  return currentZone;
}

void Pump::runZone(int zone)
{
  if (currentZone != zone)
  {
    switch(zone)
    {
      case 0:
        kill();
        break;
      case 1:
        runZone1();
        break;
      case 2:
        runZone2();
        break;
      case 3:
        runAllZones();
        break;
      default:
        break;
    }
  }
}

void Pump::runZone1()
{
  currentZone = 1;
  digitalWrite(PUMP_A, LOW);
  digitalWrite(PUMP_B, HIGH);
}

void Pump::runZone2()
{
  currentZone = 2;
  digitalWrite(PUMP_A, HIGH);
  digitalWrite(PUMP_B, LOW);
}

void Pump::runAllZones()
{
  currentZone = 3;
  digitalWrite(PUMP_A, LOW);
  digitalWrite(PUMP_B, LOW);
}

void Pump::kill()
{
  currentZone = 0;
  digitalWrite(PUMP_A, HIGH);
  digitalWrite(PUMP_B, HIGH);
}

#endif
