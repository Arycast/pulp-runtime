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

#include <string.h>
#include <stdio.h>

#include "String.hpp"

int main(void)
{
	std::vector<String>     source       = {"this", "is", "a", "test", "sentence"};

	/* Use std::move to transfer resources to 'destination' */
	std::vector<String>     destination  = std::move(source);

	String                  sentence;

	char                    substring1;
	char                    substring2[10];
	char                    substring3[20];
	char                    substring4[30];

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += (' ' + value);
	}


	/* print sentence */
	sentence.toCharArray(&substring1, 1);
	printf("\"%c\"\n", substring1);
	sentence.toCharArray(substring2, 10);
	printf("\"%s\" (%lu)\n", substring2, (unsigned long) strlen(substring2));
	sentence.toCharArray(substring3, 20);
	printf("\"%s\" (%lu)\n", substring3, (unsigned long) strlen(substring3));
	sentence.toCharArray(substring4, 30);
	printf("\"%s\" (%lu)\n", substring4, (unsigned long) strlen(substring4));

	return 0;
}
