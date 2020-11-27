#include "ProgramState.h"

ProgramState::ProgramState() {
    _buffer = (char*)malloc(6*sizeof(char)); // accepts between [1,5] chars; buffer[len] = '\0'
    _index = 0;
    _bufferOpen = false;
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
void ProgramState::changeUnits(TempAlarmControl* control) {}
void ProgramState::cancel(TempAlarmControl* control) {}
void ProgramState::toggleDisplay(TempAlarmControl* control) {}
void ProgramState::tick(TempAlarmControl* control) {}

void ProgramState::changeState(TempAlarmControl* control, ProgramState* state) {
    control->changeState(state);
}