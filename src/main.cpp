#include <Arduino.h>
#include <ADC_74HC4066.h>
#include <CurrentSensors.h>
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  currentSensors.begin();
}

void loop()
{
  currentSensors.loop();
}
