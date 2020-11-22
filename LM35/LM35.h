#ifndef _LM35_H_
#define _LM35_H_

#include <Arduino.h>

// V [mV] = R * (1100 / 1024) [mV/steps]
// T [degC] = V / 10 [mV/degC]
// T = R * ((1100 / 1024) / 10) = R * LM35_CONVERSION_CONSTANT
#define LM35_CONVERSION_CONSTANT ((double)0.107421875)
typedef byte tempUnits_t;
#define FARENHEIGHT (tempUnits_t)0
#define CELCIUS (tempUnits_t)1

class LM35 {
  public:
    LM35(uint8_t pin = A0, uint8_t numberOfSamples = 64, tempUnits_t units = FARENHEIGHT, uint8_t delayAfterReading = 25);
    /**
     * returns true if temperature was updated
     */
    bool sampleTemp();
    double currentTemp();
    void setUnits(tempUnits_t units);
  private:
    uint8_t _pin;
    uint8_t _numberOfSamples;
    tempUnits_t _units;
    uint8_t _delayAfterReading;
    double _sumOfSquares;
    uint8_t _sampleCounter;
    double _lastTempAsC;
    double tempAsC();
    double tempAsF();
};

#endif