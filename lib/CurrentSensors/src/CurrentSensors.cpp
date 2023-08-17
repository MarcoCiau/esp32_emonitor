#include "CurrentSensors.h"
#include <ADC_74HC4066.h>
#define DEBUG_VERBOSE
#define ICALIBRATION_30A 90.000
#define ICALIBRATION_100A 177.00
#define UMBRAL 0.400

#define ENABLE_FILTER
#ifdef ENABLE_FILTER
#define FILTER_SIZE 0.111
#endif

ADC_74HC4066 adc(S0_74HC4067,
                 S1_74HC4067,
                 S2_74HC4067,
                 S3_74HC4067,
                 ANALOG_74HC4067);



static uint16_t readADC(uint8_t channel)
{
  return adc.read(channel);
}

void CurrentSensors::begin()
{
  for (uint8_t i = 0; i < NUM_MAX_CURRENT_SENSOR; i++)
  {
    sensor[i].current(i, DEFAULT_100A_CT_CAL);
    sensor[i].setVREF(3300U);
    sensor[i].attachADCReadCallback(readADC);
  }
  lastSensorRead = 0;
}

void CurrentSensors::loop()
{
  if (lastSensorRead > NUM_MAX_CURRENT_SENSOR - 1)
    lastSensorRead = 0;
  readNow(lastSensorRead);
  lastSensorRead++;
  debugSensors();
}

void CurrentSensors::readNow(uint8_t nSensor)
{
  readings[nSensor] = sensor[nSensor].calcIrms(500);
}

double CurrentSensors::getCurr(uint8_t nSensor)
{
  return readings[nSensor];
}

void CurrentSensors::debugSensors()
{
  static unsigned long debugTimer = 0;
  if (millis() - debugTimer < 3000)
    return;
  debugTimer = millis();
  for (uint8_t i = 0; i < NUM_MAX_CURRENT_SENSOR; i++)
  {
    printf("CURR%d: %.2f\n", i + 1, readings[i]);
  }
}
CurrentSensors currentSensors;
