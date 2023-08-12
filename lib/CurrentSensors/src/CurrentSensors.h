#ifndef CURRENTSENSOR_H
#define CURRENTSENSOR_H
#include <Arduino.h>
#include <EmonLibMod.h>

class CurrentSensors
{
private:
  double readings[20];// 1 x volt val, 3 x current val, 2 x power factor val
  uint8_t lastSensorRead;

  /* Execute Sensors Readings*/
  void readNow(uint8_t nSensor);
  void handleVISensor(EnergyMonitor * sensor, uint8_t sensorID);
  void handleISensor(EnergyMonitor * sensor, uint8_t sensorID);
  void debugSensors();
public:
  void begin();
  void loop();
  double getCurr(uint8_t nSensor);
};

extern CurrentSensors currentSensors;

#endif // CURRENTSENSOR_H
