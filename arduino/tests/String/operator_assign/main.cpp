/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdio.h>

#include "String.hpp"

int main(void)
{
	String                  sentence1("this is sentence 1");
	String                  sentence2("this is sentence 2");
	String                  sentence;

	constexpr byte          value1_byte          = 100;
	constexpr byte          value2_byte          = 28;
	constexpr int           value1_int           = -1911221;
	constexpr int           value2_int           = -613174;
	constexpr unsigned int  value1_unsigned_int  = 2000000;
	constexpr unsigned int  value2_unsigned_int  = 3812744;
	constexpr long          value1_long          = -1054824499l;
	constexpr long          value2_long          = -1282727647l;
	constexpr unsigned long value1_unsigned_long = 302342892ul;
	constexpr unsigned long value2_unsigned_long = 210687641ul;
	constexpr float         value1_float         = 8020.30329f;
	constexpr float         value2_float         = 19931.31079f;
	constexpr double        value1_double        = -2646.30945;
	constexpr double        value2_double        = -4276.28735; /* double doesn't need suffix */

	sentence = sentence1;
	printf("%s\n", sentence.c_str());

	sentence = sentence2;
	printf("%s\n", sentence.c_str());

	sentence = value1_byte;
	printf("%s\n", sentence.c_str());

	sentence = value2_byte;
	printf("%s\n", sentence.c_str());

	sentence = value1_int;
	printf("%s\n", sentence.c_str());

	sentence = value2_int;
	printf("%s\n", sentence.c_str());

	sentence = value1_unsigned_int;
	printf("%s\n", sentence.c_str());

	sentence = value2_unsigned_int;
	printf("%s\n", sentence.c_str());

	sentence = value1_long;
	printf("%s\n", sentence.c_str());

	sentence = value2_long;
	printf("%s\n", sentence.c_str());

	sentence = value1_unsigned_long;
	printf("%s\n", sentence.c_str());

	sentence = value2_unsigned_long;
	printf("%s\n", sentence.c_str());

	sentence = value1_float;
	printf("%s\n", sentence.c_str());

	sentence = value2_float;
	printf("%s\n", sentence.c_str());

	sentence = value1_double;
	printf("%s\n", sentence.c_str());

	sentence = value2_double;
	printf("%s\n", sentence.c_str());

	return 0;
}
