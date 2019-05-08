#include <SPI.h>

const unsigned int USE_ONLY_4DIGITS = 0x0B03;
const unsigned int DECODE_FOR_DIGITS0_3 = 0x090F;
const unsigned int FULL_INTENSITY = 0x0A0F;
const unsigned int NORMAL_OPERATION = 0x0C01;
const unsigned int SHUTDOWN_OPERATION = 0x0C00;

#define LOAD 10

SPISettings settings(9500000, MSBFIRST, SPI_MODE0);

void setup() {
  analogReference(INTERNAL);
  pinMode(LOAD, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  delay(25);
  sendData(SHUTDOWN_OPERATION);
  delay(25);
  sendData(FULL_INTENSITY);
  sendData(DECODE_FOR_DIGITS0_3);
  sendData(USE_ONLY_4DIGITS);
  sendData(NORMAL_OPERATION);
  delay(25);
}

#define NUMBER_OF_SAMPLES 25
#define DELAY 10
#define DP_MASK 0x0080

//unsigned int uptime = 0;
static int sampleIndex = 0;
static float sumSq = 0.0;

void loop() {
  int reading = analogRead(A0);
  float voltage = reading * (INTERNAL / 1023.0);
  float tempC = reading / 9.3090909090909090909090909090909;
  Serial.print("sample ");
  Serial.print(sampleIndex);
  Serial.print(": ");
  Serial.print(tempC, 4);
  sumSq += sq(tempC);
  Serial.print(", sumSq=");
  Serial.println(sumSq, 4);
  if (++sampleIndex < NUMBER_OF_SAMPLES) {
    delay(DELAY);
    return;
  }
  sampleIndex = 0;
//  float tempF = toF(tempC);
//  if (uptime % 1000 == 0) {
//    Serial.print("V=");
//    Serial.print(voltage, 5);
//    Serial.print(" V; T_c=");
//    Serial.print(tempC, 5);
//    Serial.print(" C; T_f=");
//    Serial.print(tempF, 5);
//    Serial.println(" F");
//  }

  float val = sqrt(sumSq / NUMBER_OF_SAMPLES);
  sumSq=0.0;
  Serial.print("avg=");
  Serial.println(val, 4);
  int digits[4];
  formatFloat(val, digits);
  for (int i = 0; i < 4; i++) {
    int val = digits[i];
    unsigned int packet = (i+1 << 8) | digits[i];
    Serial.print(packet, HEX);
    Serial.print(" ");
    sendData(packet);
  }
  Serial.println("\n");
  
  delay(DELAY);
//  uptime += DELAY_IN_MS;
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
inline int* formatFloat(float input, int values[4]) {
  int val = input * 10;
  boolean ltZero = val < 0;
  val = abs(val);
  int multiplier = 1000;
  int decimalIndex = 2;
  // Split into array values ABC.D
  Serial.println(val);
  for (int i = 0; i < 4; i++) {
    values[i] = (val / multiplier) % 10;
    multiplier /= 10;
    Serial.print(values[i]);
    Serial.print(' ');
  }
  Serial.println();
  int i;
  for (i = 0; i < 3; i++) {
    if (values[i] != 0) {
      Serial.print("leading trim on ");
      Serial.println(i);
      break;
    }
    values[i] = BLANK;
  }
  int firstSigFig = i;
  Serial.print("firstSig=");
  Serial.println(firstSigFig);
  if (ltZero) {
    if (firstSigFig == 0){
        copyRight(values);
        decimalIndex++;
        firstSigFig++;
    }
    values[firstSigFig-1] = MINUS;        
  }
  values[decimalIndex] |= DP_MASK;
  return values;
}

inline void copyRight(int values[4]) {
  for (int i = 3; i > 0; i--) {
    values[i] = values[i-1];
  }
  values[0] = BLANK;
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
