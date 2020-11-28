#pragma once

#include "ProgramState.h"

class SettingMaxState : public ProgramState {
    public:
        static SettingMaxState* const INSTANCE;
        void setMax(TempAlarmControl* control); // "ENTER/SAVE"
        void cancel(TempAlarmControl* control);
        void exiting(TempAlarmControl* control);
        void toggleDisplay(TempAlarmControl* control);
        void render(SSD1306Ascii* oled);
    protected:
    
        SettingMaxState();
};
