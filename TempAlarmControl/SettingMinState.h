#pragma once

#include "ProgramState.h"

class SettingMinState : public ProgramState {
    public:
        static SettingMinState* const INSTANCE;
        void setMin(TempAlarmControl* control); // "ENTER/SAVE"
        void cancel(TempAlarmControl* control);
        void toggleDisplay(TempAlarmControl* control);
        void exiting(TempAlarmControl* control);
        void render(SSD1306Ascii* oled);
    protected:
        SettingMinState();
};
