#include <LCDIC2.h>

#define NULL_TEMP_VALUE -999
#define TEMP_TIMEOUT 45000

const unsigned long startTime = millis();

unsigned long elapsed() {
  return millis() - startTime;
}

struct RemoteTemp {
    double value = NULL_TEMP_VALUE;
    unsigned long lastMessageTime = 0;
    int id = NULL_TEMP_VALUE;
    char label[7]; // label[6] == '\0'
};

#define MAX_NUMBER_OF_TEMP_SENSORS 8

class TempDisplayQueue {
    public:
        TempDisplayQueue(uint8_t numberOfRemoteSensors = 8);
        ~TempDisplayQueue();
        bool begin();
    private:
        RemoteTemp* _temps;
};

TempDisplayQueue::TempDisplayQueue(uint8_t numberOfRemoteSensors) {
    _temps = (RemoteTemp*)malloc(numberOfRemoteSensors*sizeof(RemoteTemp) + 1);
}

TempDisplayQueue::~TempDisplayQueue() {
    free(_temps);
}

bool TempDisplayQueue::begin() {
    if (_temps == NULL) {
        return false;
    }
    for (int i = 0; i < 8; i++) {
        RemoteTemp t;
        _temps[i] = t;
    }
}

RemoteTemp parseTemp(char* msg) {
  RemoteTemp t;
  t.lastMessageTime = elapsed();
  
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
  return elapsed() - t.lastMessageTime > TEMP_TIMEOUT;
}

class MyLCDIC2 : public LCDIC2 {
public:
    MyLCDIC2(uint8_t address, uint8_t width, uint8_t height, bool font = 0): LCDIC2(address,width,height,font) {
        // just delegate
    }
    void println(String text) {
        LCDIC2::print(text);
        uint8_t x;
        uint8_t y;
        getCursor(x, y);
        setCursor(0, y+1);
    }
    void print(const char * text) {
        LCDIC2::print(String(text));
    }
    void print(int num) {
        LCDIC2::print(String(num));
    }
    void print(double num, uint8_t fractionDigits) {
        LCDIC2::print(String(num, fractionDigits));
    }
};
