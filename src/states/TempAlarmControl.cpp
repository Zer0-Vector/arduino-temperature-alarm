#include "TempAlarmControl.h"

#include <SSD1306Ascii.h>
#include <LM35.h>
#include <EEPROM.h>

TempAlarmControl::TempAlarmControl() {
    _alarmOn = false;
    _displayOn = true;
    TempAlarmConfig config;
    EEPROM.get(0, config);
    if (config.ID != CONFIG_DATA_ID) {
        _config = TempAlarmConfig();
    } else {
        _config = config;
    }
    this->changeState(ps_IdleState);
}

void TempAlarmControl::keypressed(char key) {
    switch (key) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
        case '#': // aka '.'
            _state->numberInput(key);
            break;
        case '*': // toggle display on/off
            _state->toggleDisplay(this);
            break;
        case 'A': // set min temp
            _state->setMin(this);
            break;
        case 'B': // set max temp
            _state->setMax(this);
            break;
        case 'C': // toggle units C/F
            _convertToF = !_convertToF;
            _saveUnitsToDisk();
            break;
        case 'D': // cancel/silence
            _state->cancel(this);
            break;
        default:
            // error!
    }
}

void TempAlarmControl::changeState(ProgramState* state) {
    _state = state;
}

void TempAlarmControl::tick() {
    _state->tick(this);
}

void TempAlarmControl::triggerAlarm() {
    _alarmOn = true;
}

void TempAlarmControl::render(SSD1306Ascii* oled, LM35* lm35) {
    _renderTempRow(oled, lm35);
    _state->render(this);
}

void TempAlarmControl::_renderTempRow(SSD1306Ascii* oled, LM35* lm35) {
    if (_alarmOn) {
        oled.setInvertMode(true);
    } else {
        oled.setInvertMode(false);
    }
    // TODO render temp
}

void TempAlarmControl::saveConfigToDisk() {
    EEPROM.put(0, this->_config);
}

void toggleUnits() {
    config.convertToF = !config.convertToF;
    saveConfigToDisk();
}