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
	constexpr const char *start1 = "this";
	constexpr const char *start2 = " this";
	constexpr const char *start3 = " this is a test sentence";
	constexpr const char *start4 = "";
	constexpr const char *start5 = NULL;

	constexpr const char *end1 = "sentence";
	constexpr const char *end2 = " sentence";
	constexpr const char *end3 = " sentence ";
	constexpr const char *end4 = " this is a test  sentence ";
	constexpr const char *end5 = "";
	constexpr const char *end6 = NULL;

	std::vector<String>     source       = {"this", "is", "a", "test", "sentence"};

	/* Use std::move to transfer resources to 'destination' */
	std::vector<String>     destination  = std::move(source);

	String                  sentence;

	String                  s_start1(start1);
	String                  s_start2(start2);
	String                  s_start3(start3);
	String                  s_start4(start4);
	String                  s_start5;

	String                  s_end1(end1);
	String                  s_end2(end2);
	String                  s_end3(end3);
	String                  s_end4(end4);
	String                  s_end5(end5);
	String                  s_end6;

	char                    buf[8];

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence += (" " + value);
	}


	/* test */
	printf("vector: \"%s\"\r\n", sentence.c_str());

	printf("test start with c \"%s\" : \"%s\"\r\n", (start1 == NULL) ? "NULL" : start1, bool_to_string(buf, sentence.startsWith(start1)));
	printf("test start with c \"%s\" : \"%s\"\r\n", (start2 == NULL) ? "NULL" : start2, bool_to_string(buf, sentence.startsWith(start2)));
	printf("test start with c \"%s\" : \"%s\"\r\n", (start3 == NULL) ? "NULL" : start3, bool_to_string(buf, sentence.startsWith(start3)));
	printf("test start with c \"%s\" : \"%s\"\r\n", (start4 == NULL) ? "NULL" : start4, bool_to_string(buf, sentence.startsWith(start4)));
	printf("test start with c \"%s\" : \"%s\"\r\n", (start5 == NULL) ? "NULL" : start5, bool_to_string(buf, sentence.startsWith(start5)));
	printf("test start with string \"%s\" : \"%s\"\r\n", s_start1.c_str(), bool_to_string(buf, sentence.startsWith(s_start1)));
	printf("test start with string \"%s\" : \"%s\"\r\n", s_start2.c_str(), bool_to_string(buf, sentence.startsWith(s_start2)));
	printf("test start with string \"%s\" : \"%s\"\r\n", s_start3.c_str(), bool_to_string(buf, sentence.startsWith(s_start3)));
	printf("test start with string \"%s\" : \"%s\"\r\n", s_start4.c_str(), bool_to_string(buf, sentence.startsWith(s_start4)));
	printf("test start with string \"%s\" : \"%s\"\r\n", s_start5.c_str(), bool_to_string(buf, sentence.startsWith(s_start5)));

	printf("test end with c \"%s\" : \"%s\"\r\n", (end1 == NULL) ? "NULL" : end1, bool_to_string(buf, sentence.endsWith(end1)));
	printf("test end with c \"%s\" : \"%s\"\r\n", (end2 == NULL) ? "NULL" : end2, bool_to_string(buf, sentence.endsWith(end2)));
	printf("test end with c \"%s\" : \"%s\"\r\n", (end3 == NULL) ? "NULL" : end3, bool_to_string(buf, sentence.endsWith(end3)));
	printf("test end with c \"%s\" : \"%s\"\r\n", (end4 == NULL) ? "NULL" : end4, bool_to_string(buf, sentence.endsWith(end4)));
	printf("test end with c \"%s\" : \"%s\"\r\n", (end5 == NULL) ? "NULL" : end5, bool_to_string(buf, sentence.endsWith(end5)));
	printf("test end with c \"%s\" : \"%s\"\r\n", (end6 == NULL) ? "NULL" : end6, bool_to_string(buf, sentence.endsWith(end6)));
	printf("test end with string \"%s\" : \"%s\"\r\n", s_end1.c_str(), bool_to_string(buf, sentence.endsWith(s_end1)));
	printf("test end with string \"%s\" : \"%s\"\r\n", s_end2.c_str(), bool_to_string(buf, sentence.endsWith(s_end2)));
	printf("test end with string \"%s\" : \"%s\"\r\n", s_end3.c_str(), bool_to_string(buf, sentence.endsWith(s_end3)));
	printf("test end with string \"%s\" : \"%s\"\r\n", s_end4.c_str(), bool_to_string(buf, sentence.endsWith(s_end4)));
	printf("test end with string \"%s\" : \"%s\"\r\n", s_end5.c_str(), bool_to_string(buf, sentence.endsWith(s_end5)));
	printf("test end with string \"%s\" : \"%s\"\r\n", s_end6.c_str(), bool_to_string(buf, sentence.endsWith(s_end6)));

	/* try substring */
	printf("substring test 1 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(1).startsWith("this")));
	printf("substring test 2 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(2).startsWith("this")));
	printf("substring test 3 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(3).startsWith("this")));
	printf("substring test 4 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(4).startsWith("this")));
	printf("substring test 5 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(5).startsWith("this")));
	printf("substring test 6 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(6).startsWith("this")));
	printf("substring test 7 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(7).startsWith("this")));
	printf("substring test 8 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(8).startsWith("this")));
	printf("substring test 9 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(9).startsWith("this")));
	printf("substring test 10 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(10).startsWith("this")));

	printf("substring test 1 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(1, 5).endsWith("is")));
	printf("substring test 2 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(2, 6).endsWith("is")));
	printf("substring test 3 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(3, 7).endsWith("is")));
	printf("substring test 4 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(4, 8).endsWith("is")));
	printf("substring test 5 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(5, 9).endsWith("is")));
	printf("substring test 6 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(6, 10).endsWith("is")));
	printf("substring test 7 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(7, 11).endsWith("is")));
	printf("substring test 8 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(8, 12).endsWith("is")));
	printf("substring test 9 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(9, 13).endsWith("is")));
	printf("substring test 10 : \"%s\"\r\n", bool_to_string(buf, sentence.substring(10, 14).endsWith("is")));

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
