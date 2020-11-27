#include <SSD1306AsciiAvrI2c.h>

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

static RH_ASK rf;
static SSD1306AsciiAvrI2c oled;

#define I2C_ADDRESS 0X3C

static LM35 temp;
static RemoteTemp temps[8];
static int8_t displayIndex = -1;
static uint8_t remoteCount = 0;
static unsigned long lastDisplayTime = 0;
static const unsigned long startTime = millis();
static unsigned long elapsed = 0;

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

  Serial.println(F("starting display"));
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Callibri15);
  oled.clear();
  oled.set2X();
  // TODO would be cool in a ticker
  oled.setFont(Callibri11);
  oled.println(F("Temperature"));
  oled.setFont(Callibri15);
  oled.println(F("Alarm"));
  delay(3000);
  oled.clear();
  mem("setup_end");
}

void mem(char* label) {
  Serial.print(F("@"));
  Serial.print(label);
  Serial.print(F(" mem: "));
  Serial.println(freeMemory());
}

RemoteTemp parseTemp(char* msg) {
  RemoteTemp t;
  t.lastMessageTime = elapsed;
  
  char cid[3];
  strncpy(cid, msg, 2);
  cid[2] = '\0';
  t.id = atoi(cid);

  char cval[7];
  memcpy(cval, &msg[2], 6);
  cval[6] = '\0';
  t.value = atof(cval);
  return t;
}

bool isTempExpired(RemoteTemp t) {
  return elapsed - t.lastMessageTime > TEMP_TIMEOUT;
}

void loop() {
  elapsed = millis() - startTime;
  uint8_t *buf = (uint8_t*)malloc(10*sizeof(uint8_t)); 
  uint8_t buflen = 10;
  if (rf.recv(buf, &buflen)) {
    Serial.print(elapsed);
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
      oled.clear();
      mem("after_expire");
    }
  }

  oled.home();
  oled.setFont(_ALPHANUMERIC_FONT);
  oled.set1X();
  if (remoteCount > 0) {
    printAvgAndCycle(sum);
  } else {
    oled.print(F("Base: "));
    oled.set2X();
    oled.print(baseTemp, 1);
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
  oled.set2X();
  uint8_t rows = oled.fontRows();
  oled.clear(0,128,0,rows);
  oled.set1X();
  oled.print(F("Avg:  "));
  oled.set2X();
  oled.print(sum / (remoteCount + 1), 1);
  printUnitSuffix();

  oled.setFont(_ALPHANUMERIC_FONT);
  oled.set1X();
  oled.clear(0,128,rows, rows+rows-1);
  if (displayIndex == -1) {
    oled.print("Base: ");
    oled.set2X();
    oled.print(temp.tempAsF(), 1);
  } else {
    RemoteTemp t = temps[displayIndex];
    oled.print(F("Sen"));
    oled.print(t.id);
    oled.print(F(": "));
    oled.set2X();
    oled.print(t.value, 1);
  }
  printUnitSuffix();
  unsigned long period = elapsed - lastDisplayTime;
  bool triggerNext = period > DISPLAY_PERIOD;
  Serial.print("period=");
  Serial.println(period);
  if (triggerNext) {
    lastDisplayTime = elapsed;
    if (++displayIndex >= remoteCount) {
      displayIndex = -1;
    }
    Serial.print("displayIndex: ");
    Serial.println(displayIndex);
  }
}


void printUnitSuffix() {
  oled.setFont(_UNICODE_FONT);
  oled.print('\260');
  oled.setFont(_ALPHANUMERIC_FONT);
  oled.println('F');
}