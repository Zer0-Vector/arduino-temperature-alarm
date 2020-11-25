#include <RH_ASK.h>

#define LM35_LOAD_PIN A7

#include <LM35.h>

#include "Debounced.h"

#define RF_BITRATE 2000
#define RF_PIN_RX 255
#define RF_PIN_TX 12
#define RF_PIN_PTT 255

#define DEBUG

static RH_ASK rf;
static LM35 temp;

static uint8_t id = 0; // TODO make configurable

void setup() {
    analogReference(DEFAULT);
    Serial.begin(9600);
    Serial.println("started setup");
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
}

void loop() {
    if (!temp.sampleTemp()) {
        return;
    }

    double t = temp.tempAsF();
    Serial.println(t,3);
    char *msg = (char*)malloc(10*sizeof(char));
    char *cstemp = (char*)malloc(8*sizeof(char));
    uint8_t mag = (uint8_t)log10(abs(t));
    Serial.println(mag);
    dtostrf(t, 3 + mag, 1, cstemp);
    Serial.println(cstemp);
    snprintf(msg, 9, "%02d%s", id, cstemp);
    free(cstemp);
    

    rf.send((uint8_t*)msg, strlen(msg));
    rf.waitPacketSent();
#ifdef DEBUG
    Serial.print(millis());
    Serial.print("(");
    Serial.print(id);
    Serial.print("): sent \"");
    Serial.print(msg);
    Serial.println("\"");
#endif
    free(msg);
    delay(2500);
}
