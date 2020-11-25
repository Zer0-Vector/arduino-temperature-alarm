#include <RH_ASK.h>

#define LM35_LOAD_PIN A7

#include <LM35.h>

#include "SSD1306AsciiAvrI2c.h"
#include "Debounced.h"

#define I2C_ADDRESS 0x3C
#define RST_PIN -1

#define RF_BITRATE 2000
#define RF_PIN_RX 255
#define RF_PIN_TX 03
#define RF_PIN_PTT 255

#define DEBUG

static RH_ASK rf;
static LM35 temp;

static uint8_t id = 0; // TODO make configurable

static const unsigned long startTime = millis();
static unsigned long elapsed = 0;

SSD1306AsciiAvrI2c oled;

uint8_t labelRowHeight;

void setup() {
    analogReference(DEFAULT);
    Serial.begin(9600);
    Serial.println("started setup");
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    if (!rf.init()) {
#ifdef DEBUG
    Serial.println(F("init RF module failed"));
#endif
    } else {
        rf.setModeTx();
    }

    delay(2000);
#ifdef DEBUG 
    Serial.print(F("Started id="));
    Serial.println(id);
#endif
    oled.setFont(Verdana12);
    oled.set2X();
    oled.clear();
    oled.print("Sensor ");
    oled.println(id);
    labelRowHeight = oled.fontHeight();
}

#define ALPHA_FONT fixed_bold10x15
#define TEMP_DISPLAY_UPDATE_DELAY 500
#define BROADCAST_DELAY 1000
#define LOOP_DELAY 100

unsigned long lastPrintTime = 0;
unsigned long lastBroadcast = 0;

void loop() {
    elapsed = millis() - startTime;
    if (!temp.sampleTemp()) {
        return;
    }

    bool shouldUpdateDisplay = elapsed - lastPrintTime > TEMP_DISPLAY_UPDATE_DELAY;
    bool shouldBroadcast = elapsed - lastBroadcast > BROADCAST_DELAY;
    if (!shouldUpdateDisplay && !shouldBroadcast) {
        return; // nothing to do
    }

    double t = temp.tempAsF();
    Serial.println(t,3);
    char *msg = (char*)malloc(10*sizeof(char));
    char *cstemp = (char*)malloc(8*sizeof(char));
    uint8_t mag = (uint8_t)log10(abs(t));
    dtostrf(t, 3 + mag, 1, cstemp);
    Serial.print("Measured temp: ");
    Serial.println(cstemp);

    if (shouldUpdateDisplay) {
        oled.setFont(ALPHA_FONT);
        oled.set2X();
        oled.clear(0, 128, labelRowHeight, oled.fontHeight() + labelRowHeight + 1);
        oled.print(cstemp);
        oled.setFont(utf8font10x16);
        oled.print('\260');
        oled.setFont(ALPHA_FONT);
        oled.print('F');
        lastPrintTime = elapsed;
    }
    snprintf(msg, 9, "%02d%s", id, cstemp);
    free(cstemp);
    if (shouldBroadcast) {
        rf.send((uint8_t*)msg, strlen(msg));
        rf.waitPacketSent();
#ifdef DEBUG
        Serial.print(elapsed);
        Serial.print("(");
        Serial.print(id);
        Serial.print("): sent \"");
        Serial.print(msg);
        Serial.println("\"");
#endif
    }
    free(msg);

    delay(LOOP_DELAY);
}
