#include <Keypad.h>

#define LM35_LOAD_PIN A7
#define LM35_SAMPLE_SIZE 128
#include <LM35.h>
#include <SSD1306AsciiAvrI2c.h>
#include <EEPROM.h>

#include <TempAlarmControl.h>

#define I2C_ADDRESS 0x3C
#define RST_PIN -1

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

#define DEBUG

static LM35 temp(1500);

const unsigned long startTime = millis();

const char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' },
};

byte rowPins[KEYPAD_ROWS] = { 12, 11, 10, 9 };
byte colPins[KEYPAD_COLS] = { 8, 7, 6, 5 };

Keypad kp = Keypad((char*)hexaKeys, rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
SSD1306AsciiAvrI2c oled;

// TODO 1. store alarm values
// TODO 2. make alarm sound/flash light
// TOOD 4. use keypad to configure alarm values
// TODO 5. use keypad to switch from F to C
// TODO 6. use keypad to calibrate

#define TEMP_FONT TimesNewRoman16_bold
#define ALPHA_FONT Callibri15
#define UTF_FONT utf8font10x16
#define TEMP_DISPLAY_UPDATE_DELAY 2000
#define LOOP_DELAY 100

/**
 * A=set max
 * B=set min
 * C=C/F 
 * D=cancel/return to normal state/silence alarm
 * #=.
 * *=display on/off (display turns off after 5 second delay)
 */

TempAlarmControl* control;

void setup() {
    analogReference(DEFAULT);
    Serial.begin(9600);
    Serial.println("started setup");
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    delay(2000);
    oled.set2X();

    control = new TempAlarmControl();
}

unsigned long lastPrintTime = 0;
unsigned long lastBroadcast = 0;
char signalIcon = '|';
bool hasValidTemp = false;
uint8_t lastMag = 0;

unsigned long elapsed() {
    return millis() - startTime;
}

#define LEFT_PADDING 3

bool keydown = false;
char currentKey = NULL;
unsigned long keydownTime = 0;
#define KEYDOWN_DECAY 250

void loop() {
    oled.setFont(TEMP_FONT);
    uint8_t rows = oled.fontRows();
    currentKey = kp.getKey();
    if (currentKey) {
        keydown = true;
        keydownTime = elapsed();
        oled.setFont(ALPHA_FONT);
        Serial.println(currentKey);
        oled.setCursor(LEFT_PADDING, rows);
        oled.print(currentKey);
    } else if (keydown && (elapsed() - keydownTime) > KEYDOWN_DECAY) {
        keydown = false;
        oled.clear(0,128,rows,2*rows);
    }

    if(!temp.sampleTemp()) {
        return;
    }

    double t = temp.tempAsF();
    Serial.println(t,3);
    char *cstemp = (char*)malloc(8*sizeof(char));
    uint8_t mag = (uint8_t)log10(abs(t));
    dtostrf(t, 3 + mag, 1, cstemp);

    Serial.print("update display temp: ");
    Serial.println(cstemp);
    if (mag != lastMag) {
        oled.clear();
    }
    lastMag = mag;

    oled.setFont(TEMP_FONT);
    oled.setCursor(LEFT_PADDING, 0);
    oled.print(cstemp);
    oled.setFont(UTF_FONT);
    oled.print('\260');
    oled.setFont(TEMP_FONT);
    oled.println('F');
    free(cstemp);

    delay(LOOP_DELAY);
}
