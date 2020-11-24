#include <RH_ASK.h>

#define LM35_LOAD_PIN A7
#define LM35_UNITS LM35_FAHRENHEIT

#include <LM35.h>

#include "Debounced.h"

#define RF_BITRATE 2000
#define RF_PIN_RX 255
#define RF_PIN_TX 12
#define RF_PIN_PTT 255

#define DIP1 2
#define DIP2 3

#define DEBUG

static RH_ASK rf;
static LM35 temp;

static String id;

void setup() {
    analogReference(DEFAULT);
    Serial.begin(9600);
    Serial.println("started setup");
    pinMode(DIP1, INPUT);
    pinMode(DIP2, INPUT);
    if (!rf.init()) {
#ifdef DEBUG
    Serial.println(F("init RF module failed"));
#endif
    } else {
        rf.setModeTx();
    }
    // testdrawstyles();
    Debounced d1 = Debounced(DIP1);
    Debounced d2 = Debounced(DIP2);

    delay(2000);
    bool d1change = false;
    bool d2change = false;
    int start = millis();
    do {
        d1change = d1.read();
        d2change = d2.read();
    } while ((!d1change && !d2change) || (millis() - start) > 10000);
    Serial.println(d1.value());
    Serial.println(d2.value());
    byte tmpid = d1.value() + (d2.value() << 1);
    String hexId = String((tmpid & 0x00F0) >> 4, HEX);
    hexId = hexId + String((tmpid & 0x000F), HEX);
    id = hexId;
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
    String stemp = String(t, 3);
    String msg = id + stemp + "\0";

#ifdef DEBUG
    Serial.println(millis());
//     Serial.println(stemp);
//     Serial.println(msg);
#endif

    rf.send((uint8_t*)msg.c_str(), msg.length());
    rf.waitPacketSent();
#ifdef DEBUG
    Serial.print(millis());
    Serial.print("(");
    Serial.print(id);
    Serial.print("): sent \"");
    Serial.print(msg);
    Serial.println("\"");
#endif
    delay(1500);
}

// void testdrawstyles(void) {
//   display.clearDisplay();

//   display.setTextSize(1);             // Normal 1:1 pixel scale
//   display.setTextColor(SSD1306_WHITE);        // Draw white text
//   display.setCursor(0,0);             // Start at top-left corner
//   display.println(F("Hello, world!"));

//   display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
//   display.println(3.141592);

//   display.setTextSize(2);             // Draw 2X-scale text
//   display.setTextColor(SSD1306_WHITE);
//   display.print(F("0x"));
//   display.println(0xDEADBEEF, HEX);

//   display.display();
//   delay(2000);
// }
