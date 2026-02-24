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
	std::vector<String>     source       = {"this", "is", "a", "test", "sentence"};

	/* Use std::move to transfer resources to 'destination' */
	std::vector<String>     destination  = std::move(source);

	String                  sentence;

	constexpr byte          lhs_byte          = 100;
	constexpr byte          rhs_byte          = 28;
	constexpr int           lhs_int           = -1911221;
	constexpr int           rhs_int           = -613174;
	constexpr unsigned int  lhs_unsigned_int  = 2000000;
	constexpr unsigned int  rhs_unsigned_int  = 3812744;
	constexpr long          lhs_long          = -1054824499l;
	constexpr long          rhs_long          = -1282727647l;
	constexpr unsigned long lhs_unsigned_long = 302342892ul;
	constexpr unsigned long rhs_unsigned_long = 210687641ul;
	constexpr float         lhs_float         = 8020.30329f;
	constexpr float         rhs_float         = 19931.31079f;
	constexpr double        lhs_double        = -2646.30945;
	constexpr double        rhs_double        = -4276.28735; /* double doesn't need suffix */

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += (" " + value);
	}

	sentence = lhs_byte          + (' ' + sentence + ' ') + rhs_byte          + ' ';
	sentence = lhs_int           + (' ' + sentence + ' ') + rhs_int           + ' ';
	sentence = lhs_unsigned_int  + (' ' + sentence + ' ') + rhs_unsigned_int  + ' ';
	sentence = lhs_long          + (' ' + sentence + ' ') + rhs_long          + ' ';
	sentence = lhs_unsigned_long + (' ' + sentence + ' ') + rhs_unsigned_long + ' ';
	sentence = lhs_float         + (' ' + sentence + ' ') + rhs_float         + ' ';
	sentence = lhs_double        + (' ' + sentence + ' ') + rhs_double        /*+ ' '*/;

	/* print sentence */
	printf("%s\r\n", sentence.c_str());

	return 0;
}
