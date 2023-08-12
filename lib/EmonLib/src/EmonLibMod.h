#ifndef EMONLIB_H
#define EMONLIB_H
#include <Arduino.h>
#include <math.h>

#define SUPPLYVOLTAGE 3.300
#define ADC_BITS    12
#define ADC_COUNTS  (1<<ADC_BITS)
#define ADC_RES 4096

typedef struct ACPower {
  /* Real Power Value */
  double realPower;

  /* Apparent Power Value*/
  double apparentPower;

  /* Power Factor Value*/
  double powerFactor;

  /* Instantaneous Power Value */
  double instP;

  /* Accumulative Power Value */
  double sumP;
}ACPower;

typedef struct voltageSensor {

  /* Voltage Sensor value */
  double vrms;

  /* Voltage Sensor Calibration Value */
  float VCAL;

  /* Voltage Signal Phase Calibration */
  double PHASECAL;

  /* Filter Voltage Values */
  double lastFilteredV, filteredV;

  /* Low pass filter - DC Voltage Value */
  double offsetV;

  /* Voltage Signal Shifted Phase Value (for Zero Crossing Detector) */
  double phaseShiftedV;

  /* Voltage Sensor ADC Sample Value */
  uint16_t sampleV;

  /* First Voltage Sensor ADC Sample Value*/
  uint16_t startV;

  /* Accumulated Square Root Voltage Value */
  double sumV;

  /* Square Root Voltage Value*/
  double sqV;

  /* Previous Voltage Signal Zero Crosssing Flag */
  uint8_t lastVCross;

  /* Voltage Signal Zero Crosssing Flag */
  uint8_t checkVCross;
}voltageSensor;

typedef struct currentSensor {
  /* Current Sensor value */
	double irms;

  /* Voltage Sensor Calibration Value */
	float ICAL;

  /* Voltage Sensor ADC Sample Value */
  int sampleI;

  /* Low Pass Filter Value */
  double filteredI;

  /* Low pass filter - DC Voltage Value (constant value) */
	double offsetI;
  
  /* Accumulated Square Root Current Value */
  double sumI;

  /* Square Root Current Value*/
	double sqI;
	
}currentSensor;

// Define the callback function type
typedef uint16_t (*ADCReadFunction)(uint8_t channel);

/* TODO: EnergyMonitor Lib code refactoring */
class EnergyMonitor
{
private:
  ACPower power;
  voltageSensor voltage;
  currentSensor current;
  uint8_t currentSensorPin;
  uint8_t voltageSensorPin;
  ADCReadFunction adcReadCallback; // Function pointer for ADC read

public:

  EnergyMonitor (uint8_t voltage_pin, uint8_t current_pin);

  void attachADCReadCallback(ADCReadFunction adcReadFunc); // Method to attach callback

  void currentSensorBegin(float iCal);
  void voltageSensorBegin(float phaseCal, float vCal);

  void calcVI();
  void calcIRMS();

  double lowPassFilter(double inputData, double lastData, float filterSize);

  double getVrms();
  double getIrms();
  double getPowerFactor();

};

#endif
