#include "TempAlarmControl.h"

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
            _progState->numberInput(key);
            break;
        case '*': // toggle display on/off
            _progState->toggleDisplay(this);
            break;
        case 'A': // set min temp
            _progState->setMin(this);
            break;
        case 'B': // set max temp
            _progState->setMax(this);
            break;
        case 'C': // toggle units C/F
            _progState->changeUnits(this);
            break;
        case 'D': // cancel/silence
            _progState->cancel(this);
            break;
        default:
            // error!
    }
}

void TempAlarmControl::changeProgramState(ProgramState* state) {
    _progState = state;
}

void TempAlarmControl::changeDisplayState(DisplayState* state) {
    _dispState = state;
}