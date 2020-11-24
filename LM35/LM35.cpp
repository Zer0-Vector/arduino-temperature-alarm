#include "LM35.h"

// TODO add directive error checks

LM35::LM35()
: _sumOfSquares(0.0), _sampleCounter(0), _lastTempAsC(0.0)
{}

double LM35::tempAsF() {
  return (this->_lastTempAsC * 9.0 / 5.0) + 32.0;
}

double LM35::tempAsC() {
  return this->_lastTempAsC;
}

bool LM35::sampleTemp() {
  int reading = analogRead(LM35_LOAD_PIN);
  this->_sumOfSquares += _LM35_CONVERSION_CONSTANT*_LM35_CONVERSION_CONSTANT * reading*reading;
  if (++this->_sampleCounter >= LM35_SAMPLE_SIZE) {
    this->_lastTempAsC = sqrt(this->_sumOfSquares / this->_sampleCounter);
    this->_sumOfSquares = 0.0;
    this->_sampleCounter = 0;
    return true;
  }
  delay(LM35_SAMPLING_DELAY);
  return false;
}