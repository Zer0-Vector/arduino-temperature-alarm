#include <RH_ASK.h>
#include <LM35.h>

#define RF_BITRATE 2000
#define RF_PIN_RX 255
#define RF_PIN_TX 12
#define RF_PIN_PTT 255

#define DIP1 2
#define DIP2 3

#define DEBUG

static RH_ASK rf;
static LM35 temp(A7);
static byte id = 1;

void setup() {
#ifdef DEBUG
    analogReference(INTERNAL);
    Serial.begin(9600);
#endif
    if (!rf.init()) {
#ifdef DEBUG
        Serial.println("init RF module failed");
#endif
    } else {
        rf.setModeTx();
    }
    
}

void loop() {
    if (!temp.sampleTemp()) {
        return;
    }
    double t = temp.currentTemp();
    Serial.println(t, 2);
    String stemp = String(t, 1);
    char* msg = "01xxxxxx";
    for (unsigned int i = 0; i < stemp.length(); i++) {
        char c = stemp.charAt(i);
        msg[2+i] = c;
    }
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
    delay(2500);
}