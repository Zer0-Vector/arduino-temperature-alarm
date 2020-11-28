#pragma once

#include "ProgramState.h"

#define DISPLAY_ON_TIMEOUT 7500

class IdleState : public ProgramState {
    public:
        static IdleState* const INSTANCE;
        void setMin(TempAlarmControl* control);
        void setMax(TempAlarmControl* control);
        void changeUnits(TempAlarmControl* control);
        void toggleDisplay(TempAlarmControl* control);
        void tick(TempAlarmControl* control);
        void render(SSD1306Ascii* oled);
    protected:
        IdleState();
    private:
        unsigned long _timeEntered;
};
