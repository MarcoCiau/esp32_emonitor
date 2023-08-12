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

EnergyMonitor line1(0, 0);
EnergyMonitor line2(0, 1);
EnergyMonitor line3(0, 2);

double getFilteredVrms(EnergyMonitor * sensor, uint8_t sensorId);
double getFilteredIrms(EnergyMonitor * sensor, uint8_t sensorId);

double rawData[4];
unsigned long debugTimer;

static uint16_t getFnc(uint16_t id)
{
  uint16_t val;
  double curr = currentSensors.getCurr(id);
  val = (uint16_t)(curr*100);
  return val;
}

static uint16_t readADC(uint8_t channel)
{
  return adc.read(channel);
}

void CurrentSensors::begin()
{
  /* Current Sensor 1 */
  line1.currentSensorBegin(ICALIBRATION_100A);
  line1.attachADCReadCallback(readADC);
  /* Current Sensor 2*/
  line2.currentSensorBegin(ICALIBRATION_100A);
  line2.attachADCReadCallback(readADC);
  /* Current Sensor Only */
  line3.currentSensorBegin(ICALIBRATION_100A);
  line3.attachADCReadCallback(readADC);
  
  debugTimer = millis();
  lastSensorRead = 0;
}

void CurrentSensors::loop()
{
    if (lastSensorRead > 2) lastSensorRead = 0;
    readNow(lastSensorRead);
    lastSensorRead++;
    debugSensors();
}

void CurrentSensors::readNow(uint8_t nSensor)
{
  switch (nSensor)
  {
  case 0:
    handleISensor(&line1, nSensor);
    break;
  case 1:
    handleISensor(&line2, nSensor);
    break;
  case 2:
    handleISensor(&line3, nSensor);
    break;
  }
}

double CurrentSensors::getCurr(uint8_t nSensor)
{
  return readings[nSensor];
}

void CurrentSensors::debugSensors()
{
  if (millis() - debugTimer < 3000) return;
  debugTimer = millis();
  #ifdef DEBUG_VERBOSE
  printf("Voltage: %d ",   (uint16_t)(readings[0] * 100.00));
  printf("CURR1: %d ",   (int16_t)(readings[1] * 100.00));
  printf("CURR2: %d ",   (int16_t)(readings[2] * 100.00));
  printf("CURR3: %d ", (int16_t)(readings[3] * 100.00));
  printf("PF1: %d ",   (int16_t)(readings[4] * 100.00));
  printf("PF2: %d\n",   (int16_t)(readings[5] * 100.00));
  #endif
}

void CurrentSensors::handleVISensor(EnergyMonitor * sensor, uint8_t sensorID)
{
  sensor->calcVI();
  if (sensorID == 0)readings[sensorID] = getFilteredVrms(sensor, sensorID);
  double sensorVal = getFilteredIrms(sensor, sensorID + 1);
  double powerFactor = sensor->getPowerFactor();
  // readings[4 + sensorID] = sensor->getPowerFactor();

  if (sensorVal > UMBRAL)
  {
    if (readings[4 + sensorID] < 0) sensorVal *= -1;
  }
  else sensorVal = 0;

  readings[sensorID + 1] = sensorVal;

  /* Handle Power Factor */
  if (sensorVal != 0 && fabs(powerFactor) > 0.50)
  {
    readings[4 + sensorID] = powerFactor;
  }
  else 
  {
    readings[4 + sensorID] = 0.95;/* deafualt power factor value when L1 and L2 current is 0 */
  }
}

void CurrentSensors::handleISensor(EnergyMonitor * sensor, uint8_t sensorID)
{
  sensor->calcIRMS();
  
  // double sensorVal = getFilteredIrms(sensor, sensorID);
  readings[sensorID] = sensor->getIrms();//(sensorVal < UMBRAL ) ? 0 : sensorVal;
}

double getFilteredVrms(EnergyMonitor * sensor, uint8_t sensorId)
{
  #ifdef ENABLE_FILTER
  rawData[sensorId] = sensor->lowPassFilter(
    sensor->getVrms(),
    rawData[sensorId],
    FILTER_SIZE );  
  return rawData[sensorId];
  #else
  return sensor->getVrms();
  #endif
}

double getFilteredIrms(EnergyMonitor * sensor, uint8_t sensorId)
{
  #ifdef ENABLE_FILTER
  rawData[sensorId] = sensor->lowPassFilter(
    sensor->getIrms(),
    rawData[sensorId],
    FILTER_SIZE );  
  return rawData[sensorId];
  #else
  return sensor->getIrms();
  #endif
}

CurrentSensors currentSensors;
