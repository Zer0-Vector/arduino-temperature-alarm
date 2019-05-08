#include <SPI.h>

const unsigned int USE_ONLY_4DIGITS = 0x0B03;
const unsigned int DECODE_FOR_DIGITS0_3 = 0x090F;
const unsigned int FULL_INTENSITY = 0x0A0F;
const unsigned int NORMAL_OPERATION = 0x0C01;

#define LOAD 10

SPISettings settings(10000000, MSBFIRST, SPI_MODE0);

void setup() {
  delay(50);
  analogReference(INTERNAL);
  pinMode(LOAD, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  sendData(FULL_INTENSITY);
  sendData(DECODE_FOR_DIGITS0_3);
  sendData(USE_ONLY_4DIGITS);
  sendData(NORMAL_OPERATION);
  delay(50);
}

#define DELAY_IN_MS 100
#define DP_MASK 0x0080

unsigned int uptime = 0;
int lastRead = 0;

void loop() {
  int reading = analogRead(A0);
  
  float voltage = reading * (INTERNAL / 1023.0);

  float tempC = reading / 9.3090909090909090909090909090909;
  float tempF = toF(tempC);
  if (uptime % 1000 == 0) {
    Serial.print("V=");
    Serial.print(voltage, 5);
    Serial.print(" V; T_c=");
    Serial.print(tempC, 5);
    Serial.print(" C; T_f=");
    Serial.print(tempF, 5);
    Serial.println(" F");
  }

  int tempCint = (tempC * 10);
  if (tempCint == lastRead) {
    Serial.print("tempCint=");
    Serial.println(tempCint);
    int multiplier = 1000;
    boolean leadingZero = true;
    for(int digit=1; digit <= 4; digit++) {
      int val = (tempCint / multiplier) % 10;
      Serial.print(digit);
      Serial.print(": val=");
      Serial.print(val);
      boolean decimalOn = (digit == 3);
      unsigned int packet = (digit << 8) | val;
      if (decimalOn) {
        packet |= DP_MASK;
      }
      leadingZero = leadingZero && digit < 3;
      if (leadingZero && val == 0) {
        Serial.print(" LZ");
        packet |= 0x0F;
      }
      
      sendData(packet);
      multiplier /= 10;
      Serial.println(" ");
    }
    Serial.println(" ");
  }
  lastRead = tempCint;
  
  delay(DELAY_IN_MS);
  uptime += DELAY_IN_MS;
}

#define BLANK 0x0F
#define MINUS 0x0A

/*
 * 4 digits
 * 
 * 0.0 -> " 0.0"
 * 0.5 -> " 0.5"
 * 0.05 -> " 0.0"
 * 100.0 -> "100.0"
 * -10.2 -> "-10.2"
 * -100.5 -> "-100"
 */
void formatFloat(float input, int values[4]) {
  int val = input * 10;
  int multiplier = 1000;
  int decimalIndex = 3;
  // Split into array values ABC.D
  for (int i = 0; i < 4; i++) {
    values[i] = (val / multiplier) % 10;
    multiplier /= 10;
  }
  int i;
  for (i = 0; i < 3; i++) {
    if (values[i] != 0) {
      break;
    }
    values[i] = BLANK;
  }
  int firstSigFig = i;
  if (val < 0) {
    if (firstSigFig == 0){
        copyRight(values);
        decimalIndex++;
        firstSigFig++;
    }
    values[firstSigFig-1] = MINUS;        
  }
}

inline void copyRight(int values[4]) {
  for (int i=0; i < 3; i++) {
    values[i+1] = values[i];
    values[i] = BLANK;
  }
}

void sendData(unsigned int packet) {
  SPI.beginTransaction(settings);
  digitalWrite(LOAD, LOW);
  SPI.transfer16(packet);
  digitalWrite(LOAD, HIGH);
  SPI.endTransaction();
}

float toF(float inC) {
  float result;
  result = (inC * 9.0 / 5.0) + 32.0;
  return result;
}
