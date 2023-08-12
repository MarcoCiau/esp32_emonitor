#include "EmonLibMod.h"

#define NO_OF_SAMPLES 600
#define ZERO_CROSSING_MAX_COUNTS 10
#define ZERO_CROSSING_DETECTOR_TIMEOUT_MS 250

EnergyMonitor::EnergyMonitor (uint8_t voltage_pin, uint8_t current_pin){
  this->voltageSensorPin = voltage_pin;
  this->currentSensorPin = current_pin;
}

void EnergyMonitor::currentSensorBegin(float iCal)
{
  current.ICAL = iCal;
  current.offsetI = ADC_COUNTS >> 1;
  current.irms = 0;
  current.sumI = 0;
}

void EnergyMonitor::attachADCReadCallback(ADCReadFunction adcReadFunc) {
  adcReadCallback = adcReadFunc; // Attach the callback function
}

void EnergyMonitor::voltageSensorBegin(float phaseCal, float vCal)
{
  voltage.VCAL = vCal;
  voltage.PHASECAL = phaseCal;
  voltage.offsetV = ADC_COUNTS >> 1;
  
  voltage.vrms = 0;
  voltage.sumV = 0;
  power.sumP = 0;
}

void EnergyMonitor::calcVI()
{
  uint16_t crossCount = 0;
  uint16_t numberOfSamples = 0;

  //millis()-start makes sure it doesnt get stuck in the loop if there is an error.
  unsigned long start = millis();   

  while (1)
  {
    //Read in raw voltage signal                   //using the voltage waveform
    voltage.startV = adcReadCallback(voltageSensorPin);
    if( (voltage.startV < (ADC_COUNTS * 0.55)) && (voltage.startV > (ADC_COUNTS * 0.45))) break;
    if ( (millis() - start) > ZERO_CROSSING_DETECTOR_TIMEOUT_MS) break;
  }

  start= millis();

  while ( (crossCount < ZERO_CROSSING_MAX_COUNTS) && ( (millis()- start) < ZERO_CROSSING_DETECTOR_TIMEOUT_MS) )
  {
    //Count number of times looped
    numberOfSamples++;
    voltage.lastFilteredV = voltage.filteredV; //Used for delay/phase compensation

    //Read raw voltage signal
    voltage.sampleV = adcReadCallback(voltageSensorPin);
    current.sampleI = adcReadCallback(currentSensorPin);

		//Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
		//then subtract this - signal is now centered on 0 counts
    voltage.offsetV = voltage.offsetV + ( (voltage.sampleV - voltage.offsetV) / ADC_RES);
    voltage.filteredV = voltage.sampleV - voltage.offsetV;

    current.offsetI = current.offsetI + ( (current.sampleI - current.offsetI) / ADC_RES);
    current.filteredI = current.sampleI - current.offsetI;

		//-----------------------------------------------------------------------------
		// C) Root-mean-square method voltage
		//-----------------------------------------------------------------------------
		//1) square voltage values
    voltage.sqV = voltage.filteredV * voltage.filteredV;
    //2) sum
    voltage.sumV += voltage.sqV;

		//-----------------------------------------------------------------------------
		// D) Root-mean-square method current
		//-----------------------------------------------------------------------------
		//1) square current values
    current.sqI = current.filteredI * current.filteredI;
    //2) sum
    current.sumI += current.sqI;

		//-----------------------------------------------------------------------------
		// E) Phase calibration
		//-----------------------------------------------------------------------------
    voltage.phaseShiftedV = voltage.lastFilteredV + voltage.PHASECAL * (voltage.filteredV - voltage.lastFilteredV);
  	// self->phaseShiftedV = self->lastFilteredV + self->PHASECAL * (self->filteredV - self->lastFilteredV);

		//-----------------------------------------------------------------------------
		// F) Instantaneous power calc
		//-----------------------------------------------------------------------------
		//Instantaneous Power
    power.instP = voltage.phaseShiftedV * current.filteredI;
    // self->instP = self->phaseShiftedV * self->filteredI;          //Instantaneous Power
    //Sum
    power.sumP += power.instP;
		// self->sumP += self->instP;                               //Sum
		//-----------------------------------------------------------------------------
		// G) Find the number of times the voltage has crossed the initial voltage
		//    - every 2 crosses we will have sampled 1 wavelength
		//    - so this method allows us to sample an integer number of half wavelengths which increases accuracy
		//-----------------------------------------------------------------------------
		voltage.lastVCross = voltage.checkVCross;
    // self->lastVCross = self->checkVCross;
    if (voltage.sampleV > voltage.startV) voltage.checkVCross = 1;
    else voltage.checkVCross = 0;
		// if (self->sampleV > self->startV) self->checkVCross = 1;
		// else self->checkVCross = 0;
    if (numberOfSamples == 1)  voltage.lastVCross = voltage.checkVCross;
		// if (numberOfSamples==1) self->lastVCross = self->checkVCross;
    if (voltage.lastVCross != voltage.checkVCross) crossCount++;
		// if (self->lastVCross != self->checkVCross) crossCount++;
  }
  
	//-------------------------------------------------------------------------------------------------------------------------
	// 3) Post loop calculations
	//-------------------------------------------------------------------------------------------------------------------------
	//Calculation of the root of the mean of the voltage and current squared (rms)
	//Calibration coefficients applied.
  double V_RATIO = voltage.VCAL * (SUPPLYVOLTAGE / (ADC_COUNTS));
	voltage.vrms = V_RATIO * sqrt( voltage.sumV / numberOfSamples);

  double I_RATIO = current.ICAL * (SUPPLYVOLTAGE / (ADC_COUNTS));
  current.irms = I_RATIO * sqrt(current.sumI / numberOfSamples);

	//Calculation power values
  power.realPower = V_RATIO * I_RATIO * power.sumP / numberOfSamples;
  power.apparentPower = voltage.vrms * current.irms;
  power.powerFactor = power.realPower / power.apparentPower;

	//Reset accumulators
  voltage.sumV = 0;
  current.sumI = 0;
  power.sumP = 0;
  
}

void EnergyMonitor::calcIRMS()
{

	for (uint16_t n = 0; n < NO_OF_SAMPLES; n++)
	{
    //read raw ADC current signal
    current.sampleI = adcReadCallback(currentSensorPin); 
    if (current.sampleI <= 0)
    {
      current.irms = 0;
      return;
    } 

		//Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
		//then subtract this - signal is now centered on 0 counts
    current.offsetI = current.offsetI + ((current.sampleI - current.offsetI)/ADC_RES);
    current.filteredI = current.sampleI - current.offsetI;

		//Root-mean-square method current
		//1) square current values
    current.sqI = current.filteredI * current.filteredI;
		//2) sum
    current.sumI += current.sqI;
	}

  //CAL IRMS with CALIBRATION Values
  double I_RATIO = current.ICAL * (SUPPLYVOLTAGE / (ADC_COUNTS));
  current.irms = I_RATIO * sqrt(current.sumI / NO_OF_SAMPLES);

	//Reset accumulators
  current.sumI = 0;
}

double EnergyMonitor::lowPassFilter(double inputData, double lastData, float filterSize)
{
  double filterOutput = lastData - (filterSize*(lastData - inputData));
  return filterOutput;
}

double EnergyMonitor::getVrms()
{
  return voltage.vrms;
}

double EnergyMonitor::getIrms()
{
  return current.irms;
}

double EnergyMonitor::getPowerFactor()
{
  return power.powerFactor;
}


