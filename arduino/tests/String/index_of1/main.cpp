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
#include <stdlib.h> /* abort() */
#include <stdio.h>

#include "String.hpp"

static void compare_index_of_with_strchr(const char *sentence, char needle, int result);
static void compare_index_of_with_strstr(const char *sentence, const char *needle, int result);
static void compare_last_index_of_with_strchr(const char *sentence, char needle, int result);
static void compare_last_index_of_with_strstr(const char *sentence, const char *needle, int result);

int main(void)
{
	std::vector<String>     source       = {"this", "is", "a", "test", "sentence"};

	/* Use std::move to transfer resources to 'destination' */
	std::vector<String>     destination  = std::move(source);

	String                  sentence;

	constexpr const char    end_c = '\0';
	constexpr const char   *end = "";

	constexpr const char    letter_t = 't';
	constexpr const char   *letter_e = "e";
	constexpr const char   *word_test = "test";

	constexpr const char   *match_string1 = " this is a test sentence";
	constexpr const char   *match_string2 = "this is a test sentence";

	constexpr const char   *unmatch_string1 = " this is a test1 ";
	constexpr const char   *unmatch_string2 = "this is a test2 ";

	constexpr const char   *long_string = "this is not a test sentence";

	const String            s_end; /* empty string */

	const String            s_letter_t(letter_t);
	const String            s_letter_e(letter_e);
	const String            s_word_test("test");

	const String            s_match_string1(match_string1);
	const String            s_match_string2(match_string2);

	const String            s_unmatch_string1(unmatch_string1);
	const String            s_unmatch_string2(unmatch_string2);

	const String            s_long_string(long_string);

	int                     result;

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += ' ' + value;
	}

	/* print sentence */
	printf("%s\n", sentence.c_str());

	/* search */

	/* c data type  */
	result = (int) sentence.indexOf(end_c);
	printf("indexOf(end_c) : %d\n", result);
	compare_index_of_with_strchr(sentence.c_str(), end_c, result);
	result = (int) sentence.indexOf(end);
	printf("indexOf(end) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), end, result);
	/*puts("SKIP");*/

	result = (int) sentence.indexOf(letter_t);
	printf("indexOf(letter_t) : %d\n", result);
	compare_index_of_with_strchr(sentence.c_str(), letter_t, result);

	result = (int) sentence.indexOf(letter_e);
	printf("indexOf(letter_e) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), letter_e, result);

	result = (int) sentence.indexOf(word_test);
	printf("indexOf(word_test) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), word_test, result);

	result = (int) sentence.indexOf(match_string1);
	printf("indexOf(match_string1) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), match_string1, result);
	result = (int) sentence.indexOf(match_string2);
	printf("indexOf(match_string2) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), match_string2, result);

	result = (int) sentence.indexOf(unmatch_string1);
	printf("indexOf(unmatch_string1) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), unmatch_string1, result);
	result = (int) sentence.indexOf(unmatch_string2);
	printf("indexOf(unmatch_string2) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), unmatch_string2, result);

	result = (int) sentence.indexOf(long_string);
	printf("indexOf(long_string) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), long_string, result);

	/* string data type  */
	result = (int) sentence.indexOf(s_end);
	printf("indexOf(s_end) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_end.c_str(), result);
	/*puts("SKIP");*/

	result = (int) sentence.indexOf(s_letter_t);
	printf("indexOf(s_letter_t) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_letter_t.c_str(), result);

	result = (int) sentence.indexOf(s_letter_e);
	printf("indexOf(s_letter_e) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_letter_e.c_str(), result);

	result = (int) sentence.indexOf(s_word_test);
	printf("indexOf(s_word_test) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_word_test.c_str(), result);

	result = (int) sentence.indexOf(s_match_string1);
	printf("indexOf(s_match_string1) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_match_string1.c_str(), result);
	result = (int) sentence.indexOf(s_match_string2);
	printf("indexOf(s_match_string2) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_match_string2.c_str(), result);

	result = (int) sentence.indexOf(s_unmatch_string1);
	printf("indexOf(s_unmatch_string1) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_unmatch_string1.c_str(), result);
	result = (int) sentence.indexOf(s_unmatch_string2);
	printf("indexOf(s_unmatch_string2) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_unmatch_string2.c_str(), result);

	result = (int) sentence.indexOf(s_long_string);
	printf("indexOf(s_long_string) : %d\n", result);
	compare_index_of_with_strstr(sentence.c_str(), s_long_string.c_str(), result);


	/* c data type  */
	result = (int) sentence.lastIndexOf(end_c);
	printf("lastIndexOf(end_c) : %d\n", result);
	compare_last_index_of_with_strchr(sentence.c_str(), end_c, result);
	result = (int) sentence.lastIndexOf(end);
	printf("lastIndexOf(end) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), end, result);

	result = (int) sentence.lastIndexOf(letter_t);
	printf("lastIndexOf(letter_t) : %d\n", result);
	compare_last_index_of_with_strchr(sentence.c_str(), letter_t, result);

	result = (int) sentence.lastIndexOf(letter_e);
	printf("lastIndexOf(letter_e) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), letter_e, result);

	result = (int) sentence.lastIndexOf(word_test);
	printf("lastIndexOf(word_test) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), word_test, result);

	result = (int) sentence.lastIndexOf(match_string1);
	printf("lastIndexOf(match_string1) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), match_string1, result);
	result = (int) sentence.lastIndexOf(match_string2);
	printf("lastIndexOf(match_string2) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), match_string2, result);

	result = (int) sentence.lastIndexOf(unmatch_string1);
	printf("lastIndexOf(unmatch_string1) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), unmatch_string1, result);
	result = (int) sentence.lastIndexOf(unmatch_string2);
	printf("lastIndexOf(unmatch_string2) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), unmatch_string2, result);

	result = (int) sentence.lastIndexOf(long_string);
	printf("lastIndexOf(long_string) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), long_string, result);

	/* string data type  */
	result = (int) sentence.lastIndexOf(s_end);
	printf("lastIndexOf(s_end) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_end.c_str(), result);

	result = (int) sentence.lastIndexOf(s_letter_t);
	printf("lastIndexOf(s_letter_t) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_letter_t.c_str(), result);

	result = (int) sentence.lastIndexOf(s_letter_e);
	printf("lastIndexOf(s_letter_e) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_letter_e.c_str(), result);

	result = (int) sentence.lastIndexOf(s_word_test);
	printf("lastIndexOf(s_word_test) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_word_test.c_str(), result);

	result = (int) sentence.lastIndexOf(s_match_string1);
	printf("lastIndexOf(s_match_string1) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_match_string1.c_str(), result);
	result = (int) sentence.lastIndexOf(s_match_string2);
	printf("lastIndexOf(s_match_string2) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_match_string2.c_str(), result);

	result = (int) sentence.lastIndexOf(s_unmatch_string1);
	printf("lastIndexOf(s_unmatch_string1) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_unmatch_string1.c_str(), result);
	result = (int) sentence.lastIndexOf(s_unmatch_string2);
	printf("lastIndexOf(s_unmatch_string2) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_unmatch_string2.c_str(), result);

	result = (int) sentence.lastIndexOf(s_long_string);
	printf("lastIndexOf(s_long_string) : %d\n", result);
	compare_last_index_of_with_strstr(sentence.c_str(), s_long_string.c_str(), result);


	return 0;
}

static void compare_index_of_with_strchr(const char *sentence, char needle, int result)
{
	const char *p = strchr(sentence, needle);
	if (p == NULL)
	{
		if (result == (-1))
		{
			puts("PASS");
		}
		else
		{
			puts("FAILED");
			abort();
		}
	}
	else
	{
		if (result == ((int) (p - sentence)))
		{
			puts("PASS");
		}
		else
		{
			puts("FAILED");
			abort();
		}
	}
}

static void compare_index_of_with_strstr(const char *sentence, const char *needle, int result)
{
	const char *p = strstr(sentence, needle);
	if (p == NULL)
	{
		if (result == (-1))
		{
			puts("PASS");
		}
		else
		{
			puts("FAILED");
			abort();
		}
	}
	else
	{
		/**
			* p is not null, but here is the different
			* by standard, strstr may return haystack (which is not null)
			* if needle is zero-length string
			*
			* but our api will return haystacklen when needle is empty string
			* considering the different in behaviour, we have to check needle
			*/
		if      ((needle    == NULL) && (result == (-1)))
		{
			/* when needle NULL, result is always -1 */
			puts("PASS");
		}
		else if ((needle[0] == '\0') && (result == ((int) strlen(sentence))))
		{
			/* when needle empty string, return length of string */
			puts("PASS");
		}
		else if (result == ((int) (p - sentence)))
		{
			/* when needle is not null and not empty string, result should match with strstr return */
			puts("PASS");
		}
		else
		{
			/* other condition means failure */
			puts("FAILED");
			abort();
		}
	}
}


static void compare_last_index_of_with_strchr(const char *sentence, char needle, int result)
{
	const char *p = strrchr(sentence, needle);
	if (p == NULL)
	{
		if (result == (-1))
		{
			puts("PASS");
		}
		else
		{
			puts("FAILED");
			abort();
		}
	}
	else
	{
		if (result == ((int) (p - sentence)))
		{
			puts("PASS");
		}
		else
		{
			puts("FAILED");
			abort();
		}
	}
}

static void compare_last_index_of_with_strstr(const char *sentence, const char *needle, int result)
{
	if ((*needle) == '\0')
	{
		/**
			* try to strstr empty string
			* return end of string
			*/
		if (result == ((int) strlen(sentence)))
		{
			puts("PASS");
		}
		else
		{
			puts("FAILED");
			abort();
		}
	}
	else
	{
		const char *last_p;
		const char *p = NULL;

		const char * const original_sentence_pointer = sentence;

		const char *next_sentence = sentence;

		do
		{
			last_p = p;
			sentence = next_sentence;
		}
		while (p = strstr(sentence, needle), (p != NULL) ? (next_sentence = p + 1, true) : false);

		if (last_p == NULL)
		{
			if (result == (-1))
			{
				puts("PASS");
			}
			else
			{
				puts("FAILED");
				abort();
			}
		}
		else
		{
			if (result == ((int) (last_p - original_sentence_pointer)))
			{
				puts("PASS");
			}
			else
			{
				puts("FAILED");
				abort();
			}
		}
	}
}
