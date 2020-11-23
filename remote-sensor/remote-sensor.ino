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

#define DEFAULT_DEBOUNCE_DELAY 50
class Debounced {
    public:
        Debounced(uint8_t dpin, unsigned long delay = DEFAULT_DEBOUNCE_DELAY);
        /**
         * returns true if value has changed
         */
        bool read();
        int value();
        void setValue(int value);
    private:
        uint8_t _pin;
        unsigned long _delay;
        int _value;
        int _lastValue;
        int _lastTime;
};

Debounced::Debounced(uint8_t dpin, unsigned long delay)
: _pin(dpin), _delay(delay), _value(-1), _lastTime(0)
{}

bool Debounced::read() {
    int reading = digitalRead(this->_pin);
    if (reading != this->_lastValue) {
        this->_lastTime = millis();
    }
    this->_lastValue = reading;
    if (millis() - this->_lastTime > this->_delay) {
        this->_value = reading;
        return true;
    }
    return false;
}

int Debounced::value() {
    return this->_value;
}
void Debounced::setValue(int value) {
    this->_value = value;
}



static Debounced d1State(DIP1);
static Debounced d2State(DIP2);

void setup() {
#ifdef DEBUG
    analogReference(INTERNAL);
    pinMode(DIP1, INPUT);
    pinMode(DIP2, INPUT);
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
    
    d1State.read();
    d2State.read();

    id = d1State.value() + (d2State.value() << 1);

    if (!temp.sampleTemp()) {
        return;
    }

    Serial.print("d1=");
    Serial.print(d1State.value());
    Serial.print(", d2=");
    Serial.println(d2State.value());

    double t = temp.currentTemp();
    String hexId = String((id & 0x00F0) >> 4, HEX);
    hexId = hexId + String((id & 0x000F), HEX);
    Serial.println(hexId);
    String stemp = String(t, 3);
    Serial.println(stemp);
    String msg = hexId + stemp;
    // msg[0] = hexId.charAt(0);
    // msg[1] = hexId.charAt(1);
    // for (unsigned int i = 0; i < stemp.length(); i++) {
    //     char c = stemp.charAt(i);
    //     msg[2+i] = c;
    // }
    
    rf.send((uint8_t*)msg.c_str(), msg.length());
    rf.waitPacketSent();
#ifdef DEBUG
    Serial.print(millis());
    Serial.print("(");
    Serial.print(id);
    Serial.print("): sent \"");
    Serial.print(msg);
    Serial.println("\"");
    Serial.println(msg.c_str());
#endif
    delay(1500);
}