#include "ProgramState.h"

class DisplayTemp : public ProgramState {
    public:
        virtual void setMin(TempAlarmControl* control);
        virtual void setMax(TempAlarmControl* control);
        virtual void changeUnits(TempAlarmControl* control);
        virtual void cancel(TempAlarmControl* control);
        virtual void toggleDisplay(TempAlarmControl* control);
};