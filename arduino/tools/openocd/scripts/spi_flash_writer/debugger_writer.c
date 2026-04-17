/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdint.h> /* for uint8_t */
#include <stddef.h> /* for size_t */
#include <string.h> /* for memset */

#include "debugger_writer.h"

void data_from_debugger(uint8_t buffer[DEBUGGER_DATA_BLOCK_SIZE],
	size_t *buffer_fill_length)
{
	/**
		* clear data,
		* this will create dummy value that if not interfered by debugger,
		* caller should terminate execution
		*/
	memset(buffer, 0, DEBUGGER_DATA_BLOCK_SIZE * sizeof(uint8_t));
	memset(buffer_fill_length, 0, sizeof(size_t));

	/**
		* give debugger access here to interfere
		* debugger then can write intended value here by breaking at label here
		*
		* DO NOT ADD SECTION DIRECTIVE HERE!
		*/
	__asm__ volatile (
		".global     INTERRUPT_HERE_TO_OVERWRITE_DATA_TO_SEND_TO_FLASH\n" \
		"INTERRUPT_HERE_TO_OVERWRITE_DATA_TO_SEND_TO_FLASH:\n"
		:
		:
		:
	);

	return; /* return */
}
