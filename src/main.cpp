#include <Arduino.h>
#include <ADC_74HC4066.h>
#define ANALOG_74HC4067 35
#define S0_74HC4067 32
#define S1_74HC4067 33
#define S2_74HC4067 13
#define S3_74HC4067 16

ADC_74HC4066 adc(S0_74HC4067,
                 S1_74HC4067,
                 S2_74HC4067,
                 S3_74HC4067,
                 ANALOG_74HC4067);
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("New ADC Read");
  for (uint8_t i = 0; i < 16; i++)
  {
    adc.selectChannel(i);
    delay(2);
    int val = adc.read();
    Serial.printf("ADC%d: %d\n", i + 1, val);
  }
  delay(1000);
}
