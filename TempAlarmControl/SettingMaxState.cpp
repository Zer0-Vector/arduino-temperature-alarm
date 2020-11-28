#include "SettingMaxState.h"
#include "IdleState.h"
#include "DisplayOffState.h"

SettingMaxState::SettingMaxState() : ProgramState() {}

void SettingMaxState::setMax(TempAlarmControl* control) {
    // TODO saveConfigToDisk()
    changeState(control, IdleState::INSTANCE);
}
void SettingMaxState::cancel(TempAlarmControl* control) {
    changeState(control, IdleState::INSTANCE);
}
void SettingMaxState::toggleDisplay(TempAlarmControl* control) {
    changeState(control, DisplayOffState::INSTANCE);
}
void SettingMaxState::exiting(TempAlarmControl* control) {
    clearBuffer();
}
void SettingMaxState::render(SSD1306Ascii* oled) {
    // TODO
}

SettingMaxState* const SettingMaxState::INSTANCE = new SettingMaxState();
