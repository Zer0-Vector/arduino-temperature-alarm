#include "Debounced.h"

Debounced::Debounced(uint8_t dpin, unsigned long delay)
: _pin(dpin), _delay(delay), _value(-1), _lastTime(0)
{}

bool Debounced::read() {
    int reading = digitalRead(this->_pin);
    if (reading != this->_lastValue) {
        this->_lastTime = millis();
    }
    this->_lastValue = reading;
    if (millis() - this->_lastTime > this->_delay) {
        this->_value = reading;
        return true;
    }
    return false;
}

int Debounced::value() {
    return this->_value;
}
void Debounced::setValue(int value) {
    this->_value = value;
}