#pragma once

#include "ProgramState.h"

#define DISPLAY_ON_TIMEOUT 7500

class IdleState : public ProgramState {
    public:
        const static IdleState* INSTANCE;
        virtual void setMin(TempAlarmControl* control);
        virtual void setMax(TempAlarmControl* control);
        virtual void changeUnits(TempAlarmControl* control);
        virtual void toggleDisplay(TempAlarmControl* control);
        virtual void tick(TempAlarmControl* control);
    protected:
        IdleState();
    private:
        unsigned long _timeEntered;
};
