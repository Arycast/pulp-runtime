#include <stdint.h>
#include <stddef.h>

#include "platform_board.h"

int platform_board_analog_read(size_t pin, uint32_t *value)
{
	(void) pin;
	*value = 0;
	return 0;
}

int platform_board_analog_write(size_t pin, uint32_t value)
{
	(void) pin;
	(void) value;
	return 0;
}
