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
#include <stdlib.h>
#include <stdio.h>

#include "String.hpp"

static void verify_replacement_string_length(const char *string, unsigned int string_length);

int main(void)
{
	std::vector<String>     source       = {"this", "is", "a", "test", "sentence"};

	/* Use std::move to transfer resources to 'destination' */
	std::vector<String>     destination  = std::move(source);

	String                  sentence;

	const String            empty;

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += ' ' + value;
	}

	/* print sentence */
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());

	sentence.replace("this", String("that"));
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());
	verify_replacement_string_length(sentence.c_str(), sentence.length());


	sentence.replace(String("s is"), "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());
	verify_replacement_string_length(sentence.c_str(), sentence.length());

	sentence.replace(String("t is"), "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());
	verify_replacement_string_length(sentence.c_str(), sentence.length());

	sentence.replace(String("aaaaaaaa"), String("bb"));
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());
	verify_replacement_string_length(sentence.c_str(), sentence.length());

	sentence.replace("aa", empty);
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());
	verify_replacement_string_length(sentence.c_str(), sentence.length());

	sentence.replace("bbbb", empty);
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());
	verify_replacement_string_length(sentence.c_str(), sentence.length());

	sentence.replace(empty, "ccc");
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());
	verify_replacement_string_length(sentence.c_str(), sentence.length());

	sentence.replace(empty, empty);
	printf("(%u) \"%s\"\n", sentence.length(), sentence.c_str());
	verify_replacement_string_length(sentence.c_str(), sentence.length());

	return 0;
}

static void verify_replacement_string_length(const char *string, unsigned int string_length)
{
	size_t len = strlen(string);
	if (((size_t) string_length) == len)
	{
		puts("PASS");
	}
	else
	{
		puts("FAILED");
		abort();
	}
}
