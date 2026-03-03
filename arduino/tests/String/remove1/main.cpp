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

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += ' ' + value;
	}

	/* print sentence */
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());

	sentence.remove(10, 7);
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());

	sentence.remove(40);
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());

	sentence.remove(15);
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());

	sentence.remove(0, 3);
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());

	sentence.remove(0);
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());

	return 0;
}
