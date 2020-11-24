#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>

// #include <LiquidCrystal.h>
#define LM35_UNITS LM35_FAHRENHEIT
#include <LM35.h>
#include <RH_ASK.h>
#include "temp-alarm-base.h"

#define NUMBER_OF_SAMPLES 128
#define SAMPLING_DELAY 15

#define LCD_PIN_RS 12
#define LCD_PIN_E 11
#define LCD_PIN_RW 10
#define LCD_PIN_D0 2
#define LCD_PIN_D1 3
#define LCD_PIN_D2 4
#define LCD_PIN_D3 5

#define RF_BITRATE 2000
#define RF_PIN_RX 11
#define RF_PIN_TX 255
#define RF_PIN_PTT 255

#define DEBUG

static RH_ASK rf;
static SSD1306AsciiAvrI2c oled;

#define I2C_ADDRESS 0X3C

#ifdef DEBUG
static uint16_t lastTime = 0L;
#endif

static LM35 temp;
static uint8_t labelWidth;

void setup() {
  analogReference(INTERNAL);
#ifdef DEBUG
  Serial.begin(9600);
#endif
  if (!rf.init()) {
#ifdef DEBUG
    Serial.println(F("init RF module failed!"));
#endif
  } else {
    delay(1500);
    rf.setModeRx();
#ifdef DEBUG
    Serial.println(F("RF module init done"));
#endif
  }

  Serial.println("starting display");
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(font8x8);
  oled.clear();
  oled.println();
  uint8_t currentWidth = 0;
  const char* label = "Base:";
  oled.print(label);
  labelWidth = oled.strWidth(label);
  currentWidth += labelWidth;
  oled.setFont(Adafruit5x7);
  oled.set2X();
  const char* blankTemp = "    ";
  currentWidth += oled.strWidth(blankTemp);
  oled.print(blankTemp);
  oled.setFont(font8x8);
  oled.set1X();
  oled.setCursor(currentWidth+2, 0);
  oled.print("o");
  oled.setFont(Adafruit5x7);
  oled.set2X();
  oled.println("F");
}



#define TEMP_TIMEOUT 2500

RemoteTemp parseTemp(char* msg) {
  RemoteTemp t;
  t.lastMessageTime = millis();
  String smsg = String(msg);
  t.id = (byte)smsg.substring(0,2).toInt();
  t.value = smsg.substring(2).toDouble();
  return t;
}

bool isTempExpired(RemoteTemp t) {
  return millis() - t.lastMessageTime > TEMP_TIMEOUT;
}

static RemoteTemp temps[8];

void loop() {
  // int reading = analogRead(A0); // [0, 1024)
  // sumSq += LM35_CONVERSION_CONSTANT*LM35_CONVERSION_CONSTANT * reading*reading;

  uint8_t buf[10];
  uint8_t buflen = sizeof(buf);
  if (rf.recv(buf, &buflen)) {
    Serial.print("Received message: ");
    Serial.println((char*)buf);
    RemoteTemp tmsg = parseTemp((char*)buf);
    if (tmsg.id >= 0) {
      temps[tmsg.id] = tmsg;
    }
  }

#ifdef DEBUG
  uint16_t thisTime = millis();
  uint16_t elapsed = thisTime - lastTime;
  if (elapsed > 2500) {
    Serial.print(thisTime);
    Serial.println(": waiting");
    lastTime = thisTime;
  }
#endif
  if (!temp.sampleTemp()) {
    return;
  }
  // if (++sampleIndex < NUMBER_OF_SAMPLES) {
  //   delay(SAMPLING_DELAY);
  //   return;
  // }
  // sampleIndex = 0;
  // double val = toF(sqrt(sumSq / NUMBER_OF_SAMPLES));
  // sumSq = 0.0;
#ifdef DEBUG
 Serial.print(F("BASE temp: "));
 Serial.println(temp.tempAsF(), 3);
 Serial.println();
 
  oled.setCursor(labelWidth, 0);
  oled.println(temp.tempAsF(), 1);

 for (int i = 0; i < 8; i++) {
   RemoteTemp t = temps[i];
   if (isTempExpired(t)) {
     t.id = -1;
   }
   if (t.id >= 0) {
     oled.setFont(font8x8);
     oled.setCursor(0, 25);
     oled.set1X();
     oled.print("Sen");
     oled.print(t.id);
     oled.print(":");
     oled.setCursor(labelWidth, 9);
     oled.setFont(Adafruit5x7);
     oled.set2X();
     oled.println(t.value, 1);
     Serial.print(F("Sensor "));
     Serial.print(t.id);
     Serial.print(F(": "));
     Serial.println(t.value, 3);
   }
 }
#endif
  delay(SAMPLING_DELAY);
}
