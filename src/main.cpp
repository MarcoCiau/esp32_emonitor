#include <Arduino.h>
#include <ADC_74HC4066.h>
#include <hwConfig.h>
#include <CurrentSensors.h>
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  currentSensors.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  currentSensors.loop();
    // Serial.println("New ADC Read");
  // for (uint8_t i = 0; i < 3; i++)
  // {
  //   Serial.printf("ADC%d: %d,", i + 1, adc.read(i));
  //   delay(20);
  // }
  // Serial.println();
  // delay(250);
}
