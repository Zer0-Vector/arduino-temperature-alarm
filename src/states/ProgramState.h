#pragma once

#include "TempAlarmControl.h"

#define MAX_BUFFER_LEN 5

class ProgramState {
    public:
        ProgramState();
        ~ProgramState();
        virtual void numberInput(char n);
        virtual void setMin(TempAlarmControl* control);
        virtual void setMax(TempAlarmControl* control);
        virtual void changeUnits(TempAlarmControl* control);
        virtual void cancel(TempAlarmControl* control);
        virtual void toggleDisplay(TempAlarmControl* control);
        virtual void tick(TempAlarmControl* control);
    protected:
        void changeState(TempAlarmControl* control, ProgramState* state);
        void clearBuffer();
    private:
        bool _bufferOpen;
        uint8_t _index;
        char* _buffer; // min = 1 (single digit), max = 5 (100.5); negative values are not allowed; interpreted as current units
};
