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

SSD1306AsciiAvrI2c oled;

uint8_t labelRowHeight;
size_t labelWidth;

#define LABEL_FONT Verdana12
#define ALPHA_FONT utf8font10x16
#define TEMP_DISPLAY_UPDATE_DELAY 1000
#define BROADCAST_DELAY 400
#define LOOP_DELAY 100

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
    oled.setFont(LABEL_FONT);
    oled.set2X();
    oled.clear();
    char * label = (char*)malloc(10*sizeof(char));
    snprintf(label, 9, "Sensor %d", id);
    oled.println(label);
    labelWidth = oled.strWidth(label)+(oled.fontWidth()/2);
    labelRowHeight = oled.fontRows();
    Serial.println(labelRowHeight);
}

unsigned long lastPrintTime = 0;
unsigned long lastBroadcast = 0;
char signalIcon = '|';
bool hasValidTemp = false;

unsigned long elapsed() {
    return millis() - startTime;
}

void drawSpinner(bool invertIt) {
    oled.setFont(LABEL_FONT);
    oled.set2X();
    oled.setCursor(labelWidth , 0);
    // invert icon if broadcasting
    oled.setInvertMode(invertIt);
    oled.print(signalIcon);
    
    // rotate signal icon
    switch (signalIcon) {
        case '|':
            signalIcon = '\\';
            break;
        case '\\':
            signalIcon = '-';
            break;
        case '-':
            signalIcon = '/';
            break;
        case '/':
            signalIcon = '|';
            break;
    }
}

void loop() {
    unsigned long sinceLastBroadcast = elapsed() - lastBroadcast;
    bool shouldBroadcast = (sinceLastBroadcast > BROADCAST_DELAY);
    if(!temp.sampleTemp()) {
        drawSpinner(false);
        delay(LOOP_DELAY / 4);
        return;
    }
    unsigned long sinceLastPrint = elapsed() - lastPrintTime;
    bool shouldUpdateDisplay = (sinceLastPrint > TEMP_DISPLAY_UPDATE_DELAY);
    drawSpinner(shouldBroadcast);
    if (!(shouldUpdateDisplay || shouldBroadcast)) {
        delay(LOOP_DELAY / 4);
        return; // nothing to do
    }
    
    double t = temp.tempAsF();
    Serial.println(t,3);
    char *msg = (char*)malloc(10*sizeof(char));
    char *cstemp = (char*)malloc(8*sizeof(char));
    uint8_t mag = (uint8_t)log10(abs(t));
    dtostrf(t, 3 + mag, 1, cstemp);

    if (shouldUpdateDisplay) {
        Serial.print("update display temp: ");
        Serial.println(cstemp);

        oled.clear(0, 128, labelRowHeight, oled.fontRows() + labelRowHeight + 1);
        oled.setInvertMode(false);
        oled.setFont(ALPHA_FONT);
        oled.set2X();
        oled.setCursor(0,labelRowHeight);
        oled.print(cstemp);
        oled.print('\260');
        oled.print('F');
        lastPrintTime = elapsed();
    }
    snprintf(msg, 9, "%02d%s", id, cstemp);
    free(cstemp);
    if (shouldBroadcast) {
        rf.send((uint8_t*)msg, strlen(msg));
        rf.waitPacketSent();
        lastBroadcast = elapsed();
#ifdef DEBUG
        Serial.print(elapsed());
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
