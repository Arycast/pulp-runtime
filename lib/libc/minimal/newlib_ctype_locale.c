/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/**
	* implementation of __locale_ctype_ptr for newlib
	*/

#include <ctype.h>

__attribute__ ((weak)) const char * __locale_ctype_ptr(void)
{
	/* set the most basic locale that is "C" */
	return "C";
}
