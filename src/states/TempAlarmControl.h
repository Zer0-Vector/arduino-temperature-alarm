#pragma once

#include <SSD1306Ascii.h>
#include <LM35.h>

#define MAX_UNSET 127
#define MIN_UNSET -128

#define MIN_MIN_TEMP_C 2
#define MAX_MAX_TEMP_C 126

#define CONFIG_DATA_ID 65123

#include "TempAlarmStates.h"

struct TempAlarmConfig {
    TempAlarmConfig() : 
        minThreshold(MIN_UNSET), maxThreshold(MAX_UNSET), convertToF(true) {}

    int8_t minThreshold;
    int8_t maxThreshold;
    bool convertToF;
    const uint16_t ID = CONFIG_DATA_ID;
};

class TempAlarmControl {
    public:
        TempAlarmControl();
        ~TempAlarmControl();
        void keypressed(char key);
        void triggerAlarm();
        void render(SSD1306Ascii* oled, LM35* lm35);
        void tick();
    private:
        friend class ProgramState;

        void changeState(ProgramState* state);
        void setDisplayOn(bool displayOn);
        void toggleUnits();
        void saveConfigToDisk();
    private:
        ProgramState* _state;
        bool _alarmOn;
        bool _displayOn;
        TempAlarmConfig _config;
        void _renderTempRow(SSD1306Ascii* oled, LM35* lm35);
};
