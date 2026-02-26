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

	char                    c;

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += ' ' + value;
	}

	/* print sentence */
	printf("%s\r\n", sentence.c_str());
	printf("string length: %u\r\n", sentence.length());



	/* change at index 24 (zero termination) */
	sentence[24] = '\0';

	printf("%s\r\n", sentence.c_str());
	printf("string length: %u\r\n", sentence.length());

	/* change at index 24 (zero termination) with any character */
	c = (sentence[24] = 'a');

	printf("%c: \"%s\"\r\n", c, sentence.c_str());
	printf("string length: %u\r\n", sentence.length());

	/* change at index 23 (zero termination) */
	sentence[23] = '\0';

	printf("%s\r\n", sentence.c_str());
	printf("string length: %u\r\n", sentence.length());

	/* change at index 23 (zero termination) with any character */
	c = (sentence[23] = 'b');

	printf("%c: \"%s\"\r\n", c, sentence.c_str());
	printf("string length: %u\r\n", sentence.length());

	/* change at index 10 (zero termination) */
	sentence[10] = '\0';

	printf("%s\r\n", sentence.c_str());
	printf("string length: %u\r\n", sentence.length());

	/* change at index 10 (zero termination) with any character */
	c = (sentence[10] = 'c');

	printf("%c: \"%s\"\r\n", c, sentence.c_str());
	printf("string length: %u\r\n", sentence.length());

	return 0;
}
