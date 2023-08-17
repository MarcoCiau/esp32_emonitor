#ifndef CURRENTSENSOR_H
#define CURRENTSENSOR_H
#include <Arduino.h>
#include <EmonLib.h>
#include "HWConfig.h"
class CurrentSensors
{
private:
  float calibration[NUM_MAX_CURRENT_SENSOR];
  double readings[NUM_MAX_CURRENT_SENSOR];
  EnergyMonitor sensor[NUM_MAX_CURRENT_SENSOR];
  uint8_t lastSensorRead;
  /* Execute Sensors Readings*/
  void readNow(uint8_t nSensor);
  void debugSensors();
public:
  void begin();
  void loop();
  double getCurr(uint8_t nSensor);
};

extern CurrentSensors currentSensors;

#endif // CURRENTSENSOR_H
