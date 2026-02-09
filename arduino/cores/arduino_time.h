#ifndef arduino_time_h
#define arduino_time_h

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void delay(unsigned long ms);

void delayMicroseconds(unsigned int us);

unsigned long millis(void);

unsigned long micros(void);

#ifdef __cplusplus
}
#endif

#endif