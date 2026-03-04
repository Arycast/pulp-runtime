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
	constexpr float      value1 = 290823.092134304398213878989f;
	constexpr float      value2 = -613.2304354f;
	constexpr double     value3 = 3812744e2;
	constexpr double     value4 = -128.99999999999;

	const String s_value1(value1);
	const String s_value2(value2);
	const String s_value3(value3);
	const String s_value4(value4);
	const String s_value5("   -1e-3");
	const String s_value6(" +e10");
	const String s_value7("+1.e+10");
	const String s_value8("-1.10000004");
	const String s_value9("-.10064");
	const String s_value10("-.10064e");

	const String s_value1p1(value1, 1);
	const String s_value2p1(value2, 1);
	const String s_value3p1(value3, 1);
	const String s_value4p1(value4, 1);

	const String s_value1p10(value1, 10);
	const String s_value2p10(value2, 10);
	const String s_value3p10(value3, 10);
	const String s_value4p10(value4, 10);


	printf("value1: %s\n", s_value1.c_str());
	printf("value1p1: %s\n", s_value1p1.c_str());
	printf("value1p10: %s\n", s_value1p10.c_str());
	printf("value2: %s\n", s_value2.c_str());
	printf("value2p1: %s\n", s_value2p1.c_str());
	printf("value2p10: %s\n", s_value2p10.c_str());
	printf("value3: %s\n", s_value3.c_str());
	printf("value3p1: %s\n", s_value3p1.c_str());
	printf("value3p10: %s\n", s_value3p10.c_str());
	printf("value4: %s\n", s_value4.c_str());
	printf("value4p1: %s\n", s_value4p1.c_str());
	printf("value4p10: %s\n", s_value4p10.c_str());

	printf("value5: %s, toDouble() = %f\n", s_value5.c_str(), s_value5.toDouble());
	printf("value6: %s, toDouble() = %f\n", s_value6.c_str(), s_value6.toDouble());
	printf("value7: %s, toDouble() = %f\n", s_value7.c_str(), s_value7.toDouble());
	printf("value8: %s, toDouble() = %f\n", s_value8.c_str(), s_value8.toDouble());
	printf("value9: %s, toDouble() = %f\n", s_value9.c_str(), s_value9.toDouble());
	printf("value10: %s, toDouble() = %f\n", s_value10.c_str(), s_value10.toDouble());

	return 0;
}
