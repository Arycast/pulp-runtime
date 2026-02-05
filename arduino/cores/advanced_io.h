#ifndef advanced_io_h
#define advanced_io_h

#include <stdint.h>

/*#define HIGH 1
#define LOW 0

#define MSBFIRST 1
#define LSBFIRST 0

#define GPIO_PWM 17
#define GPIO_PWM_MUX 2*/


/* to suport C code, provide C wrapper to call C++ code */
#ifndef __cplusplus

/**
	* if we compile in plain C,
	* redefine pulseIn and pulseInLong function name as
	* pulseIn_c and pulseInLong_c
	*/
#define  pulseIn      __pulseIn_c
#define  pulseInLong  __pulseInLong_c

#endif



/* provide these function as C function */

#ifdef __cplusplus
extern "C"
{
#endif

void noTone(uint8_t pin);

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value);

void tone(uint8_t pin, unsigned int frequency, unsigned long duration); 

#ifdef __cplusplus
} /* extern "C" */
#endif



/**
	* to work around polymorphism, when __cplusplus defined
	* we need to declare extern c to c wrapper function
	*/
#ifndef __cplusplus

/* C */

/* no polymorphism in C */
unsigned long pulseIn(int pin, int value);
unsigned long pulseInLong(int pin, int value);

/* not part of standard */
unsigned long __pulseIn_c_with_timeout(int pin, int value, unsigned long timeout);
unsigned long __pulseInLong_c_with_timeout(int pin, int value, unsigned long timeout);

/* ! defined(__cplusplus) */

#else /* defined(__cplusplus) */

/* C++ */

/* provide polymorphism in C++ */
unsigned long pulseIn(int pin, int value);
unsigned long pulseInLong(int pin, int value);

unsigned long pulseIn(int pin, int value, unsigned long timeout);
unsigned long pulseInLong(int pin, int value, unsigned long timeout);

/* provide c low level function */

extern "C" unsigned long __pulseIn_c(int pin, int value);
extern "C" unsigned long __pulseInLong_c(int pin, int value);

extern "C" unsigned long __pulseIn_c_with_timeout(int pin, int value, unsigned long timeout);
extern "C" unsigned long __pulseInLong_c_with_timeout(int pin, int value, unsigned long timeout);

#endif /* defined(__cplusplus) */

#endif
