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

	const String string_byte_oct(value_byte, OCT);
	const String string_int_oct(value_int, OCT);
	const String string_unsigned_int_oct(value_unsigned_int, OCT);
	const String string_long_oct(value_long, OCT);
	const String string_unsigned_long_oct(value_unsigned_long, OCT);

	const String string_byte_hex(value_byte, HEX);
	const String string_int_hex(value_int, HEX);
	const String string_unsigned_int_hex(value_unsigned_int, HEX);
	const String string_long_hex(value_long, HEX);
	const String string_unsigned_long_hex(value_unsigned_long, HEX);

	const String string_byte(value_byte);
	const String string_int(value_int);
	const String string_unsigned_int(value_unsigned_int);
	const String string_long(value_long);
	const String string_unsigned_long(value_unsigned_long);


	const String string_value1("    0111");
	const String string_value2("  +00001");
	const String string_value3("-92fish");
	const String string_value4("   +3233text");
	const String string_value5;




	printf("(OCT) %s, value: %u, toInt() = %ld\n", string_byte_oct.c_str(), (unsigned int) value_byte, string_byte_oct.toInt(OCT));
	printf("(OCT) %s, value: %d, toInt() = %ld\n", string_int_oct.c_str(), value_int, string_int_oct.toInt(OCT));
	printf("(OCT) %s, value: %u, toInt() = %ld\n", string_unsigned_int_oct.c_str(), value_unsigned_int, string_unsigned_int_oct.toInt(OCT));
	printf("(OCT) %s, value: %ld, toInt() = %ld\n", string_long_oct.c_str(), value_long, string_long_oct.toInt(OCT));
	printf("(OCT) %s, value: %lu, toInt() = %ld\n", string_unsigned_long_oct.c_str(), value_unsigned_long, string_unsigned_long_oct.toInt(OCT));

	printf("value1 (OCT?) %s, toInt() = %ld\n", string_value1.c_str(), string_value1.toInt(OCT));
	printf("value2 (OCT?) %s, toInt() = %ld\n", string_value2.c_str(), string_value2.toInt(OCT));
	printf("value3 (OCT?) %s, toInt() = %ld\n", string_value3.c_str(), string_value3.toInt(OCT));
	printf("value4 (OCT?) %s, toInt() = %ld\n", string_value4.c_str(), string_value4.toInt(OCT));
	printf("value5 (OCT?) %s, toInt() = %ld\n", string_value5.c_str(), string_value5.toInt(OCT));


	printf("(HEX) %s, value: %u, toInt() = %ld\n", string_byte_hex.c_str(), (unsigned int) value_byte, string_byte_hex.toInt(HEX));
	printf("(HEX) %s, value: %d, toInt() = %ld\n", string_int_hex.c_str(), value_int, string_int_hex.toInt(HEX));
	printf("(HEX) %s, value: %u, toInt() = %ld\n", string_unsigned_int_hex.c_str(), value_unsigned_int, string_unsigned_int_hex.toInt(HEX));
	printf("(HEX) %s, value: %ld, toInt() = %ld\n", string_long_hex.c_str(), value_long, string_long_hex.toInt(HEX));
	printf("(HEX) %s, value: %lu, toInt() = %ld\n", string_unsigned_long_hex.c_str(), value_unsigned_long, string_unsigned_long_hex.toInt(HEX));

	printf("value1 (HEX?) %s, toInt() = %ld\n", string_value1.c_str(), string_value1.toInt(HEX));
	printf("value2 (HEX?) %s, toInt() = %ld\n", string_value2.c_str(), string_value2.toInt(HEX));
	printf("value3 (HEX?) %s, toInt() = %ld\n", string_value3.c_str(), string_value3.toInt(HEX));
	printf("value4 (HEX?) %s, toInt() = %ld\n", string_value4.c_str(), string_value4.toInt(HEX));
	printf("value5 (HEX?) %s, toInt() = %ld\n", string_value5.c_str(), string_value5.toInt(HEX));


	printf("      %s, value: %u, toInt() = %ld\n", string_byte.c_str(), (unsigned int) value_byte, string_byte.toInt());
	printf("      %s, value: %d, toInt() = %ld\n", string_int.c_str(), value_int, string_int.toInt());
	printf("      %s, value: %u, toInt() = %ld\n", string_unsigned_int.c_str(), value_unsigned_int, string_unsigned_int.toInt());
	printf("      %s, value: %ld, toInt() = %ld\n", string_long.c_str(), value_long, string_long.toInt());
	printf("      %s, value: %lu, toInt() = %ld\n", string_unsigned_long.c_str(), value_unsigned_long, string_unsigned_long.toInt());

	printf("value1        %s, toInt() = %ld\n", string_value1.c_str(), string_value1.toInt());
	printf("value2        %s, toInt() = %ld\n", string_value2.c_str(), string_value2.toInt());
	printf("value3        %s, toInt() = %ld\n", string_value3.c_str(), string_value3.toInt());
	printf("value4        %s, toInt() = %ld\n", string_value4.c_str(), string_value4.toInt());
	printf("value5        %s, toInt() = %ld\n", string_value5.c_str(), string_value5.toInt());


	return 0;
}
