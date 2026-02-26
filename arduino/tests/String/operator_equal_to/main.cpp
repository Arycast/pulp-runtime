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

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "String.hpp"

static char * bool_to_string(char *s, bool value);

int main(void)
{
	constexpr const char sentence1_string[] = " this is a test sentence";
	constexpr const char sentence2_string[] = "this is a test sentence";

	std::vector<String>     source       = {"this", "is", "a", "test", "sentence"};

	/* Use std::move to transfer resources to 'destination' */
	std::vector<String>     destination  = std::move(source);

	String                  sentence;
	String                  sentence1(sentence1_string); /* control */
	String                  sentence2(sentence2_string); /* control */

	char                    buf[10];

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += ' ' + value;
	}

	/* print sentence */
	printf("%s\r\n", sentence.c_str());
	printf("string length: %u\r\n", sentence.length());


	/* compare equal */
	printf("compare equal with sentence1: %s\n", bool_to_string(buf, sentence == sentence1));
	printf("compare equal with sentence2: %s\n", bool_to_string(buf, sentence == sentence2));

	printf("compare equal with rhs char1: %s\n", bool_to_string(buf, sentence == sentence1_string));
	printf("compare equal with rhs char2: %s\n", bool_to_string(buf, sentence == sentence2_string));

	printf("compare equal with lhs char1: %s\n", bool_to_string(buf, sentence1_string == sentence));
	printf("compare equal with lhs char2: %s\n", bool_to_string(buf, sentence2_string == sentence));

	printf("self compare equal sentence: %s\n", bool_to_string(buf, sentence == sentence));
	printf("self compare equal sentence1: %s\n", bool_to_string(buf, sentence1 == sentence1));
	printf("self compare equal sentence2: %s\n", bool_to_string(buf, sentence2 == sentence2));

	/* compare not equal */
	printf("compare not equal with sentence1: %s\n", bool_to_string(buf, sentence != sentence1));
	printf("compare not equal with sentence2: %s\n", bool_to_string(buf, sentence != sentence2));

	printf("compare not equal with rhs char1: %s\n", bool_to_string(buf, sentence != sentence1_string));
	printf("compare not equal with rhs char2: %s\n", bool_to_string(buf, sentence != sentence2_string));

	printf("compare not equal with lhs char1: %s\n", bool_to_string(buf, sentence1_string != sentence));
	printf("compare not equal with lhs char2: %s\n", bool_to_string(buf, sentence2_string != sentence));

	printf("self compare not equal sentence: %s\n", bool_to_string(buf, sentence != sentence));
	printf("self compare not equal sentence1: %s\n", bool_to_string(buf, sentence1 != sentence1));
	printf("self compare not equal sentence2: %s\n", bool_to_string(buf, sentence2 != sentence2));

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
