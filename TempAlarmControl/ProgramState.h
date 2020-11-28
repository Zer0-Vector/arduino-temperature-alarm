#pragma once

#include <SSD1306Ascii.h>
#include "TempAlarmControl.h"

#define MAX_BUFFER_LEN 5

class ProgramState {
    public:
        ~ProgramState();
        virtual void numberInput(char n);
        virtual void setMin(TempAlarmControl* control);
        virtual void setMax(TempAlarmControl* control);
        virtual void cancel(TempAlarmControl* control);
        virtual void toggleDisplay(TempAlarmControl* control);
        virtual void tick(TempAlarmControl* control);
        virtual void render(SSD1306Ascii* oled) = 0;// FIXME implement in subclasses
        void setDisplayOn(TempAlarmControl* control, bool displayOn);
    protected:
        ProgramState();
        void changeState(TempAlarmControl* control, ProgramState* state);
        void clearBuffer();
        unsigned long _timeEntered;

        virtual void entered(TempAlarmControl* control);
        virtual void exiting(TempAlarmControl* control);

    private:
        bool _bufferOpen;
        uint8_t _index;
        char* _buffer; // min = 1 (single digit), max = 5 (100.5); negative values are not allowed; interpreted as current units
};
