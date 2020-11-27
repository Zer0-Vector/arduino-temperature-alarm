#pragma once

class ProgramState;

class TempAlarmControl {
    public:
        void keypressed(char key);
        // void triggerAlarm();
        // void render();
        // void tick();
    private:
        friend class ProgramState;
        void changeProgramState(ProgramState* state);
        const ProgramState* ps_DisplayTemp;
        const ProgramState* ps_SettingMin;
        const ProgramState* ps_SettingMax;
        const ProgramState* ps_ToggleUnits; // TODO FIXME not needed; toggle could happen with one button press in any state
        const ProgramState* ps_DisplayOff;
        // void displayOn();
        // void displayOff();
    private:
        ProgramState* _progState;
        bool _alarmOn;
        // void renderTempRow();
        // void renderControlRow();
};
