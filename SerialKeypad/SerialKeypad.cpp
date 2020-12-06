#include "SerialKeypad.h"
#include <Arduino.h>

SerialKeypad::SerialKeypad(uint8_t pin_clk, uint8_t pin_ld, uint8_t pin_cke, uint8_t pin_data) {
    _clk = pin_clk;
    _ld = pin_ld;
    _cke = pin_cke;
    _data = pin_data;
    pinMode(_clk, OUTPUT);
    pinMode(_ld, OUTPUT);
    pinMode(_cke, OUTPUT);
    pinMode(_data, INPUT);
}
byte SerialKeypad::getKey() {
    _pulse(_ld);
    digitalWrite(_clk, HIGH);
    digitalWrite(_cke, LOW);
    byte b = shiftIn(_data, _clk, MSBFIRST);
    digitalWrite(_cke, HIGH);
    _reading.valueIn(b ^ 0xF0);
    return _reading.value();
}

#define PULSE_DELAY 5
void SerialKeypad::_pulse(uint8_t pin) {
    digitalWrite(pin, LOW);
    delay(PULSE_DELAY);
    digitalWrite(pin, HIGH);
    delay(PULSE_DELAY);
}
