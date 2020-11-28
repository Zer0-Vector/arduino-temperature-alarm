#pragma once

#include "ProgramState.h"

class SettingMaxState : public ProgramState {
    public:
        const static SettingMaxState* INSTANCE;
        virtual void setMax(TempAlarmControl* control); // "ENTER/SAVE"
        virtual void cancel(TempAlarmControl* control);
        virtual void toggleDisplay(TempAlarmControl* control);
    protected:
        SettingMaxState();
};
