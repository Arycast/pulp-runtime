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
	String test1;
	String test2(nullptr);
	String test3("this is test 3");

	char c;

	/* print sentence */
	if (c = test1[2], c == '\0')
	{
		printf("test1: \\0\n");
	}
	else
	{
		printf("test1: %c\n", c);
	}

	if (c = test2[2], c == '\0')
	{
		printf("test2: \\0\n");
	}
	else
	{
		printf("test2: %c\n", c);
	}

	if (c = test3[2], c == '\0')
	{
		printf("test3: \\0\n");
	}
	else
	{
		printf("test3: %c\n", c);
	}



	test1[6] = 'A';
	if (c = test1[2], c == '\0')
	{
		printf("test1 (A): \\0\n");
	}
	else
	{
		printf("test1 (A): %c\n", c);
	}


	printf("test1: %c\n", test1.charAt(4));
	printf("test2: %c\n", test2.charAt(4));
	printf("test3: %c\n", test3.charAt(4));

	test1[8] = 'B';
	printf("test1 (B): %c\n", test1.charAt(8));

	return 0;
}
