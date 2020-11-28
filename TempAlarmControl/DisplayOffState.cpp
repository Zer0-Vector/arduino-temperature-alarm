#include "DisplayOffState.h"
#include "SettingMaxState.h"
#include "SettingMinState.h"
#include "IdleState.h"

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
    setDisplayOn(control, false);
}

void DisplayOffState::exiting(TempAlarmControl* control) {
    setDisplayOn(control, true);
}

void DisplayOffState::tick(TempAlarmControl* control) {
    // nop; remain off
}

void DisplayOffState::render(SSD1306Ascii* oled) {
    // nop
}

DisplayOffState* const DisplayOffState::INSTANCE = new DisplayOffState();
