/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdbool.h>
#include <stdio.h>

#include "String.hpp"

static char * bool_to_string(char *s, bool value);

int main(void)
{
	constexpr const char   *a = "a";
	constexpr const char   *b = "b";

	constexpr const char   *num_1 = "1";
	constexpr const char   *num_2 = "2";

	constexpr const char   *num_999 = "999";
	constexpr const char   *num_1000 = "1000";

	String                  s_a = a;
	String                  s_b = b;

	String                  s_num_1 = num_1;
	String                  s_num_2 = num_2;

	String                  s_num_999 = num_999;
	String                  s_num_1000 = num_1000;

	char                    buf[8];

	/* print sentence */

	printf("a greater than b : %s\n", bool_to_string(buf, s_a > b));
	printf("a greater than b : %s\n", bool_to_string(buf, a > s_b));
	printf("a greater than b : %s\n", bool_to_string(buf, s_a > s_b));

	printf("a greater than a : %s\n", bool_to_string(buf, s_a > a));
	printf("a greater than a : %s\n", bool_to_string(buf, a > s_a));
	printf("a greater than a : %s\n", bool_to_string(buf, s_a > s_a));

	printf("b greater than b : %s\n", bool_to_string(buf, s_b > b));
	printf("b greater than b : %s\n", bool_to_string(buf, b > s_b));
	printf("b greater than b : %s\n", bool_to_string(buf, s_b > s_b));

	printf("1 greater than 2 : %s\n", bool_to_string(buf, s_num_1 > num_2));
	printf("1 greater than 2 : %s\n", bool_to_string(buf, num_1 > s_num_2));
	printf("1 greater than 2 : %s\n", bool_to_string(buf, s_num_1 > s_num_2));

	printf("999 greater than 1000 : %s\n", bool_to_string(buf, s_num_999 > num_1000));
	printf("999 greater than 1000 : %s\n", bool_to_string(buf, num_999 > s_num_1000));
	printf("999 greater than 1000 : %s\n", bool_to_string(buf, s_num_999 > s_num_1000));


	printf("a greater or equal than b : %s\n", bool_to_string(buf, s_a >= b));
	printf("a greater or equal than b : %s\n", bool_to_string(buf, a >= s_b));
	printf("a greater or equal than b : %s\n", bool_to_string(buf, s_a >= s_b));

	printf("a greater or equal than a : %s\n", bool_to_string(buf, s_a >= a));
	printf("a greater or equal than a : %s\n", bool_to_string(buf, a >= s_a));
	printf("a greater or equal than a : %s\n", bool_to_string(buf, s_a >= s_a));

	printf("b greater or equal than b : %s\n", bool_to_string(buf, s_b >= b));
	printf("b greater or equal than b : %s\n", bool_to_string(buf, b >= s_b));
	printf("b greater or equal than b : %s\n", bool_to_string(buf, s_b >= s_b));

	printf("1 greater or equal than 2 : %s\n", bool_to_string(buf, s_num_1 >= num_2));
	printf("1 greater or equal than 2 : %s\n", bool_to_string(buf, num_1 >= s_num_2));
	printf("1 greater or equal than 2 : %s\n", bool_to_string(buf, s_num_1 >= s_num_2));

	printf("999 greater or equal than 1000 : %s\n", bool_to_string(buf, s_num_999 >= num_1000));
	printf("999 greater or equal than 1000 : %s\n", bool_to_string(buf, num_999 >= s_num_1000));
	printf("999 greater or equal than 1000 : %s\n", bool_to_string(buf, s_num_999 >= s_num_1000));


	printf("a less than b : %s\n", bool_to_string(buf, s_a < b));
	printf("a less than b : %s\n", bool_to_string(buf, a < s_b));
	printf("a less than b : %s\n", bool_to_string(buf, s_a < s_b));

	printf("a less than a : %s\n", bool_to_string(buf, s_a < a));
	printf("a less than a : %s\n", bool_to_string(buf, a < s_a));
	printf("a less than a : %s\n", bool_to_string(buf, s_a < s_a));

	printf("b less than b : %s\n", bool_to_string(buf, s_b < b));
	printf("b less than b : %s\n", bool_to_string(buf, b < s_b));
	printf("b less than b : %s\n", bool_to_string(buf, s_b < s_b));

	printf("1 less than 2 : %s\n", bool_to_string(buf, s_num_1 < num_2));
	printf("1 less than 2 : %s\n", bool_to_string(buf, num_1 < s_num_2));
	printf("1 less than 2 : %s\n", bool_to_string(buf, s_num_1 < s_num_2));

	printf("999 less than 1000 : %s\n", bool_to_string(buf, s_num_999 < num_1000));
	printf("999 less than 1000 : %s\n", bool_to_string(buf, num_999 < s_num_1000));
	printf("999 less than 1000 : %s\n", bool_to_string(buf, s_num_999 < s_num_1000));


	printf("a less or equal than b : %s\n", bool_to_string(buf, s_a <= b));
	printf("a less or equal than b : %s\n", bool_to_string(buf, a <= s_b));
	printf("a less or equal than b : %s\n", bool_to_string(buf, s_a <= s_b));

	printf("a less or equal than a : %s\n", bool_to_string(buf, s_a <= a));
	printf("a less or equal than a : %s\n", bool_to_string(buf, a <= s_a));
	printf("a less or equal than a : %s\n", bool_to_string(buf, s_a <= s_a));

	printf("b less or equal than b : %s\n", bool_to_string(buf, s_b <= b));
	printf("b less or equal than b : %s\n", bool_to_string(buf, b <= s_b));
	printf("b less or equal than b : %s\n", bool_to_string(buf, s_b <= s_b));

	printf("1 less or equal than 2 : %s\n", bool_to_string(buf, s_num_1 <= num_2));
	printf("1 less or equal than 2 : %s\n", bool_to_string(buf, num_1 <= s_num_2));
	printf("1 less or equal than 2 : %s\n", bool_to_string(buf, s_num_1 <= s_num_2));

	printf("999 less or equal than 1000 : %s\n", bool_to_string(buf, s_num_999 <= num_1000));
	printf("999 less or equal than 1000 : %s\n", bool_to_string(buf, num_999 <= s_num_1000));
	printf("999 less or equal than 1000 : %s\n", bool_to_string(buf, s_num_999 <= s_num_1000));

	return 0;
}

static char * bool_to_string(char *s, bool value)
{
	if (s != NULL)
	{
		if (value == false)
		{
			s[0] = 'f';
			s[1] = 'a';
			s[2] = 'l';
			s[3] = 's';
			s[4] = 'e';
			s[5] = '\0';
		}
		else
		{
			s[0] = 't';
			s[1] = 'r';
			s[2] = 'u';
			s[3] = 'e';
			s[4] = '\0';
		}
	}

	return s;
}
