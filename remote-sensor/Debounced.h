#ifndef _DEBOUNCED_H_
#define _DEBOUNCED_H_

#include <Arduino.h>

#define DEFAULT_DEBOUNCE_DELAY 50
class Debounced {
    public:
        Debounced(uint8_t dpin, unsigned long delay = DEFAULT_DEBOUNCE_DELAY);
        /**
         * returns true if value has changed
         */
        bool read();
        int value();
        void setValue(int value);
    private:
        uint8_t _pin;
        unsigned long _delay;
        int _value;
        int _lastValue;
        int _lastTime;
};

#endif