#include "ADC_74HC4066.h"

ADC_74HC4066::ADC_74HC4066(int s0Pin, int s1Pin, int s2Pin, int s3Pin, int analogPin)
    : s0(s0Pin), s1(s1Pin), s2(s2Pin), s3(s3Pin), analogInput(analogPin) {
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    pinMode(analogInput, INPUT);
}

void ADC_74HC4066::selectChannel(int channel) {
    if (channel >= 0 && channel < 16) {
        digitalWrite(s0, channel & 0x01);
        digitalWrite(s1, (channel >> 1) & 0x01);
        digitalWrite(s2, (channel >> 2) & 0x01);
        digitalWrite(s3, (channel >> 3) & 0x01);
    }
}

int ADC_74HC4066::read() {
    int value = analogRead(analogInput);
    return value;
}
