#include "IdleState.h"
#include "Arduino.h"

IdleState::IdleState() : ProgramState() {}

void IdleState::setMin(TempAlarmControl* control) {
    control->changeState(SettingMinState::INSTANCE);
}

void IdleState::setMax(TempAlarmControl* control) {
    control->changeState(SettingMaxState::INSTANCE);
}

void IdleState::changeUnits(TempAlarmControl* control) {
    control->toggleUnits();
}

void IdleState::toggleDisplay(TempAlarmControl* control) {
    // this state implies display is on
    control->changeState(DisplayOffState::INSTANCE);
}

void IdleState::tick(TempAlarmControl* control) {
    if (millis() - _timeEntered > DISPLAY_ON_TIMEOUT) {
        control->changeState(DisplayOffState::INSTANCE);
    }
}

const IdleState* IdleState::INSTANCE = new IdleState();
