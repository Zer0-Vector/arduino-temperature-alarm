#include <LCDIC2.h>

LCDIC2 lcd(0x27, 20, 4);

#define RED_LED_PIN 2
#define GREEN_LEN_PIN 4

void setup() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LEN_PIN, OUTPUT);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LEN_PIN, LOW);
    if (lcd.begin()) {
        digitalWrite(GREEN_LEN_PIN, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("Hello, World!");
        lcd.setCursor(false);
        lcd.setShift(true);
    } else {
        digitalWrite(RED_LED_PIN, HIGH);
        for(;;); // THERE WAS AN ERROR!
    }
}

void loop() {
    lcd.moveRight();
    delay(750);
    // for (uint8_t i = 0; i <= 15; i++) {
    //     if (i == 0) {
    //         lcd.setCursor(0,2);
    //         lcd.print(' ');
    //     } else {
    //         lcd.setCursor(i-1,1);
    //         lcd.print(' ');
    //     }
    //     lcd.setCursor(i, 1);
    //     String* txt = new String(i, HEX);
    //     lcd.print(*txt);
    //     delete txt;
    //     delay(250);
    // }
    // for (uint8_t i = 0; i <= 15; i++) {
    //     if (i == 0) {
    //         lcd.setCursor(15,1);
    //         lcd.print(' ');
    //     } else {
    //         lcd.setCursor(15-i+1,2);
    //         lcd.print(' ');
    //     }
    //     lcd.setCursor(15-i, 2);
    //     String* txt = new String(i, HEX);
    //     lcd.print(*txt);
    //     delete txt;
    //     delay(250);
    // }
}