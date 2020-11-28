#include "ProgramState.h"

#define DEFAULT_STATE_TIMEOUT 90000

ProgramState::ProgramState() {
    _buffer = (char*)malloc(6*sizeof(char)); // accepts between [1,5] chars; buffer[len] = '\0'
    _index = 0;
    _bufferOpen = false;
}

ProgramState::~ProgramState() {
    free(_buffer);
}

void ProgramState::numberInput(char n) {
    if (_bufferOpen && _index < MAX_BUFFER_LEN) {
        if (n == '#') {
            _buffer[_index++] = '.';
        } else {
            _buffer[_index++] = n;
        }
        _buffer[_index] = '\0';
    }
}

void ProgramState::clearBuffer() {
    for (;_index >= 0; _index--) {
        _buffer[_index] = '\0';
    }
    assert(_index == 0);
}
void ProgramState::setMin(TempAlarmControl* control) {}
void ProgramState::setMax(TempAlarmControl* control) {}
void ProgramState::changeUnits(TempAlarmControl* control) {
    control->toggleUnits();
}
void ProgramState::cancel(TempAlarmControl* control) {}
void ProgramState::toggleDisplay(TempAlarmControl* control) {}
void ProgramState::tick(TempAlarmControl* control) {
    if (timeoutExpired()) {
        this->changeState(control, TempAlarmControl::ps_DisplayOff);
    }
}
void ProgramState::entered(TempAlarmControl* control) {
    _timeEntered = millis();
}
void ProgramState::exiting(TempAlarmControl* control) {}

void ProgramState::changeState(TempAlarmControl* control, ProgramState* state) {
    exiting(control);
    control->changeState(state);
    state->entered(control);
}