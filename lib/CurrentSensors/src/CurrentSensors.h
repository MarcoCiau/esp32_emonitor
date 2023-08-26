#ifndef CURRENTSENSOR_H
#define CURRENTSENSOR_H
#include <Arduino.h>
#include <EmonLib.h>
#include "HWConfig.h"
class CurrentSensors
{
private:
  float calibration[NUM_MAX_CURRENT_SENSOR];
  float readings[NUM_MAX_CURRENT_SENSOR];
  EnergyMonitor sensor[NUM_MAX_CURRENT_SENSOR];
  uint8_t lastSensorRead;
  /* Execute Sensors Readings*/
  void readNow(uint8_t channel);
  void debugSensors();
public:
  void begin();
  void loop();
  float getValue(uint8_t channel);
  float getCalibration(uint8_t channel);
  void setCalibration(uint8_t channel, float value);
};

extern CurrentSensors currentSensors;

#endif // CURRENTSENSOR_H
