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
		sentence.concat(" " + value);
	}

	/* print sentence */
	printf(" \" this is a test sentence\" : %d\n", sentence.compareTo(" this is a test sentence"));
	printf(" \" THIS IS A TEST SENTENCE\" : %d\n", sentence.compareToIgnoreCase(" THIS IS A TEST SENTENCE"));

	printf("                       NULL : %d\n", sentence.compareTo(NULL));
	printf("                       NULL : %d\n", sentence.compareToIgnoreCase(NULL));

	printf("                     \"test\" : %d\n", sentence.compareTo("test"));
	printf("                     \"TEST\" : %d\n", sentence.compareToIgnoreCase("TEST"));

	printf("                    \" this\" : %d\n", sentence.compareTo(" this"));
	printf("                    \" THIS\" : %d\n", sentence.compareToIgnoreCase(" THIS"));

	printf("                         \"\" : %d\n", sentence.compareTo(""));
	printf("                         \"\" : %d\n", sentence.compareToIgnoreCase(""));

	printf("\" this is a test sentence2\" : %d\n", sentence.compareTo(" this is a test sentence2"));
	printf("\" THIS IS A TEST SENTENCE2\" : %d\n", sentence.compareToIgnoreCase(" THIS IS A TEST SENTENCE2"));

	return 0;
}
