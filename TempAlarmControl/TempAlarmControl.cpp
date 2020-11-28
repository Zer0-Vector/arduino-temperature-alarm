#include "TempAlarmControl.h"
#include "ProgramState.h"

#include <SSD1306Ascii.h>
#include <LM35.h>
#include <EEPROM.h>

TempAlarmControl::TempAlarmControl() {
    _invertDisplay = false;
    _alarmSoundOn = false;
    _displayOn = true;
    TempAlarmConfig* config;
    EEPROM.get(0, *config);
    if (config->ID == CONFIG_DATA_ID) {
        _config = config;
    }
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
            _toggleUnits();
            break;
        case 'D': // cancel/silence
            _state->cancel(this);
            break;
        default:
            // error!
            ; // TODO log debug msg
    }
}

void TempAlarmControl::changeState(ProgramState* state) {
    _state = state;
}

void TempAlarmControl::tick(LM35* lm35) {
    _state->tick(this);
    if (_checkTemp(lm35)) {
        _alarmOn();
    } else {
        _alarmOff();
    }

    if (_alarmSoundOn) {
        _playSound();
    }
}

void TempAlarmControl::_alarmOn() {
    _invertDisplay = true;
    _alarmSoundOn = true;
}

void TempAlarmControl::_alarmOff() {
    _invertDisplay = false;
    _alarmSoundOn = false;
}

void TempAlarmControl::suppressAlarmSound() {
    _alarmSoundOn = false;
}

void TempAlarmControl::render(SSD1306Ascii* oled, LM35* lm35) {
    _renderTempRow(oled, lm35);
    _state->render(oled);
}

void TempAlarmControl::_renderTempRow(SSD1306Ascii* oled, LM35* lm35) {
    if (_invertDisplay) {
        oled->setInvertMode(true);
    } else {
        oled->setInvertMode(false);
    }
    // TODO render temp
    static uint8_t lastMag = 0;
    double t;
    if (_config->convertToF) {
        t = lm35->tempAsF();
    } else {
        t = lm35->tempAsC();
    }
    Serial.println(t,3);
    char *cstemp = (char*)malloc(8*sizeof(char));
    uint8_t mag = (uint8_t)log10(abs(t));
    dtostrf(t, 3 + mag, 1, cstemp);

    Serial.print("update display temp: ");
    Serial.println(cstemp);
    if (mag != lastMag) {
        oled->clear();
    }
    lastMag = mag;

    oled->setFont(TEMP_FONT);
    oled->setCursor(LEFT_PADDING, 0);
    oled->print(cstemp);
    oled->setFont(UTF_FONT);
    oled->print('\260');
    oled->setFont(TEMP_FONT);
    if (_config->convertToF) {
        oled->println('F');
    } else {
        oled->println('C');
    }
    free(cstemp);
}

void TempAlarmControl::saveConfigToDisk() {
    EEPROM.put(0, this->_config);
}

void TempAlarmControl::_toggleUnits() {
    _config->convertToF = !_config->convertToF;
    saveConfigToDisk();
}

void TempAlarmControl::setDisplayOn(bool displayOn) {
    this->_displayOn = displayOn;
}
