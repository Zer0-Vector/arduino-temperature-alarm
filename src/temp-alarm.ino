// #include <Keypad.h>

#define LM35_LOAD_PIN A7
#define LM35_SAMPLE_SIZE 128
#include <LM35.h>
#include <SSD1306AsciiAvrI2c.h>
#include <EEPROM.h>

// #include <TempAlarmControl.h>

#define I2C_ADDRESS 0x3C
#define RST_PIN -1

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

#define PIN_SCK 13
#define PIN_MISO 12
#define PIN_MOSI 11
#define PIN_SS 10
#define PIN_SSIN 9
#define PIN_LOAD 8
#define PIN_INTERRUPT 3

#define DEBUG

#include <SPI.h>

LM35 temp(1100);

const unsigned long startTime = millis();

// TODO I could swap row/column wires to make this array "right-side up"
const char hexaKeys[KEYPAD_ROWS*KEYPAD_COLS] = {
    '1', '4', '7', '*',
    '2', '5', '8', '0',
    '3', '6', '9', '#',
    'A', 'B', 'C', 'D'
};

SSD1306AsciiAvrI2c oled;

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

// TempAlarmControl* control;

volatile bool keyDown = false;

void setup() {
    analogReference(EXTERNAL);
    Serial.begin(115200);
    Serial.println(F("started setup"));
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    delay(2000);
    oled.set2X();
    Serial.println(F("oled initalized"));
    
    pinMode(PIN_SS, OUTPUT); // Used to enable 74HC595
    digitalWrite(PIN_SS, HIGH); // init diabled

    pinMode(PIN_SSIN, OUTPUT); // Used to enable 74HC165 clock
    digitalWrite(PIN_SSIN, HIGH); // init disabled

    pinMode(PIN_LOAD, OUTPUT); // Used to transfer keypad data into 74HC165
    digitalWrite(PIN_LOAD, HIGH); // init disabled
    Serial.println(F("pins initialized"));
    
    SPI.begin();

    Serial.println(F("spi initalized"));
    pinMode(PIN_INTERRUPT, INPUT);
    
    attachInterrupt(1, keyPressed, HIGH);
    Serial.println(F("interrupt initalized"));
    write595(0b11111111);
    Serial.println(F("595 initalized"));
    Serial.println(F("Setup done!"));
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

// byte currentKey = NULL;
unsigned long keydownTime = 0;
#define KEYDOWN_DECAY 250

// interrupt callback
void keyPressed() {
    keyDown = true;
}

uint8_t processKey() {
    static uint8_t lastKey;
    uint8_t currentKey = readKey();

    if (currentKey) {
        lastKey = currentKey;
    } else {
        lastKey = 0;
    }
    return currentKey;
}

uint8_t readKey() {
    // TODO
    uint8_t z = 0;
    write595(0b11111111);
    z = read165();
    if (z == 0) {
        return 0;
    }

    uint8_t key = 1;
    for (uint8_t i = 1; i <= 0b1000; i <<= 1) {
        write595(i);
        z = read165();

        if (z != 0) {
            break;
        }
        key += 4;
    }

    if (z == 0) {
        return 0;
    }

    for (uint8_t col = 0; col < 4; col++) {
        if ((1<<col) == z) {
            return key + col;
        }
    }

    return 0;
}

void write595(uint8_t b) {
    digitalWrite(PIN_SS, LOW); // enable 595
    asm("nop\n nop\n"); // short delay
    SPI.transfer(b); // send byte
    digitalWrite(PIN_SS, HIGH); // disable 595
}

uint8_t read165() {
    digitalWrite(PIN_LOAD, LOW); // pulse load
    asm("nop\n nop\n");
    digitalWrite(PIN_LOAD, HIGH);
    digitalWrite(PIN_SSIN, LOW); // enable 165 clk
    asm("nop\n nop\n");
    uint8_t b = SPI.transfer(0);
    digitalWrite(PIN_SSIN, HIGH);
    return b;
}

void loop() {
    oled.setFont(TEMP_FONT);
    uint8_t rows = oled.fontRows();
    // currentKey = kp.getKey();
    if (keyDown) {
        uint8_t currentKey = processKey();
        keyDown = false;
        if (currentKey) {
            // Serial.print(F("Key: "));
            // Serial.print(hexaKeys[currentKey-1]);
            // Serial.print(F(" ("));
            // Serial.print(currentKey, HEX);
            // Serial.println(F(")"));
            keydownTime = elapsed();
            oled.setFont(ALPHA_FONT);
            oled.setCursor(LEFT_PADDING, rows);
            oled.print(hexaKeys[currentKey-1]);
        }
    } else if (elapsed() - keydownTime > KEYDOWN_DECAY) {
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

    Serial.print(F("update display temp: "));
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
