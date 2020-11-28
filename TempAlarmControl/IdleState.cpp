#include "IdleState.h"
#include "SettingMinState.h"
#include "SettingMaxState.h"
#include "DisplayOffState.h"
#include "Arduino.h"

IdleState::IdleState() : ProgramState() {}

void IdleState::setMin(TempAlarmControl* control) {
    changeState(control, SettingMinState::INSTANCE);
}

void IdleState::setMax(TempAlarmControl* control) {
    changeState(control, SettingMaxState::INSTANCE);
}

void IdleState::toggleDisplay(TempAlarmControl* control) {
    // this state implies display is on
    changeState(control, DisplayOffState::INSTANCE);
}

void IdleState::tick(TempAlarmControl* control) {
    if (millis() - _timeEntered > DISPLAY_ON_TIMEOUT) {
        changeState(control, DisplayOffState::INSTANCE);
    }
}

void IdleState::render(SSD1306Ascii* oled) {
    // TODO
}

IdleState* const IdleState::INSTANCE = new IdleState();
