#include "SettingMinState.h"
#include "IdleState.h"
#include "DisplayOffState.h"

SettingMinState::SettingMinState() : ProgramState() {}

void SettingMinState::setMin(TempAlarmControl* control) {
    // TODO saveConfigToDisk()
    changeState(control, IdleState::INSTANCE);
}
void SettingMinState::cancel(TempAlarmControl* control) {
    changeState(control, IdleState::INSTANCE);
}
void SettingMinState::toggleDisplay(TempAlarmControl* control) {
    changeState(control, DisplayOffState::INSTANCE);
}
void SettingMinState::exiting(TempAlarmControl* control) {
    clearBuffer();
}
void SettingMinState::render(SSD1306Ascii* oled) {
    // TODO
}

SettingMinState* const SettingMinState::INSTANCE = new SettingMinState();
