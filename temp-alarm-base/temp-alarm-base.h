#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>

#include <MemoryFree.h>

#define NULL_TEMP_VALUE -999
#define TEMP_TIMEOUT 45000

struct RemoteTemp {
    double value = NULL_TEMP_VALUE;
    long lastMessageTime = 0;
    int id = NULL_TEMP_VALUE;
    char label[7]; // label[6] == '\0'
};

#define MAX_NUMBER_OF_TEMP_SENSORS 8

#define _ALPHANUMERIC_FONT X11fixed7x14B
#define _UNICODE_FONT utf8font10x16

class TempDisplayQueue {
    public:
        TempDisplayQueue();
        ~TempDisplayQueue();
        bool begin(RemoteTemp* buffer = NULL);
        RemoteTemp* temps;
    private:
};

TempDisplayQueue::TempDisplayQueue() {
}

TempDisplayQueue::~TempDisplayQueue() {
    free(temps);
}

bool TempDisplayQueue::begin(RemoteTemp* buffer) {
    if (buffer != NULL) {
        temps = buffer;
    }
    if (temps == NULL) {
        return false;
    }
    for (int i = 0; i < 8; i++) {
        RemoteTemp t;
        temps[i] = t;
    }
}
