#pragma once

#include "ProgramState.h"

class DisplayOffState : public ProgramState {
    public:
        const static DisplayOffState* INSTANCE;
        virtual void setMin(TempAlarmControl* control);
        virtual void setMax(TempAlarmControl* control);
        virtual void changeUnits(TempAlarmControl* control);
        virtual void toggleDisplay(TempAlarmControl* control);
        virtual void tick(TempAlarmControl* control);
        virtual void entered(TempAlarmControl* control);
        virtual void exiting(TempAlarmControl* control);
    protected:
        DisplayOffState();
};
