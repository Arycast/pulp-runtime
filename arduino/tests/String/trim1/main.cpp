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

	String                  sentence1;
	String                  sentence2("   Line one is messy   \n" \
		" \t Line two has tabs \n" \
		"Final line with spaces   ");

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence1 += ' ' + value;
	}

	/* print sentence */
	printf("before trim: (%u) \"%s\"\n", sentence1.length(), sentence1.c_str());

	sentence1.trim();
	printf(" after trim: (%u) \"%s\"\n", sentence1.length(), sentence1.c_str());


	/* print sentence */
	printf("before trim: (%u) \"%s\"\n", sentence2.length(), sentence2.c_str());

	sentence2.trim();
	printf(" after trim: (%u) \"%s\"\n", sentence2.length(), sentence2.c_str());

	return 0;
}
