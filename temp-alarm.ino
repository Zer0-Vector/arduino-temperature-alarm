#include <SPI.h>

#define USE_ONLY_4DIGITS (uint8_t)0x0B03
#define DECODE_FOR_DIGITS0_3 (uint8_t)0x090F
#define FULL_INTENSITY (uint8_t)0x0A0F
#define NORMAL_OPERATION (uint8_t)0x0C01
#define SHUTDOWN_OPERATION (uint8_t)0x0C00

#define LOAD_PIN 10
#define LED_YELLOW_PIN 2
#define LED_RED_PIN 4
#define LED_GREEN_PIN 5

#define NUMBER_OF_SAMPLES 128
#define DELAY 15
#define DP_MASK 0x0080

#define DEBUG

SPISettings settings(9500000, MSBFIRST, SPI_MODE0);

void setup() {
  analogReference(INTERNAL);
  pinMode(LOAD_PIN, OUTPUT);
  // pinMode(LED_YELLOW_PIN, OUTPUT);
  // pinMode(LED_RED_PIN, OUTPUT);
  // pinMode(LED_GREEN_PIN, OUTPUT);
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

  // digitalWrite(LED_YELLOW_PIN, LOW);
  // digitalWrite(LED_RED_PIN, LOW);
  // digitalWrite(LED_GREEN_PIN, LOW);
  // delay(250);

  // digitalWrite(LED_GREEN_PIN, HIGH);
  
  // delay(500);
  // digitalWrite(LED_RED_PIN, HIGH);
  // digitalWrite(LED_GREEN_PIN, LOW);
  
  // delay(500);
  // digitalWrite(LED_YELLOW_PIN, HIGH);
  // digitalWrite(LED_RED_PIN, LOW);
  
  // delay(500);
  // digitalWrite(LED_GREEN_PIN, HIGH);
  // digitalWrite(LED_YELLOW_PIN, LOW);
  // delay(500);
  // digitalWrite(LED_RED_PIN, HIGH);
  // delay(500);
  // digitalWrite(LED_YELLOW_PIN, HIGH);
  // delay(2000);
  // digitalWrite(LED_YELLOW_PIN, LOW);
  // digitalWrite(LED_RED_PIN, LOW);
  // digitalWrite(LED_GREEN_PIN, LOW);
  // delay(250);
}

#ifdef DEBUG
static unsigned int uptime = 0;
#endif
static unsigned int sampleIndex = 0;
static double sumSq = 0.0;

// V [mV] = R * (1100 / 1024) [mV/steps]
// T [degC] = V / 10 [mV/degC]
// T = R * ((1100 / 1024) / 10) = R * LM35_CONVERSION_CONSTANT
#define LM35_CONVERSION_CONSTANT ((double)0.107421875)

void loop() {
  int reading = analogRead(A0); // [0, 1024)
#ifdef DEBUG
  uptime = millis() - uptime;
#endif
  sumSq += sq(reading * LM35_CONVERSION_CONSTANT);
  if (++sampleIndex < NUMBER_OF_SAMPLES) {
    delay(DELAY);
    return;
  }
  sampleIndex = 0;
  double val = toF(sqrt(sumSq / NUMBER_OF_SAMPLES));
  sumSq = 0.0;
#ifdef DEBUG
  Serial.println(uptime);
  Serial.print(F("avg="));
  Serial.println(val, 4);
#endif
  
  int digits[4];
  formatFloat(val, digits);
  for (int i = 0; i < 4; i++) {
    int val = digits[i];
    unsigned int packet = (i + 1 << 8) | digits[i];
#ifdef DEBUG    
    Serial.print(packet, HEX);
    Serial.print(F(" "));
#endif
    sendData(packet);
  }
#ifdef DEBUG
  Serial.println(F("\n"));
#endif

  delay(DELAY);
}

#define BLANK 0x0F
#define MINUS 0x0A

/*
   4 digits

   0.0 -> " 0.0"
   0.5 -> " 0.5"
   0.05 -> " 0.0"
   100.0 -> "100.0"
   -10.2 -> "-10.2"
   -100.5 -> "-100"
*/
inline int* formatFloat(double input, int values[4]) {
  int val = input * 10;
  boolean ltZero = val < 0;
  val = abs(val);
  int multiplier = 1000;
  int decimalIndex = 2;
  // Split into array values ABC.D
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
      break;
    }
    values[i] = BLANK;
  }
  int firstSigFig = i;
  if (ltZero) {
    if (firstSigFig == 0) {
      copyRight(values);
      decimalIndex++;
      firstSigFig++;
    }
    values[firstSigFig - 1] = MINUS;
  }
  values[decimalIndex] |= DP_MASK;
  return values;
}

inline void copyRight(int values[4]) {
  for (int i = 3; i > 0; i--) {
    values[i] = values[i - 1];
  }
  values[0] = BLANK;
}

inline void sendData(unsigned int packet) {
  SPI.beginTransaction(settings);
  digitalWrite(LOAD_PIN, LOW);
  SPI.transfer16(packet);
  digitalWrite(LOAD_PIN, HIGH);
  SPI.endTransaction();
}

inline double toF(double inC) {
  return (inC * 9.0 / 5.0) + 32.0;
}
