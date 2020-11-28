#include "DisplayOffState.h"

DisplayOffState::DisplayOffState() : ProgramState() {}

void DisplayOffState::setMin(TempAlarmControl* control) {
    changeState(control, SettingMinState::INSTANCE);
}

void DisplayOffState::setMax(TempAlarmControl* control) {
    changeState(control, SettingMaxState::INSTANCE);
}

void DisplayOffState::changeUnits(TempAlarmControl* control) {
    // nop; don't toggle units when display is off
}

void DisplayOffState::toggleDisplay(TempAlarmControl* control) {
    // this state implies display is on
    changeState(control, IdleState::INSTANCE);
}

void DisplayOffState::entered(TempAlarmControl* control) {
    control->setDisplayOn(false);
}

void DisplayOffState::exiting(TempAlarmControl* control) {
    control->setDisplayOn(true);
}

void DisplayOffState::tick(TempAlarmControl* control) {
    // nop; remain off
}

const DisplayOffState* DisplayOffState::INSTANCE = new DisplayOffState();
