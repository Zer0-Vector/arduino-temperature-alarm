
#include "MAX7219.h"


MAX7219::MAX7219(uint32_t clock) {
    _settings = new SPISettings(clock, MSBFIRST, SPI_MODE0);
}

MAX7219::~MAX7219() {
    delete _settings;
}

void MAX7219::begin() {
    SPI.begin();
    delay(50);
    sendData(SHUTDOWN_OPERATION);
    delay(50);
    sendData(FULL_INTENSITY);
    sendData(DECODE_FOR_DIGITS0_3);
    sendData(USE_ONLY_4DIGITS);
    sendData(NORMAL_OPERATION);
    delay(50);
}

void MAX7219::end() {
    sendData(SHUTDOWN_OPERATION);
    SPI.end();
}

bool MAX7219::writeNumbers(const char * data) {
    size_t len = strlen(data);
    if (len > 5) return false; // only 4 digits plus decimal are allowed.

    uint16_t packets[4];
    for (int i = 0; i < 4; i++) {
        packets[i] = 0;
    }
    int p=3;
    int i;
    for (i = len-1; i >= 0; i--) {
        char c = data[i];
        switch (c) {
            case '.':
                packets[p] |= DP_MASK;
                break;

            case '-':
                packets[p--] |= MINUS;
                break;

            default:
                if (c >= '0' || c <= '9') {
                    packets[p--] |= (c - '0');

#ifdef DEBUG
                    Serial.print("digit")
                    Serial.print(p);
                    Serial.print(": ");
                    Serial.println(packets[p], HEX);
#endif
                }
        }
    }

    // pad front with blanks
    Serial.println(p);
    for (; p >=0; p--) {
        packets[p] |= BLANK;
    }

    // add digit mask and send
    Serial.print("sending spi data: ");
    for(p=0; p<4; p++) {
        packets[p] |= ((p+1) << 8);
        sendData(packets[p]);
        Serial.print(packets[p], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void MAX7219::sendData(uint16_t packet) {
    SPI.beginTransaction(*_settings);
    digitalWrite(CS_PIN, LOW);
    SPI.transfer16(packet);
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}
