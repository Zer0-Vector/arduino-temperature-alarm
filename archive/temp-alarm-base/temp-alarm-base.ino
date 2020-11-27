#include <PerfTools.h>

#define LM35_UNITS LM35_FAHRENHEIT
#include <LM35.h>

#include <RH_ASK.h>
#include "temp-alarm-base.h"

#define NUMBER_OF_SAMPLES 128
#define SAMPLING_DELAY 50

#define RF_BITRATE 2000
#define RF_PIN_RX 11
#define RF_PIN_TX 255
#define RF_PIN_PTT 255

#define DISPLAY_PERIOD 6000

#define DEBUG

#define I2C_ADDRESS 0X27
#define LCD_COLS 20
#define LCD_ROWS 4

#define ERROR_LED_PIN 2
#define OPERATING_LED_PIN 4

RH_ASK rf;
MyLCDIC2 lcd(I2C_ADDRESS, LCD_COLS, LCD_ROWS);
LM35 temp(3300);
RemoteTemp temps[8];

int8_t displayIndex = -1;
uint8_t remoteCount = 0;
unsigned long lastDisplayTime = 0;

void setup() {
  analogReference(DEFAULT);
#ifdef DEBUG
  Serial.begin(9600);
#endif
  pinMode(ERROR_LED_PIN, OUTPUT);
  pinMode(OPERATING_LED_PIN, OUTPUT);
  digitalWrite(ERROR_LED_PIN, LOW);
  digitalWrite(OPERATING_LED_PIN, LOW);
  if (!rf.init()) {
#ifdef DEBUG
    Serial.println(F("init RF module failed!"));
    digitalWrite(ERROR_LED_PIN, HIGH);
    for(;;);
#endif
  } else {
    delay(1500);
    rf.setModeRx();
#ifdef DEBUG
    Serial.println(F("RF module init done"));
#endif
  }

  Serial.println(F("starting display"));
  if (!lcd.begin()) {
    Serial.println(F("lcd failed to start"));
    digitalWrite(ERROR_LED_PIN, HIGH);
    for (;;); // TODO show error somehow; error led?
  }
  digitalWrite(OPERATING_LED_PIN, HIGH);
  lcd.setCursor(false);
  lcd.setBlink(false);
  lcd.home();
  delay(1500);
  // TODO would be cool in a ticker
  lcd.print("Temperature Alarm");
  delay(2000);
  lcd.clear();
  delay(1500);
  memchk("setup_end");
}

void loop() {
  uint8_t *buf = (uint8_t*)malloc(10*sizeof(uint8_t)); 
  uint8_t buflen = 10;
  if (rf.recv(buf, &buflen)) {
    Serial.print(elapsed());
    Serial.print(F(": Received message: "));
    buf[9] = '\0'; // just to be sure
    Serial.println((char*)buf);
    RemoteTemp tmsg = parseTemp((char*)buf);
    if (tmsg.id >= 0 && tmsg.id <= 7) {
      bool found = false;
      for (int i = 0; i < remoteCount; i++) {
        if (temps[i].id == tmsg.id) {
          Serial.print("Found id = ");
          Serial.println(tmsg.id);
          temps[i] = tmsg;
          found = true;
          break;
        }
      }
      if (!found) {
        Serial.print("New sensor. count=");

        Serial.println(remoteCount);
        temps[remoteCount++] = tmsg;
      }
    }
  }
  free(buf);
  
  if (!temp.sampleTemp()) {
    return;
  }
  double baseTemp = temp.tempAsF();
  double sum = baseTemp;

#ifdef DEBUG
 Serial.print(F("BASE temp: "));
 Serial.println(baseTemp, 3);
#endif

  for (int i = 0; i < 8; i++) {
    RemoteTemp t = temps[i];
    if (t.id <= NULL_TEMP_VALUE) {
      continue;
    }
    sum += t.value;
    if (isTempExpired(t)) {
      remoteCount--;
      displayIndex--;
      RemoteTemp t2;
      temps[i] = t2;
      // shift values left
      int lastEmpty = i;
      for (int j = i + 1; j < 8; j++) {
        if (temps[j].id > -999) {
          RemoteTemp tmp = temps[lastEmpty];
          temps[lastEmpty] = temps[j];
          temps[j] = tmp;
          lastEmpty++;
        }
      }
      lcd.clear();
      memchk("after_expire");
    }
  }

  lcd.home();
  if (remoteCount > 0) {
    printAvgAndCycle(sum);
  } else {
    lcd.print("Base: ");
    lcd.print(baseTemp, 1);
    printUnitSuffix();
  }



#ifdef DEBUG
  for (int i = 0; i < 8; i++) {
    RemoteTemp t = temps[i];
    if (t.id <= -999) continue;
    Serial.print(F("Sensor "));
    Serial.print(t.id);
    Serial.print(F(": "));
    Serial.println(t.value, 3);
  }
#endif

  delay(SAMPLING_DELAY);
}

void printAvgAndCycle(double sum) {
  lcd.clear();
  lcd.print("Avg:  ");
  lcd.print(sum / (remoteCount + 1), 1);
  printUnitSuffix();

  lcd.clear();
  if (displayIndex == -1) {
    lcd.print("Base: ");
    lcd.print(temp.tempAsF(), 1);
  } else {
    RemoteTemp t = temps[displayIndex];
    lcd.print("Sen");
    lcd.print(t.id);
    lcd.print(": ");
    lcd.print(t.value, 1);
  }
  printUnitSuffix();
  unsigned long period = elapsed() - lastDisplayTime;
  bool triggerNext = period > DISPLAY_PERIOD;
  Serial.print("period=");
  Serial.println(period);
  if (triggerNext) {
    lastDisplayTime = elapsed();
    if (++displayIndex >= remoteCount) {
      displayIndex = -1;
    }
    Serial.print("displayIndex: ");
    Serial.println(displayIndex);
  }
}

void printUnitSuffix() {
  lcd.println(" degF");
}