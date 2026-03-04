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
	char                    c3;

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += ' ' + value;
	}

	/* print sentence */
	printf("%s\n", sentence.c_str());
	printf("string length: %u\n", sentence.length());



	/* (corner case) change at index 25 (outside of buffer) */
	c = sentence[25]++;
	printf("before increment (25): %d (%u)\n", (int) c, sentence.length());
	printf(" after increment (25): %d (%u)\n", (int) sentence[25], sentence.length());

	/* (corner case) change at index 24 (outside of string/at zero termination) */
	c = sentence[24]++;
	printf("before increment (24): %d (%u)\n", (int) c, sentence.length());
	printf(" after increment (24): %d (%u)\n", (int) sentence[24], sentence.length());

	/* change at index 23 */
	c = sentence[23]--;
	printf("before decrement (23): %c (%u)\n", c, sentence.length());
	printf(" after decrement (23): %c (%u)\n", (char) sentence[23], sentence.length());

	/* print sentence again */

	printf("%s\n", sentence.c_str());
	printf("string length: %u\n", sentence.length());

	/* save c3 */
	c3 = sentence[3];

	/* (corner case) change at index 3 to '\0' */
	sentence[3] = '\0';

	/* before increment */
	printf(" current: %s (%u)\n", sentence.c_str(), sentence.length());

	/* increment */
	c = sentence[3]++;

	printf("before increment (3): %c (%u)\n", c, sentence.length());
	printf(" after increment (3): %d (%u)\n", (int) sentence[3], sentence.length());


	/* return c3 */
	sentence[3] = c3;

	printf(" recovery: %s (%u)\n", sentence.c_str(), sentence.length());


	/* (corner case) change at index 25 */
	c = --sentence[25];
	printf("before decrement (25): %d (%u)\n", (int) c, sentence.length());
	printf(" after decrement (25): %d (%u)\n", (int) sentence[25], sentence.length());

	/* (corner case) change at index 24 */
	c = --sentence[24];
	printf("before decrement (24): %d (%u)\n", (int) c, sentence.length());
	printf(" after decrement (24): %d (%u)\n", (int) sentence[24], sentence.length());

	/* change at index 23 */
	c = ++sentence[23];
	printf("before increment (23): %c (%u)\n", c, sentence.length());
	printf(" after increment (23): %c (%u)\n", (char) sentence[23], sentence.length());

	/* print sentence again */

	printf("%s\n", sentence.c_str());
	printf("string length: %u\n", sentence.length());

	return 0;
}
