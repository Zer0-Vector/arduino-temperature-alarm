#include "LM35.h"

// TODO add directive error checks

LM35::LM35(uint16_t mv)
: _mv(mv), _sumOfSquares(0.0), _sampleCounter(0), _lastTempAsC(0.0)
{
}

double LM35::tempAsF() {
  return (this->_lastTempAsC * 9.0 / 5.0) + 32.0;
}

double LM35::tempAsC() {
  return this->_lastTempAsC;
}

bool LM35::sampleTemp() {
  int reading = analogRead(LM35_LOAD_PIN);
  double toC = AIN_TO_TEMP(this->_mv, reading);
  this->_sumOfSquares += toC*toC;
  if (++this->_sampleCounter >= LM35_SAMPLE_SIZE) {
    this->_lastTempAsC = sqrt(this->_sumOfSquares / this->_sampleCounter);
    this->_sumOfSquares = 0.0;
    this->_sampleCounter = 0;
    return true;
  }
  delay(LM35_SAMPLING_DELAY);
  return false;
}