/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdint.h>

/* additional stack */

#ifndef PULPISIMO_ADDITIONAL_STACK
#define PULPISIMO_ADDITIONAL_STACK  (6 * 1024)
#endif

/* make sure we don't add too much */
#if ((PULPISIMO_ADDITIONAL_STACK) > (8 * 1024))
	#error PULPISIMO_ADDITIONAL_STACK should not set to be more than 8 kiB
#endif

/* allocate in more space in stack */
uint8_t more_stack[PULPISIMO_ADDITIONAL_STACK] __attribute__ ((section(".stack")));
