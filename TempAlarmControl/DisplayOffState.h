#pragma once

#include "ProgramState.h"

class DisplayOffState : public ProgramState {
    public:
        static DisplayOffState* const INSTANCE;
        void setMin(TempAlarmControl* control);
        void setMax(TempAlarmControl* control);
        void changeUnits(TempAlarmControl* control);
        void toggleDisplay(TempAlarmControl* control);
        void tick(TempAlarmControl* control);
        void entered(TempAlarmControl* control);
        void exiting(TempAlarmControl* control);
        void render(SSD1306Ascii* oled);
    protected:
        DisplayOffState();
};
