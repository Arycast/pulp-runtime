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

#include <assert.h>

#include "String.hpp"

int main(void)
{
	std::vector<String>     source       = {"this", "is", "a", "test", "sentence"};

	/* Use std::move to transfer resources to 'destination' */
	std::vector<String>     destination  = std::move(source);

	String                  sentence;

	bool                    retval;

	byte                    value_byte          = 100;
	int                     value_int           = -1911221;
	unsigned int            value_unsigned_int  = 2000000;
	long                    value_long          = -1800000000l;
	unsigned long           value_unsigned_long = 99999999999ul;
	float                   value_float         = 1222.33311f;
	double                  value_double        = -21.659999; /* default is double */

	/* add word into sentence */
	for (const String& value : destination)
	{
		sentence.concat(" " + value);
	}


	retval = sentence.concat(' ');
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(value_byte);
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(' ');
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(value_int);
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(' ');
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(value_unsigned_int);
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(' ');
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(value_long);
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(' ');
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(value_unsigned_long);
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(' ');
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(value_float);
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(' ');
	assert(retval == true); /* assume alloc success */
	retval = sentence.concat(value_double);
	assert(retval == true); /* assume alloc success */

	/* print sentence */
	printf("%s\n", sentence.c_str());

	return 0;
}
