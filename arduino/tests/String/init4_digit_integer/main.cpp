/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdio.h>

#include <assert.h>

#include "String.hpp"

int main(void)
{
	constexpr byte          value_byte          = 28;
	constexpr int           value_int           = -613174;
	constexpr unsigned int  value_unsigned_int  = 3812744;
	constexpr long          value_long          = -1282727647l;
	constexpr unsigned long value_unsigned_long = 210687641ul;

	const String string_byte_3(value_byte, 3);
	const String string_int_3(value_int, 3);
	const String string_unsigned_int_3(value_unsigned_int, 3);
	const String string_long_3(value_long, 3);
	const String string_unsigned_long_3(value_unsigned_long, 3);

	const String string_byte_4(value_byte, 4);
	const String string_int_4(value_int, 4);
	const String string_unsigned_int_4(value_unsigned_int, 4);
	const String string_long_4(value_long, 4);
	const String string_unsigned_long_4(value_unsigned_long, 4);

	const String string_byte_5(value_byte, 5);
	const String string_int_5(value_int, 5);
	const String string_unsigned_int_5(value_unsigned_int, 5);
	const String string_long_5(value_long, 5);
	const String string_unsigned_long_5(value_unsigned_long, 5);

	const String string_byte_10(value_byte, 10);
	const String string_int_10(value_int, 10);
	const String string_unsigned_int_10(value_unsigned_int, 10);
	const String string_long_10(value_long, 10);
	const String string_unsigned_long_10(value_unsigned_long, 10);

	const String string_byte_12(value_byte, 12);
	const String string_int_12(value_int, 12);
	const String string_unsigned_int_12(value_unsigned_int, 12);
	const String string_long_12(value_long, 12);
	const String string_unsigned_long_12(value_unsigned_long, 12);

	const String string_byte_16(value_byte, 16);
	const String string_int_16(value_int, 16);
	const String string_unsigned_int_16(value_unsigned_int, 16);
	const String string_long_16(value_long, 16);
	const String string_unsigned_long_16(value_unsigned_long, 16);

	const String string_byte_20(value_byte, 20);
	const String string_int_20(value_int, 20);
	const String string_unsigned_int_20(value_unsigned_int, 20);
	const String string_long_20(value_long, 20);
	const String string_unsigned_long_20(value_unsigned_long, 20);

	const String string_byte_30(value_byte, 30);
	const String string_int_30(value_int, 30);
	const String string_unsigned_int_30(value_unsigned_int, 30);
	const String string_long_30(value_long, 30);
	const String string_unsigned_long_30(value_unsigned_long, 30);


	printf("convert %u to base  3: %s\r\n", (unsigned int) value_byte, string_byte_3.c_str()); /* print digit */
	printf("convert %u to base  4: %s\r\n", (unsigned int) value_byte, string_byte_4.c_str());
	printf("convert %u to base  5: %s\r\n", (unsigned int) value_byte, string_byte_5.c_str());
	printf("convert %u to base 10: %s\r\n", (unsigned int) value_byte, string_byte_10.c_str());
	printf("convert %u to base 12: %s\r\n", (unsigned int) value_byte, string_byte_12.c_str());
	printf("convert %u to base 16: %s\r\n", (unsigned int) value_byte, string_byte_16.c_str());
	printf("convert %u to base 20: %s\r\n", (unsigned int) value_byte, string_byte_20.c_str());
	printf("convert %u to base 30: %s\r\n", (unsigned int) value_byte, string_byte_30.c_str());



	printf("convert %d to base  3: %s\r\n", value_int, string_int_3.c_str());
	printf("convert %d to base  4: %s\r\n", value_int, string_int_4.c_str());
	printf("convert %d to base  5: %s\r\n", value_int, string_int_5.c_str());
	printf("convert %d to base 10: %s\r\n", value_int, string_int_10.c_str());
	printf("convert %d to base 12: %s\r\n", value_int, string_int_12.c_str());
	printf("convert %d to base 16: %s\r\n", value_int, string_int_16.c_str());
	printf("convert %d to base 20: %s\r\n", value_int, string_int_20.c_str());
	printf("convert %d to base 30: %s\r\n", value_int, string_int_30.c_str());

	printf("convert %u to base  3: %s\r\n", value_unsigned_int, string_unsigned_int_3.c_str());
	printf("convert %u to base  4: %s\r\n", value_unsigned_int, string_unsigned_int_4.c_str());
	printf("convert %u to base  5: %s\r\n", value_unsigned_int, string_unsigned_int_5.c_str());
	printf("convert %u to base 10: %s\r\n", value_unsigned_int, string_unsigned_int_10.c_str());
	printf("convert %u to base 12: %s\r\n", value_unsigned_int, string_unsigned_int_12.c_str());
	printf("convert %u to base 16: %s\r\n", value_unsigned_int, string_unsigned_int_16.c_str());
	printf("convert %u to base 20: %s\r\n", value_unsigned_int, string_unsigned_int_20.c_str());
	printf("convert %u to base 30: %s\r\n", value_unsigned_int, string_unsigned_int_30.c_str());



	printf("convert %ld to base  3: %s\r\n", value_long, string_long_3.c_str());
	printf("convert %ld to base  4: %s\r\n", value_long, string_long_4.c_str());
	printf("convert %ld to base  5: %s\r\n", value_long, string_long_5.c_str());
	printf("convert %ld to base 10: %s\r\n", value_long, string_long_10.c_str());
	printf("convert %ld to base 12: %s\r\n", value_long, string_long_12.c_str());
	printf("convert %ld to base 16: %s\r\n", value_long, string_long_16.c_str());
	printf("convert %ld to base 20: %s\r\n", value_long, string_long_20.c_str());
	printf("convert %ld to base 30: %s\r\n", value_long, string_long_30.c_str());

	printf("convert %lu to base  3: %s\r\n", value_unsigned_long, string_unsigned_long_3.c_str());
	printf("convert %lu to base  4: %s\r\n", value_unsigned_long, string_unsigned_long_4.c_str());
	printf("convert %lu to base  5: %s\r\n", value_unsigned_long, string_unsigned_long_5.c_str());
	printf("convert %lu to base 10: %s\r\n", value_unsigned_long, string_unsigned_long_10.c_str());
	printf("convert %lu to base 12: %s\r\n", value_unsigned_long, string_unsigned_long_12.c_str());
	printf("convert %lu to base 16: %s\r\n", value_unsigned_long, string_unsigned_long_16.c_str());
	printf("convert %lu to base 20: %s\r\n", value_unsigned_long, string_unsigned_long_20.c_str());
	printf("convert %lu to base 30: %s\r\n", value_unsigned_long, string_unsigned_long_30.c_str());

	return 0;
}
