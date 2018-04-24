#include <avr/interrupt.h>
#include <SPI.h>

#include "Probes.h"
#include "Pump.h"
#include "Web.h"

int testMsgCountA = 1;
int testMsgCountB = 1;

Probes probes;
Pump pump;
Web web;

void setup(void)
{
  Serial.begin(57600);
  printf_begin();
  probes.start();
  pump.runAllZones();
}

int failCount = 0;
void loop(void)
{
  int instruction = 0;
  if(web.getInstructions(instruction))
  {
    pump.runZone(instruction);
  }

  double probeA = probes.getTemp(nodeB);
  double probeB = probes.getTemp(nodeC);

  if (probeA && probeB)
  {
    web.saveStateToWeb(probeA, probeB, pump.getCurrentZone());
  }
}

