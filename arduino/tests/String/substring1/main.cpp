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
		sentence += (" " + value);
	}


	/* print sentence */
	printf("%s\r\n", sentence.substring(10).c_str());
	printf("%s\r\n", sentence.substring(20).c_str());
	printf("%s\r\n", sentence.substring(30).c_str());

	printf("%s\r\n", sentence.substring(10, 16).c_str());
	printf("%s\r\n", sentence.substring(20, 26).c_str());
	printf("%s\r\n", sentence.substring(30, 36).c_str());

	return 0;
}
