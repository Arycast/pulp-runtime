/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdint.h>
#include <stddef.h>

#include <ctype.h>
#include <stdio.h>

#include "../../debugger_writer.h"

#ifndef MAX_HEX_DUMP_LINES
#define MAX_HEX_DUMP_LINES 4
#endif

static void hex_dump(size_t offset, const void * restrict buffer, size_t buffer_len,
	FILE * restrict stream);

int main(void)
{
	uint8_t  buffer[DEBUGGER_DATA_BLOCK_SIZE];
	size_t   buffer_len;

	size_t   accumulate = 0;

	do
	{
		/* let gdb set data */
		data_from_debugger(buffer, &buffer_len);

		/* write data if debugger set length to non-zero */
		if (buffer_len > 0)
		{
			/* write data to spi flash */
			hex_dump(accumulate, buffer, buffer_len, stdout);

			/* accumulate data */
			accumulate += buffer_len;
		}
	}
	while (buffer_len >= DEBUGGER_DATA_BLOCK_SIZE);

	if (buffer_len == 0)
	{
		fputs("debugger do not set buffer fill length or set it to 0\n", stderr);
	}

	return 0;
}

/**
	* print in hex format
	* since restrict keyword is an extension (not part of C11 standard),
	* prefix it with underscore
	*/
static void hex_dump(size_t offset, const void * restrict buffer, size_t buffer_len,
	FILE * restrict stream)
{
	const unsigned char *_buffer = (const unsigned char *) buffer;
	size_t i, j;

	for (i = 0; (i < buffer_len) && (i < (MAX_HEX_DUMP_LINES * 16)); i += 16)
	{
		/* 1. Print the offset (like 00000010:) */
		fprintf(stream, "%08zx: ", offset + i); /* z is for size_t, like l is for long */

		/* 2. Print the hex values */
		for (j = 0; j < 16; j++)
		{
			if ((i + j) < buffer_len)
			{
				fprintf(stream, "%02x ", (unsigned int) _buffer[i + j]);
			}
			else
			{
				fputs("   ", stream); /* Padding for the last line */
			}
		}

		/* separator to literal value */
		fputc(' ', stream);

		/* 3. Print the ASCII sidebar */
		for (j = 0; j < 16; j++)
		{
			if ((i + j) < buffer_len)
			{
				char c = (char) _buffer[i + j];

				/* Only print printable characters; dots for everything else */
				fputc(isprint(c) ? c : '.', stream);
			}
		}

		/* next line */
		fputc('\n', stream);
	}

	if (i < buffer_len)
	{
		fputs("........  (continue...)\n", stream);
	}
}
