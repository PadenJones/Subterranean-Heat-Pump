#ifndef TEMPERATURE_PROBE_H
#define TEMPERATURE_PROBE_H

#include <DallasTemperature.h>
#include <OneWire.h>

const uint8_t oneWireBus = 2;

class TemperatureProbe
{
  private:
    OneWire* oneWire;
    DallasTemperature* sensor;
    
  public:
    TemperatureProbe();
    double getTemp();
    ~TemperatureProbe();
};

TemperatureProbe::TemperatureProbe()
{
  oneWire = new OneWire(oneWireBus);
  sensor = new DallasTemperature(oneWire);
  sensor->begin();
}

double TemperatureProbe::getTemp()
{
  double temperature = 0;
  sensor->requestTemperatures();
  temperature = sensor->getTempFByIndex(0);
  return temperature;
}

TemperatureProbe::~TemperatureProbe()
{
  delete oneWire;
  delete sensor;
}

#endif
