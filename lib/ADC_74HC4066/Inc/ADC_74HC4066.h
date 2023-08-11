#ifndef ADC_74HC4066_H
#define ADC_74HC4066_H

#include <Arduino.h>

/**
 * @brief Class to control a 74HC4066-based analog multiplexer for ADC readings.
 */
class ADC_74HC4066 {
public:
    /**
     * @brief Constructor.
     * @param s0Pin Control pin S0.
     * @param s1Pin Control pin S1.
     * @param s2Pin Control pin S2.
     * @param s3Pin Control pin S3.
     * @param analogPin Analog input pin.
     */
    ADC_74HC4066(int s0Pin, int s1Pin, int s2Pin, int s3Pin, int analogPin);

    /**
     * @brief Selects the specified channel on the multiplexer.
     * @param channel Channel number (0 to 15).
     */
    void selectChannel(int channel);

    /**
     * @brief Reads and returns the analog value from the selected channel.
     * @return Analog value.
     */
    int read();

private:
    int s0, s1, s2, s3;
    int analogInput;
};

#endif // ADC_74HC4066_H
