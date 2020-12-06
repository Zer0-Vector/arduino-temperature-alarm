#pragma once

#include <Debounced.h>

class SerialKeypad {
    public:
        SerialKeypad(uint8_t pin_clk, uint8_t pin_ld, uint8_t pin_cke, uint8_t pin_data);
        byte getKey();
    private:
        void _pulse(uint8_t pin);
        uint8_t _clk;
        uint8_t _ld;
        uint8_t _cke;
        uint8_t _data;
        Debounced _reading;
};