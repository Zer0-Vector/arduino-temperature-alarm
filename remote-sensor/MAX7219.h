#ifndef _MAX7219_H_
#define _MAX7219_H_

#include <SPI.h>

#define USE_ONLY_4DIGITS 0x0B03
#define DECODE_FOR_DIGITS0_3 0x090F
#define FULL_INTENSITY 0x0A0F
#define NORMAL_OPERATION 0x0C01
#define SHUTDOWN_OPERATION 0x0C00

#define DP_MASK 0x0080

#define BLANK 0x0F
#define MINUS 0x0A

#ifndef CS_PIN
#define CS_PIN 10
#endif

class MAX7219 {
    public:
        MAX7219(uint32_t clock = 4000000);
        ~MAX7219();
        void begin();
        void end();
        void sendData(uint16_t packet);
        bool writeNumbers(const char * data);
    private:
        SPISettings* _settings;
};

#endif // ndef _MAX7219_H_
