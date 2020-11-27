#include "ProgramState.h"

class SettingMin : public ProgramState {
    public:
        // save min value, clear buffer, return to displayTemp
        virtual void setMin(TempAlarmControl* control);
        // buffer number until max is reached, then nop
        virtual void numberInput(char n);
        // clear buffer, return to displayTemp
        virtual void cancel(TempAlarmControl* control);
}