#pragma once

#include "ProgramState.h"

class SettingMinState : public ProgramState {
    public:
        const static SettingMinState* INSTANCE;
        virtual void setMin(TempAlarmControl* control); // "ENTER/SAVE"
        virtual void cancel(TempAlarmControl* control);
        virtual void toggleDisplay(TempAlarmControl* control);
    protected:
        SettingMinState();
};
