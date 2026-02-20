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

	char digit_str[256];

	size_t  retval;

	retval = unsigned_char_to_string_export(digit_str, 256, value_byte, 3);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base  3: %s\r\n", (unsigned int) value_byte, digit_str); /* print digit */

	retval = unsigned_char_to_string_export(digit_str, 256, value_byte, 4);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base  4: %s\r\n", (unsigned int) value_byte, digit_str);

	retval = unsigned_char_to_string_export(digit_str, 256, value_byte, 5);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base  5: %s\r\n", (unsigned int) value_byte, digit_str);

	retval = unsigned_char_to_string_export(digit_str, 256, value_byte, 11);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 11: %s\r\n", (unsigned int) value_byte, digit_str);

	retval = unsigned_char_to_string_export(digit_str, 256, value_byte, 12);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 12: %s\r\n", (unsigned int) value_byte, digit_str);

	retval = unsigned_char_to_string_export(digit_str, 256, value_byte, 16);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 16: %s\r\n", (unsigned int) value_byte, digit_str);

	retval = unsigned_char_to_string_export(digit_str, 256, value_byte, 20);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 20: %s\r\n", (unsigned int) value_byte, digit_str);

	retval = unsigned_char_to_string_export(digit_str, 256, value_byte, 30);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 30: %s\r\n", (unsigned int) value_byte, digit_str);





	retval = unsigned_int_to_string_export(digit_str, 256, (unsigned int) value_int, 3);
	assert((retval > 0) && (retval < 256));
	printf("convert %d to base  3: %s\r\n", value_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, (unsigned int) value_int, 4);
	assert((retval > 0) && (retval < 256));
	printf("convert %d to base  4: %s\r\n", value_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, (unsigned int) value_int, 5);
	assert((retval > 0) && (retval < 256));
	printf("convert %d to base  5: %s\r\n", value_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, (unsigned int) value_int, 10);
	assert((retval > 0) && (retval < 256));
	printf("convert %d to base 10: %s\r\n", value_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, (unsigned int) value_int, 12);
	assert((retval > 0) && (retval < 256));
	printf("convert %d to base 12: %s\r\n", value_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, (unsigned int) value_int, 16);
	assert((retval > 0) && (retval < 256));
	printf("convert %d to base 16: %s\r\n", value_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, (unsigned int) value_int, 20);
	assert((retval > 0) && (retval < 256));
	printf("convert %d to base 20: %s\r\n", value_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, (unsigned int) value_int, 30);
	assert((retval > 0) && (retval < 256));
	printf("convert %d to base 30: %s\r\n", value_int, digit_str);



	retval = unsigned_int_to_string_export(digit_str, 256, value_unsigned_int, 3);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base  3: %s\r\n", value_unsigned_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, value_unsigned_int, 4);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base  4: %s\r\n", value_unsigned_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, value_unsigned_int, 5);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base  5: %s\r\n", value_unsigned_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, value_unsigned_int, 10);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 10: %s\r\n", value_unsigned_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, value_unsigned_int, 12);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 12: %s\r\n", value_unsigned_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, value_unsigned_int, 16);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 16: %s\r\n", value_unsigned_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, value_unsigned_int, 20);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 20: %s\r\n", value_unsigned_int, digit_str);

	retval = unsigned_int_to_string_export(digit_str, 256, value_unsigned_int, 30);
	assert((retval > 0) && (retval < 256));
	printf("convert %u to base 30: %s\r\n", value_unsigned_int, digit_str);





	retval = unsigned_long_to_string_export(digit_str, 256, (unsigned long) value_long, 3);
	assert((retval > 0) && (retval < 256));
	printf("convert %ld to base  3: %s\r\n", value_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, (unsigned long) value_long, 4);
	assert((retval > 0) && (retval < 256));
	printf("convert %ld to base  4: %s\r\n", value_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, (unsigned long) value_long, 5);
	assert((retval > 0) && (retval < 256));
	printf("convert %ld to base  5: %s\r\n", value_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, (unsigned long) value_long, 10);
	assert((retval > 0) && (retval < 256));
	printf("convert %ld to base 10: %s\r\n", value_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, (unsigned long) value_long, 12);
	assert((retval > 0) && (retval < 256));
	printf("convert %ld to base 12: %s\r\n", value_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, (unsigned long) value_long, 16);
	assert((retval > 0) && (retval < 256));
	printf("convert %ld to base 16: %s\r\n", value_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, (unsigned long) value_long, 20);
	assert((retval > 0) && (retval < 256));
	printf("convert %ld to base 20: %s\r\n", value_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, (unsigned long) value_long, 30);
	assert((retval > 0) && (retval < 256));
	printf("convert %ld to base 30: %s\r\n", value_long, digit_str);



	retval = unsigned_long_to_string_export(digit_str, 256, value_unsigned_long, 3);
	assert((retval > 0) && (retval < 256));
	printf("convert %lu to base  3: %s\r\n", value_unsigned_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, value_unsigned_long, 4);
	assert((retval > 0) && (retval < 256));
	printf("convert %lu to base  4: %s\r\n", value_unsigned_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, value_unsigned_long, 5);
	assert((retval > 0) && (retval < 256));
	printf("convert %lu to base  5: %s\r\n", value_unsigned_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, value_unsigned_long, 10);
	assert((retval > 0) && (retval < 256));
	printf("convert %lu to base 10: %s\r\n", value_unsigned_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, value_unsigned_long, 12);
	assert((retval > 0) && (retval < 256));
	printf("convert %lu to base 12: %s\r\n", value_unsigned_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, value_unsigned_long, 16);
	assert((retval > 0) && (retval < 256));
	printf("convert %lu to base 16: %s\r\n", value_unsigned_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, value_unsigned_long, 20);
	assert((retval > 0) && (retval < 256));
	printf("convert %lu to base 20: %s\r\n", value_unsigned_long, digit_str);

	retval = unsigned_long_to_string_export(digit_str, 256, value_unsigned_long, 30);
	assert((retval > 0) && (retval < 256));
	printf("convert %lu to base 30: %s\r\n", value_unsigned_long, digit_str);

	return 0;
}
