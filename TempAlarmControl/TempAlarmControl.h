#pragma once

#include <SSD1306Ascii.h>
#include <LM35.h>

#define MAX_UNSET 127
#define MIN_UNSET -128

#define MIN_MIN_TEMP_C 2
#define MAX_MAX_TEMP_C 126

#define CONFIG_DATA_ID 65123

#define TEMP_FONT TimesNewRoman16_bold
#define ALPHA_FONT Callibri15
#define UTF_FONT utf8font10x16

#define LEFT_PADDING 3

class ProgramState;

struct TempAlarmConfig {
    int8_t minThreshold = MIN_UNSET;
    int8_t maxThreshold = MAX_UNSET;
    bool convertToF = true;
    const uint16_t ID = CONFIG_DATA_ID;
};

class TempAlarmControl {
    public:
        TempAlarmControl();
        void keypressed(char key);
        void render(SSD1306Ascii* oled, LM35* lm35);
        void tick(LM35* lm35);
    protected:
        void changeState(ProgramState* state);
        void setDisplayOn(bool displayOn);
        void saveConfigToDisk();
        void suppressAlarmSound();
        friend class ProgramState;
    private:
        ProgramState* _state;
        bool _invertDisplay;
        bool _alarmSoundOn;
        bool _displayOn;
        TempAlarmConfig* _config;
        void _renderTempRow(SSD1306Ascii* oled, LM35* lm35);
        bool _checkTemp(LM35* lm35);
        void _alarmOn();
        void _alarmOff();
        void _playSound();
        void _toggleUnits();
};
