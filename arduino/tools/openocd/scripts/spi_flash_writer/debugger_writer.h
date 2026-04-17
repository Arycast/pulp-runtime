/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __DEBUGGER_WRITER_H__
#define __DEBUGGER_WRITER_H__

#include <stdint.h>
#include <stddef.h>

/**
	* data block size to write to flash
	* default size is 1 sector, which is 4 kiB
	*/
#ifndef DEBUGGER_DATA_BLOCK_SIZE
#define DEBUGGER_DATA_BLOCK_SIZE     (4 * 1024)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* interface to debugger */
void data_from_debugger(uint8_t buffer[DEBUGGER_DATA_BLOCK_SIZE],
	size_t *buffer_fill_length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(__DEBUGGER_WRITER_H__) */
