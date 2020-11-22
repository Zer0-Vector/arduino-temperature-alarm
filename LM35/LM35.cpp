#include "LM35.h"

LM35::LM35(uint8_t pin, uint8_t numberOfSamples, tempUnits_t units, uint8_t delayAfterReading) 
:
  _pin(pin), _numberOfSamples(numberOfSamples), _units(units), _delayAfterReading(delayAfterReading),
  _sumOfSquares(0), _sampleCounter(0), _lastTempAsC(0.0)
{}


double LM35::currentTemp() {
  switch(_units) {
    case FARENHEIGHT:
      return this->tempAsF();
    default:
      return this->tempAsC();
  }
}

double LM35::tempAsC() {
  return this->_lastTempAsC;
}

double LM35::tempAsF() {
  return (this->_lastTempAsC * 9.0 / 5.0) + 32.0;
}

bool LM35::sampleTemp() {
  int reading = analogRead(this->_pin);
  this->_sumOfSquares += LM35_CONVERSION_CONSTANT*LM35_CONVERSION_CONSTANT * reading*reading;
  if (++this->_sampleCounter >= this->_numberOfSamples) {
    this->_lastTempAsC = sqrt(this->_sumOfSquares / this->_sampleCounter);
    this->_sumOfSquares = 0.0;
    this->_sampleCounter = 0;
    return true;
  }
  delay(this->_delayAfterReading);
  return false;
}