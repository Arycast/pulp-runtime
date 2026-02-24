/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <vector>
#include <utility> /* Required for std::move */

#include <stdio.h>

#include "String.hpp"

int main(void)
{
	constexpr byte          value_byte          = 28;
	constexpr int           value_int           = -613174;
	constexpr unsigned int  value_unsigned_int  = 3812744;
	constexpr long          value_long          = -1282727647l;
	constexpr unsigned long value_unsigned_long = 210687641ul;
	constexpr float         value_float         = 19931.31079f;
	constexpr double        value_double        = -4276.28735; /* double doesn't need suffix */

	String                  sentence("test concat ");

	sentence += value_byte;
	sentence += ' ';
	sentence += value_int;
	sentence += ' ';
	sentence += value_unsigned_int;
	sentence += ' ';
	sentence += value_long;
	sentence += ' ';
	sentence += value_unsigned_long;
	sentence += ' ';
	sentence += value_float;
	sentence += ' ';
	sentence += value_double;

	/* print sentence */
	printf("%s\r\n", sentence.c_str());

	return 0;
}
