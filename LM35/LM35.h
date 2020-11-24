#ifndef _LM35_H_
#define _LM35_H_
#pragma once

#include <Arduino.h>

#define _XSTR(x) #x
#define _STR(x) _XSTR(x)

// with analogReference(INTERNAL) (on Uno/Nano)
// V [mV] = R * (1100 / 1024) [mV/steps]
// T [degC] = V / 10 [mV/degC]
// T = R * ((1100 / 1024) / 10) = R * LM35_CONVERSION_CONSTANT
#define _LM35_CONVERSION_CONSTANT 0.107421875

#ifndef LM35_SAMPLE_SIZE
#define LM35_SAMPLE_SIZE 64
#endif
#pragma message("LM35_SAMPLE_SIZE " _STR(LM35_SAMPLE_SIZE))

#ifndef LM35_LOAD_PIN
#define LM35_LOAD_PIN A0
#endif
#pragma message("LM35 PIN " _STR(LM35_LOAD_PIN))

#ifndef LM35_SAMPLING_DELAY
#define LM35_SAMPLING_DELAY 25
#endif

class LM35 {
  public:
    LM35();

    /**
     * returns true if temperature was updated
     */
    bool sampleTemp();
    /**
     * returns last sampled temp
     */
    double tempAsC();
    double tempAsF();
  private:
    double _sumOfSquares;
    uint8_t _sampleCounter;
    double _lastTempAsC;
};

#endif // _LM35_H_