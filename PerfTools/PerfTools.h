#ifndef _PERFTOOLS_H_
#define _PERFTOOLS_H_

#include <MemoryFree.h>

#define TIMERSTART(NAME) unsigned long timer_ ## NAME = micros()
#define TIMEREND(NAME) do {\
unsigned long timer_elapsed_ ## NAME = micros() - timer_ ## NAME;\
Serial.print("elapsed for " #NAME ": ");\
Serial.println(timer_elapsed_ ## NAME);\
} while(false)

void memchk(const char * label = NULL) {
    if (label != NULL) {
        Serial.print(label);
        Serial.print(" ");
    }
    Serial.println(freeMemory());
}

#endif