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
	std::vector<String>     source       = {"THIS", "IS", "A", "TEST", "SENTENCE"};

	/* Use std::move to transfer resources to 'destination' */
	std::vector<String>     destination  = std::move(source);

	String                  sentence1;
	String                  sentence2 = "test all lower";
	String                  sentence3 = "TeSt MiXeD CAsE";

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence1 += ' ' + value;
	}

	/* print sentence */

	printf("before lower: %s\n", sentence1.c_str());
	sentence1.toLowerCase();
	printf(" after lower: %s\n", sentence1.c_str());

	printf("before lower: %s\n", sentence2.c_str());
	sentence2.toLowerCase();
	printf(" after lower: %s\n", sentence2.c_str());

	printf("before lower: %s\n", sentence3.c_str());
	sentence3.toLowerCase();
	printf(" after lower: %s\n", sentence3.c_str());

	return 0;
}
