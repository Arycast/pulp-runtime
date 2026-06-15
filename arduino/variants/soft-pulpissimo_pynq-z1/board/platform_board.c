#include <stdint.h>
#include <stddef.h>

#include "platform_board.h"

/* board macro should be defined */
#ifndef ARDUINO_SOFT_PULPISSIMO_PYNQ_Z1
#error  macro ARDUINO_SOFT_PULPISSIMO_PYNQ_Z1 is not defined, \
	it seems that this file is included in compilation unit \
	that is being built without arduino or \
	misconfiguration in platform.txt or board.txt or \
	this file is included in wrong board configuration
#endif

/* to be used to read analog value through external device (external adc) */
int platform_board_analog_read(size_t pin, uint32_t *value)
{
	(void) pin;
	*value = 0;
	return 0;
}

/* to be used to write analog value through external device (external dac) */
int platform_board_analog_write(size_t pin, uint32_t value)
{
	(void) pin;
	(void) value;
	return 0;
}
