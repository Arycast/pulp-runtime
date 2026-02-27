/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/**
	* configuration
	*/
#include "config_String.h"

/**
	* assert
	*/
#include "assert_helper.h"

/* standard header c */
#include <stddef.h>
#include <string.h> /* strlen, memcpy, memmove etc. */
#include <stdlib.h> /* alloc related functions */
#include <stdio.h> /* snprintf */

/* standard header c++ */
#include <exception> /* for std::terminate */
#include <algorithm> /* for std::reverse */

/* local header */
#include "String.hpp"

/* when do simulation test, include simulation header */
#ifdef SIMULATION_TEST
#include "simulation_test.h"
#endif

/**
	* helper to convert integer to string
	* return number of character written to string str NOT INCLUDING terminating 0 ('\0')
	* so if return value is same as argument str_len value, then '\0' is not written
	* which is indicate error
	*/
template <typename T>
static size_t unsigned_integer_to_string(char *str, size_t str_len, T value, unsigned int base = DEC)
{
#if ((SSTRING_CONF_BASE_UPPERCASE) != 0)
	constexpr const char base_char_map[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
#else
	constexpr const char base_char_map[] = "0123456789abcdefghijklmnopqrstuvwxyz";
#endif

	constexpr T   zero = 0;

	const     T  _base = static_cast<T>(base);

	size_t len = 0;

	/**
		* if base is bigger than 36 (10 number digit + 26 alphabet)
		* then we can't convert it to string
		*
		* if not enough space to write in buffer, also don't write anything
		*/
	if ((base <= 1) || (base > 36) || (str == NULL) || (str_len <= 1))
	{
		return 0;
	}

	/* convert value to string */
	if (value == zero)
	{
		/* len guaranteed to be 2 or more */
		str[len++] = '0';
	}
	else
	{
		do
		{
			/**
				* fill string buffer
				* (MODULO IS AN EXPENSIVE OPERATION!)
				*/
			str[len++] = base_char_map[value % _base];

			/* reduce value (THIS IS AN EXPENSIVE OPERATION!) */
			value /= _base;
		}
		while ((value > zero) && (len < str_len));
	}

	/* Reverse the string in place */
	std::reverse(str, str + len);

	/* add termination string */
	if (len < str_len)
	{
		str[len] = '\0';
	}

	return len;
}

/* for testing only, export convertion function */
#ifdef SIMULATION_TEST
size_t unsigned_char_to_string_export(char *str, size_t str_len, unsigned char value, unsigned int base = DEC)
{
	return unsigned_integer_to_string<unsigned char>(str, str_len, value, base);
}

size_t int_to_string_export(char *str, size_t str_len, int value, unsigned int base = DEC)
{
	bool add_minus_sign = false;
	if ((value < 0) && (base == DEC))
	{
		str_len--;
		*str++ = '-';
		value *= (-1);
		add_minus_sign = true;
	}
	return unsigned_integer_to_string<unsigned int>(str, str_len, (unsigned int) value, base) + (add_minus_sign ? 1u : 0u);
}

size_t unsigned_int_to_string_export(char *str, size_t str_len, unsigned int value, unsigned int base = DEC)
{
	return unsigned_integer_to_string<unsigned int>(str, str_len, value, base);
}

size_t long_to_string_export(char *str, size_t str_len, long value, unsigned int base = DEC)
{
	bool add_minus_sign = false;
	if ((value < 0) && (base == DEC))
	{
		str_len--;
		*str++ = '-';
		value *= (-1);
		add_minus_sign = true;
	}
	return unsigned_integer_to_string<unsigned long>(str, str_len, (unsigned long) value, base) + (add_minus_sign ? 1u : 0u);
}

size_t unsigned_long_to_string_export(char *str, size_t str_len, unsigned long value, unsigned int base = DEC)
{
	return unsigned_integer_to_string<unsigned long>(str, str_len, value, base);
}
#endif


/**
	* assume buf_len > 1
	* assume base > 1
	*/
template <typename T>
static bool unsigned_integer_to_dynamic_string(char **buf, size_t buf_len, size_t *str_len, T value, unsigned int base = DEC, bool add_minus_sign = false)
{
	char    *_buf;
	size_t    retval;

	/**
		* byte should be able to contain 8 bit at max + 1 string termination character
		* add more 1 extra sign character
		*/
	_buf = (char *) malloc(((add_minus_sign) ? (buf_len + 1) : buf_len) * sizeof(char));
	if (_buf == NULL)
	{
		return false; /* return fail */
	}

	/* fill string */
	retval = unsigned_integer_to_string<T>((add_minus_sign) ? (_buf + 1) : _buf, buf_len, value, base);
	if ((retval <= 0) || (retval >= buf_len))
	{
		/* allocated buffer is too small */
		free(_buf); /* free buffer again */
		return false; /* return fail */
	}

	/* success, add minus sign */
	if (add_minus_sign)
	{
		*_buf = '-';
	}

	/* set output into buffer */
	*buf = _buf;
	*str_len = (add_minus_sign) ? (retval + 1) : retval;
	return true;
}





/**
	* static member
	*/
/*static constexpr const char  *String::empty_string = "";*/
/*char                   String::dummy_char_storage = '\0';*/ /* init 0 so we don't waste .data and valgrind don't scold up */

/* constructor */

String::String(void)
{
	this->__non_standard__set_new_buffer(NULL, 0);
}

/* without move semantic (copy semantic) */
String::String(const String  &s)
{
	/**
		* collect buffer information
		*/

	const char   *rvalue_c_buf         = s.c_str();
	size_t        rvalue_buffer_length = s.__non_standard__get_buffer_length();
	size_t        rvalue_string_length = s.__non_standard__get_string_length();

	if ((rvalue_c_buf == NULL) || (rvalue_c_buf == (String::empty_string)) ||
		(rvalue_buffer_length == 0) || (rvalue_string_length == 0))
	{
		this->__non_standard__set_new_buffer(NULL, 0);
	}
	else
	{
		char *new_string;

		_ASSERT((rvalue_buffer_length > 0) && (rvalue_string_length < rvalue_buffer_length));

		/* allocate based on buffer length */
		new_string = (char *) malloc(rvalue_buffer_length * sizeof(char));
		if (new_string == NULL)
		{
			/* try again but with smaller buffer */
			new_string = (char *) malloc((rvalue_string_length + 1) * sizeof(char));
			if (new_string == NULL)
			{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
				/* unhandled exception, terminate */
				std::terminate(); /* this function is non-return function */
#else
				this->__non_standard__set_new_buffer(NULL, 0);
#endif
				return;
			}
			else
			{
				rvalue_buffer_length = rvalue_string_length + 1;
			}
		}
		else
		{
			rvalue_buffer_length = rvalue_buffer_length;
		}

		/* copy data from rvalue */
		memcpy(new_string, rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

		this->__non_standard__set_new_buffer(new_string, rvalue_buffer_length, rvalue_string_length);
	}
}

/* with move semantic */
#if ((__cplusplus) >= 201103L)
String::String(String       &&s)
{
	/* steal buffer */
	this->__non_standard__set_new_buffer(
		s.__non_standard__c_str_non_const(),
		s.__non_standard__get_buffer_length(),
		s.__non_standard__get_string_length()
	);

	s.__non_standard__invalidate_string();
}
#endif


/* string */
String::String(const char    *s)
{
	size_t        buffer_length;
	size_t        string_length;

	if ((s == NULL) || (string_length = strlen(s), string_length == 0))
	{
		this->__non_standard__set_new_buffer(NULL, 0);
	}
	else
	{
		char *new_string;

		buffer_length = string_length + 1;

		_ASSERT((buffer_length > 0) && (string_length < buffer_length));

		/* allocate based on buffer length */
		new_string = (char *) malloc(buffer_length * sizeof(char));
		if (new_string == NULL)
		{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
			/* unhandled exception, terminate */
			std::terminate(); /* this function is non-return function */
#else
			this->__non_standard__set_new_buffer(NULL, 0);
#endif
			return;
		}

		/* copy data from rvalue */
		memcpy(new_string, s, (string_length + 1) * sizeof(char));

		this->__non_standard__set_new_buffer(new_string, buffer_length, string_length);
	}
}

String::String(char          value)
{
	if (value == '\0')
	{
		/* empty string */
		this->__non_standard__set_new_buffer(NULL, 0);
	}
	else
	{
		/* 2 char for value and terminating string */
		char *new_string = (char *) malloc(2 * sizeof(char));
		if (new_string == NULL)
		{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
			/* unhandled exception, terminate */
			std::terminate(); /* this function is non-return function */
#else
			this->__non_standard__set_new_buffer(NULL, 0);
#endif
			return;
		}

		/* fill string */
		new_string[0] = value;
		new_string[1] = '\0';

		/* put into buffer */
		this->__non_standard__set_new_buffer(new_string, 2, 1);
	}
}

String::String(byte          value) : String(value, DEC)
{
}

String::String(byte          value, unsigned int base)
{
	char    *new_string = NULL;
	bool     retval;

	size_t string_length = 8;
	size_t buffer_length = string_length + 1;

	retval = unsigned_integer_to_dynamic_string<byte>(
		&new_string, buffer_length, &string_length,
		value, base, false);
	if (! retval)
	{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
		/* unhandled exception, terminate */
		std::terminate(); /* this function is non-return function */
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	/* allocation success */
	_ASSERT(new_string != NULL);
	_ASSERT(string_length < buffer_length);

	/* put into buffer */
	this->__non_standard__set_new_buffer(new_string, buffer_length, string_length);
}

String::String(int           value) : String(value, DEC)
{
}

String::String(int           value, unsigned int base)
{
	char    *new_string = NULL;
	bool     retval;

	size_t string_length = 32;
	size_t buffer_length = string_length + 1;

	/**
		* be careful when allocate, if base is DEC,
		* we have possibility to add sign
		*/
	if ((value < 0) && (base == DEC))
	{
		unsigned int u_value = (unsigned int) (value * (-1));
		retval = unsigned_integer_to_dynamic_string<unsigned int>(
			&new_string, buffer_length, &string_length,
			u_value, base, true);

		/* buffer is increase by 1 due to adding sign */
		buffer_length++;
	}
	else
	{
		unsigned int u_value = (unsigned int) value;
		retval = unsigned_integer_to_dynamic_string<unsigned int>(
			&new_string, buffer_length, &string_length,
			u_value, base, false);
	}

	if (! retval)
	{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
		/* unhandled exception, terminate */
		std::terminate(); /* this function is non-return function */
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	/* allocation success */
	_ASSERT(new_string != NULL);
	_ASSERT(string_length < buffer_length);

	/* put into buffer */
	this->__non_standard__set_new_buffer(new_string, buffer_length, string_length);
}

String::String(unsigned int  value) : String(value, DEC)
{
}

String::String(unsigned int  value, unsigned int base)
{
	char    *new_string = NULL;
	bool     retval;

	size_t string_length = 32;
	size_t buffer_length = string_length + 1;

	/* allocate buffer */
	retval = unsigned_integer_to_dynamic_string<unsigned int>(
		&new_string, buffer_length, &string_length,
		value, base, false);

	if (! retval)
	{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
		/* unhandled exception, terminate */
		std::terminate(); /* this function is non-return function */
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	/* allocation success */
	_ASSERT(new_string != NULL);
	_ASSERT(string_length < buffer_length);

	/* put into buffer */
	this->__non_standard__set_new_buffer(new_string, buffer_length, string_length);
}

String::String(long          value) : String(value, DEC)
{
}

String::String(long          value, unsigned int base)
{
	char    *new_string = NULL;
	bool     retval;

	size_t string_length = 64;
	size_t buffer_length = string_length + 1;

	/**
		* be careful when allocate, if base is DEC,
		* we have possibility to add sign
		*/
	if ((value < 0) && (base == DEC))
	{
		unsigned long u_value = (unsigned long) (value * (-1));
		retval = unsigned_integer_to_dynamic_string<unsigned long>(
			&new_string, buffer_length, &string_length,
			u_value, base, true);

		/* buffer is increase by 1 due to adding sign */
		buffer_length++;
	}
	else
	{
		unsigned long u_value = (unsigned long) value;
		retval = unsigned_integer_to_dynamic_string<unsigned long>(
			&new_string, buffer_length, &string_length,
			u_value, base, false);
	}

	if (! retval)
	{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
		/* unhandled exception, terminate */
		std::terminate(); /* this function is non-return function */
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	/* allocation success */
	_ASSERT(new_string != NULL);
	_ASSERT(string_length < buffer_length);

	/* put into buffer */
	this->__non_standard__set_new_buffer(new_string, buffer_length, string_length);
}

String::String(unsigned long value) : String(value, DEC)
{
}

String::String(unsigned long value, unsigned int base)
{
	char    *new_string = NULL;
	bool     retval;

	size_t string_length = 64;
	size_t buffer_length = string_length + 1;

	/* allocate buffer */
	retval = unsigned_integer_to_dynamic_string<unsigned long>(
		&new_string, buffer_length, &string_length,
		value, base, false);

	if (! retval)
	{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
		/* unhandled exception, terminate */
		std::terminate(); /* this function is non-return function */
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	/* allocation success */
	_ASSERT(new_string != NULL);
	_ASSERT(string_length < buffer_length);

	/* put into buffer */
	this->__non_standard__set_new_buffer(new_string, buffer_length, string_length);
}

String::String(float         value) : String((double) value)
{
}

String::String(double        value)
{
	constexpr size_t buffer_length = 40;
	size_t string_length;

	char *new_string = (char *) malloc(buffer_length * sizeof(char));

	int retval;

	if (new_string == NULL)
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	/* convert double value to string */
	retval = snprintf(new_string, buffer_length, "%f", value);
	if ((retval <= 0) || (retval >= ((int) buffer_length)))
	{
		free(new_string); /* free new_string before return error */

#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	string_length = (size_t) retval;

	/* allocation success */
	_ASSERT(new_string != NULL);
	_ASSERT(string_length < buffer_length);

	/* put into buffer */
	this->__non_standard__set_new_buffer(new_string, buffer_length, string_length);
}





/* destructor */

String::~String(void)
{
	this->__non_standard__free_string_non_invalidate();
	/*this->__non_standard__invalidate_string();*/
}






/**
	* operator =
	*/
/**
	* with copy semantic
	*/
String &String::operator=(const String   &rvalue)
{
	/**
		* collect buffer information
		*/

	const char   *rvalue_c_buf         = rvalue.c_str();
	size_t        rvalue_buffer_length = rvalue.__non_standard__get_buffer_length();
	size_t        rvalue_string_length = rvalue.__non_standard__get_string_length();

	/**
		* by specification,
		* if c_buf is NULL than buffer_length must be 0
		* if c_buf is empty_string than string_length must be 0
		*/
	_ASSERT(((rvalue_c_buf == NULL) && (rvalue_buffer_length == 0)) ||
		((                rvalue_c_buf  == (String::empty_string)) && (rvalue_string_length == 0)) ||
		(rvalue_c_buf != NULL));

	/**
		* by specification, string length is always smaller than buffer
		* (this hold even if c_str is NULL or empty_string)
		* if buffer_length == 0, then string_length == 0 MUST true
		* if buffer_length  > 0, then string_length < buffer_length MUST true
		*/
	_ASSERT(((rvalue_buffer_length == 0) && (rvalue_string_length == 0)) ||
		((rvalue_buffer_length > 0) && (rvalue_string_length < rvalue_buffer_length)));


	if      (this == (&rvalue))
	{
		return (*this);
	}
	else if ((rvalue_c_buf == NULL) || (rvalue_c_buf == (String::empty_string)) ||
		(rvalue_buffer_length == 0) || (rvalue_string_length == 0))
	{
		return this->__non_standard__free_string_non_invalidate().__non_standard__invalidate_string();
	}
	else
	{
		/**
			* collect buffer information
			*/

		char    *lvalue_c_buf         = this->__non_standard__c_str_non_const();
		size_t   lvalue_buffer_length = this->__non_standard__get_buffer_length();
		size_t   lvalue_string_length = this->__non_standard__get_string_length();

		char    *new_string;

		/* check lvalue */
		_ASSERT(((lvalue_c_buf == NULL) && (lvalue_buffer_length == 0)) ||
			((((const char *) lvalue_c_buf) == (String::empty_string)) && (lvalue_string_length == 0)) ||
			(lvalue_c_buf != NULL));

		_ASSERT(((lvalue_buffer_length == 0) && (lvalue_string_length == 0)) ||
			((lvalue_buffer_length > 0) && (lvalue_string_length < lvalue_buffer_length)));


		/* create new buffer */
		if ((lvalue_c_buf == NULL) || (lvalue_c_buf == (String::empty_string)))
		{
			new_string = (char *) malloc(rvalue_buffer_length * sizeof(char));
			if (new_string == NULL)
			{
				/* try again but with smaller buffer */
				new_string = (char *) malloc((rvalue_string_length + 1) * sizeof(char));
				if (new_string == NULL)
				{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
					/* unhandled exception, terminate */
					std::terminate(); /* this function is non-return function */
#else
					return (*this); /* return this instance unchanged */
#endif
				}
				else
				{
					lvalue_buffer_length = rvalue_string_length + 1;
				}
			}
			else
			{
				lvalue_buffer_length = rvalue_buffer_length;
			}

			/* copy data from rvalue */
			memcpy(new_string, rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

			lvalue_string_length = rvalue_string_length;
		}
		else
		{
			/* if current buffer smaller than rvalue string */
			if (lvalue_buffer_length < rvalue_string_length)
			{
				/* (try to) realloc as big as rvalue buffer */
				new_string = (char *) realloc(lvalue_c_buf, rvalue_buffer_length * sizeof(char));
				if (new_string == NULL)
				{
					/* try again but with smaller buffer */
					new_string = (char *) realloc(lvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));
					if (new_string == NULL)
					{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return (*this); /* return this instance unchanged */
#endif
					}
					else
					{
						lvalue_buffer_length = rvalue_string_length + 1;
					}
				}
				else
				{
					lvalue_buffer_length = rvalue_buffer_length;
				}
			}
			else
			{
				new_string = lvalue_c_buf;
			}

			/* copy data from rvalue */
			memcpy(new_string, rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

			/* current/old buffer already freed by reallocation */
		}

		/* set new buffer */
		this->__non_standard__set_new_buffer(new_string, lvalue_buffer_length, lvalue_string_length);

		return (*this);
	}
}

/**
	* with move semantic
	*/
#if (__cplusplus >= 201103L) /* C++11 */
/* move assignment operator */
String &String::operator=(String        &&rvalue)
{
	/* steal buffer */
	if (this != (&rvalue))
	{
		this->__non_standard__free_string_non_invalidate().__non_standard__set_new_buffer(
			rvalue.__non_standard__c_str_non_const(),
			rvalue.__non_standard__get_buffer_length(),
			rvalue.__non_standard__get_string_length()
		);

		/* invalidate string */
		rvalue.__non_standard__invalidate_string();
	}

	return (*this);
}
#endif

/**
	* with c string
	*/
String &String::operator=(const char     *rvalue)
{
	char   *s = this->__non_standard__c_str_non_const();
	size_t  rvalue_buffer_length;
	size_t  rvalue_string_length;

	/* if we work with same buffer, just return */
	if (((const char *) s) == rvalue)
	{
		return (*this);
	}

	/* check if we need to allocate buffer */
	if ((rvalue == NULL) ||
		(rvalue_string_length = strlen(rvalue), rvalue_string_length == 0))
	{
		/* check if lvalue has valid buffer */
		if ((s == NULL) || (((const char *) s) == (String::empty_string)) ||
			(this->__non_standard__get_string_length() == 0))
		{
			/* do nothing */
		}
		else
		{
			/* has valid buffer, set string to 0 */
			s[0] = '\0';
		}

		/* set string length 0 */
		this->__non_standard__set_new_buffer(s, this->__non_standard__get_buffer_length(), 0);
	}
	else
	{
		rvalue_buffer_length = rvalue_string_length + 1;

		/* allocate new string */
		s = (char *) malloc(rvalue_buffer_length * sizeof(char));
		if (s == NULL)
		{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
			/* unhandled exception, terminate */
			std::terminate(); /* this function is non-return function */
#else
			return this; /* return this instance unchanged */
#endif
		}

		/* copy string */
		memcpy(s, rvalue, (rvalue_string_length + 1) * sizeof(char));

		/* free current buffer and set new string */
		this->__non_standard__free_string_non_invalidate().__non_standard__set_new_buffer(s, rvalue_buffer_length, rvalue_string_length);
	}

	return (*this);
}

/* with number */
String &String::operator=(char            rvalue)
{
	const char s[] = {rvalue, '\0'};
	return ((*this) = s);
}

String &String::operator=(byte            rvalue)
{
	String r(rvalue);
#if ((__cplusplus) >= 201103L)
	return ((*this) = std::move(r));
#else
	return ((*this) = r);
#endif
}

String &String::operator=(int             rvalue)
{
	String r(rvalue);
#if ((__cplusplus) >= 201103L)
	return ((*this) = std::move(r));
#else
	return ((*this) = r);
#endif
}

String &String::operator=(unsigned int    rvalue)
{
	String r(rvalue);
#if ((__cplusplus) >= 201103L)
	return ((*this) = std::move(r));
#else
	return ((*this) = r);
#endif
}

String &String::operator=(long            rvalue)
{
	String r(rvalue);
#if ((__cplusplus) >= 201103L)
	return ((*this) = std::move(r));
#else
	return ((*this) = r);
#endif
}

String &String::operator=(unsigned long   rvalue)
{
	String r(rvalue);
#if ((__cplusplus) >= 201103L)
	return ((*this) = std::move(r));
#else
	return ((*this) = r);
#endif
}

String &String::operator=(double          rvalue)
{
	String r(rvalue);
#if ((__cplusplus) >= 201103L)
	return ((*this) = std::move(r));
#else
	return ((*this) = r);
#endif
}

String &String::operator=(float           rvalue)
{
	String r(rvalue);
#if ((__cplusplus) >= 201103L)
	return ((*this) = std::move(r));
#else
	return ((*this) = r);
#endif
}


/**
	* operator +=
	*/
/**
	* without move semantic (copy semantic)
	*/
String &String::operator+=(const String &rvalue)
{
	/**
		* collect buffer information
		*/

	char         *lvalue_c_buf         =  this->__non_standard__c_str_non_const();
	size_t        lvalue_buffer_length =  this->__non_standard__get_buffer_length();
	size_t        lvalue_string_length =  this->__non_standard__get_string_length();

	const char   *rvalue_c_buf         = rvalue.c_str();
	size_t        rvalue_buffer_length = rvalue.__non_standard__get_buffer_length();
	size_t        rvalue_string_length = rvalue.__non_standard__get_string_length();

	char    *new_string;

	/**
		* by specification,
		* if c_buf is NULL than buffer_length must be 0
		* if c_buf is empty_string than string_length must be 0
		*/
	_ASSERT(((lvalue_c_buf == NULL) && (lvalue_buffer_length == 0)) ||
		((((const char *) lvalue_c_buf) == (String::empty_string)) && (lvalue_string_length == 0)) ||
		(lvalue_c_buf != NULL));

	_ASSERT(((rvalue_c_buf == NULL) && (rvalue_buffer_length == 0)) ||
		((                rvalue_c_buf  == (String::empty_string)) && (rvalue_string_length == 0)) ||
		(rvalue_c_buf != NULL));

	/**
		* by specification, string length is always smaller than buffer
		* (this hold even if c_str is NULL or empty_string)
		* if buffer_length == 0, then string_length == 0 MUST true
		* if buffer_length  > 0, then string_length < buffer_length MUST true
		*/
	_ASSERT(((lvalue_buffer_length == 0) && (lvalue_string_length == 0)) ||
		((lvalue_buffer_length > 0) && (lvalue_string_length < lvalue_buffer_length)));

	_ASSERT(((rvalue_buffer_length == 0) && (rvalue_string_length == 0)) ||
		((rvalue_buffer_length > 0) && (rvalue_string_length < rvalue_buffer_length)));




	/**
		* check if r-value buffer is empty
		* in that case, there are nothing to concat, just return
		*/
	if ((rvalue_c_buf == NULL) || (rvalue_c_buf == (String::empty_string)) ||
		(rvalue_buffer_length == 0) || (rvalue_string_length == 0))
	{
		/* nothing to concat */
		return (*this);
	}


	/* r-value is instance with proper buffer */


	/**
		* check lvalue, if lvalue buffer is NULL or empty_string then
		* we need to allocate new buffer
		*/
	if ((lvalue_c_buf == NULL) || (((const char *) lvalue_c_buf) == (String::empty_string)))
	{
		/**
			* because rvalue is not in move semantic we need to
			* create new string buffer with malloc (cannot steal it from rvalue)
			*/
		new_string = (char *) malloc(rvalue_buffer_length * sizeof(char));

		/* check malloc */
		if (new_string == NULL)
		{
			/**
				* malloc is fail, lets try again with samller value
				* which is rvalue_string_length
				*/
			new_string = (char *) malloc((rvalue_string_length + 1) * sizeof(char));
			if (new_string == NULL)
			{
				/**
					* attempt to alloca smaller buffer is fail too,
					* so we should throw ENOMEM
					*
					* depend on config, we can recover from this error by
					* don't modify current string
					*/
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
				/* unhandled exception, terminate */
				std::terminate(); /* this function is non-return function */
#else
				return (*this); /* return this instance unchanged */
#endif
			}
			else
			{
				/* allocation success but using string length */
				lvalue_buffer_length = rvalue_string_length + 1;
			}
		}
		else
		{
			/* allocation success by using buffer_length */
			lvalue_buffer_length = rvalue_buffer_length;
		}

		/* allocation success because new_string is not NULL */

		/* only copy new string, including c string ending */
		memcpy(new_string, rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

		/**
			* force add '\0' at the end of buffer, to make sure
			* strlen is safe
			*/
		new_string[rvalue_buffer_length - 1] = '\0';

		/**
			* set new string length for lvalue
			* ignore current string length value in lvalue
			*/
		lvalue_string_length = rvalue_string_length;
	} /* lvalue == NULL */
	else /* lvalue != NULL */
	{
		/**
			* lvalue have proper buffer (although maybe empty in length)
			*/

		/* check if lvalue have big enough buffer to both string */
		if ((lvalue_string_length + rvalue_string_length) < lvalue_buffer_length)
		{
			/**
				* we have big enough buffer in lvalue for concatenation
				*/

			/* just copy string without reallocation */
			memcpy(lvalue_c_buf + lvalue_string_length,
				rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

			/* set new_string to current buffer (effectively don't change current buffer) */
			new_string = lvalue_c_buf;

			/**
				* since lvalue buffer don't change, don't need to set it
				* just set new string length
				*/
			lvalue_string_length += rvalue_string_length;
		} /* (lvalue string + rvalue string) < lvalue buffer */
		else /* (lvalue string + rvalue string) >= rvalue buffer */
		{
			/**
				* in case of current buffer don't fit for concatenation,
				* realloc buffer and copy because we cannot steal rvalue buffer
				*/

#if ((SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION) != 0)
			/**
				* before adding buffer, reduce buffer because we only
				* preserve zero termination from one operand
				*/
			if (rvalue_buffer_length == (rvalue_string_length + 1))
			{
				/**
					* reduce buffer length
					* rvalue_buffer_length must be bigger than 0 here
					*
					* can also done with rvalue_buffer_length = rvalue_string_length
					*/
				rvalue_buffer_length--;
			}
#endif

			/**
				* WARNING! before realloc, remember that it is always possible that
				* rvalue and lvalue are the same instance,
				* in that case, calling realloc can invalidate rvalue buffer,
				* so we need to check whether we dealing with same buffer or not
				* by comparing pointer value
				*/
			if (this == (&rvalue)) /* implicit cast this pointer to const object pointer */
			{
				/**
					* lvalue and rvalue is same instance, use malloc instead of realloc
					* by default we copy as big as both buffer,
					* but if fail, then just copy as big as both string
					* (won't work on lazy allocation, because malloc will always success)
					*/
				new_string = (char *) malloc((lvalue_buffer_length + rvalue_buffer_length) * sizeof(char));
				if (new_string == NULL)
				{
					/* maybe we allocate too much space, just work with string then */
					new_string = (char *) malloc((lvalue_string_length + rvalue_string_length + 1) * sizeof(char));
					if (new_string == NULL)
					{
						/* even with smaller buffer we cannot reserve heap,
						* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return (*this); /* return this instance unchanged */
#endif
					}
					else
					{
						/* set new buffer length */
						lvalue_buffer_length = lvalue_string_length + rvalue_string_length + 1;
					}
				}
				else
				{
					/* set new buffer length */

					lvalue_buffer_length = lvalue_buffer_length + rvalue_buffer_length;
				}

				/**
					* if we reach this, then alloc success
					* copy old data
					*/
				memcpy(new_string,                        lvalue_c_buf, lvalue_string_length * sizeof(char));
				memcpy(new_string + lvalue_string_length, rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

				/* only update string length */
				lvalue_string_length += rvalue_string_length;

				/**
					* free old pointer
					* this may invalidate rvalue object
					* but it is ok since we won't access it again
					*/
				this->__non_standard__free_string_non_invalidate();
			} /* this == rvalue */
			else /* this != rvalue */
			{
				/**
					* lvalue and rvalue are different instance
					* we can use realloc here
					*/
				new_string = (char *) realloc(lvalue_c_buf,
					(lvalue_buffer_length + rvalue_buffer_length) * sizeof(char));
				if (new_string == NULL)
				{
					/**
						* maybe we reallocate too much space,
						* just work with string then, (lvalue pointer
						* still valid if realloc fail)
						*/
					new_string = (char *) realloc(lvalue_c_buf,
						(lvalue_string_length + rvalue_string_length + 1) * sizeof(char));
					if (new_string == NULL)
					{
						/* even with smaller buffer we cannot reserve heap,
						* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return (*this); /* return this instance unchanged */
#endif
					}
					else
					{
						/* set new buffer length */
						lvalue_buffer_length = lvalue_string_length + rvalue_string_length + 1;
					}
				}
				else
				{
					/* set new buffer length */
					lvalue_buffer_length = lvalue_buffer_length + rvalue_buffer_length;
				}


				/**
					* lvalue string already copied by realloc
					* only copy rvalue
					*/
				memcpy(new_string + lvalue_string_length,
					rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

				/* only update string length */
				lvalue_string_length += rvalue_string_length;

				/**
					* don't invalidate old buffer,
					* realloc already take care of it
					*/
			} /* this != rvalue */
		} /* (lvalue string + rvalue string) >= lvalue buffer */
	} /* lvalue != NULL */

	/**
		* set new buffer to this instance
		* any free operation should already done in previous step
		*/
	this->__non_standard__set_new_buffer(new_string, lvalue_buffer_length, lvalue_string_length);

	return (*this);
} /* operator+= */

/**
	* with move semantic
	*/
#if ((__cplusplus) >= 201103L) /* C++11 */
String &String::operator+=(String &&rvalue)
{
	/**
		* collect buffer information
		*/

	char    *lvalue_c_buf         =  this->__non_standard__c_str_non_const();
	size_t   lvalue_buffer_length =  this->__non_standard__get_buffer_length();
	size_t   lvalue_string_length =  this->__non_standard__get_string_length();

	char    *rvalue_c_buf         = rvalue.__non_standard__c_str_non_const();
	size_t   rvalue_buffer_length = rvalue.__non_standard__get_buffer_length();
	size_t   rvalue_string_length = rvalue.__non_standard__get_string_length();

	char    *new_string;

	/**
		* by specification,
		* if c_buf is NULL than buffer_length must be 0
		* if c_buf is empty_string than string_length must be 0
		*/
	_ASSERT(((lvalue_c_buf == NULL) && (lvalue_buffer_length == 0)) ||
		((((const char *) lvalue_c_buf) == (String::empty_string)) && (lvalue_string_length == 0)) ||
		(lvalue_c_buf != NULL));

	_ASSERT(((rvalue_c_buf == NULL) && (rvalue_buffer_length == 0)) ||
		((((const char *) rvalue_c_buf) == (String::empty_string)) && (rvalue_string_length == 0)) ||
		(rvalue_c_buf != NULL));

	/**
		* by specification, string length is always smaller than buffer
		* (this hold even if c_str is NULL or empty_string)
		* if buffer_length == 0, then string_length == 0 MUST true
		* if buffer_length  > 0, then string_length < buffer_length MUST true
		*/
	_ASSERT(((lvalue_buffer_length == 0) && (lvalue_string_length == 0)) ||
		((lvalue_buffer_length > 0) && (lvalue_string_length < lvalue_buffer_length)));

	_ASSERT(((rvalue_buffer_length == 0) && (rvalue_string_length == 0)) ||
		((rvalue_buffer_length > 0) && (rvalue_string_length < rvalue_buffer_length)));




	/**
		* check if r-value buffer is empty
		* in that case, there are nothing to concat, just return
		*/
	if ((rvalue_c_buf == NULL) || (((const char *) rvalue_c_buf) == (String::empty_string)) ||
		(rvalue_buffer_length == 0) || (rvalue_string_length == 0))
	{
		/* nothing to concat */
		return (*this);
	}


	/* r-value is instance with proper buffer */


	/**
		* check lvalue, if lvalue buffer is NULL or empty_string then
		* we need to allocate new buffer
		*/
	if ((lvalue_c_buf == NULL) || (((const char *) lvalue_c_buf) == (String::empty_string)))
	{
		/* we currently don't have buffer, steal from rvalue! */
		new_string = rvalue_c_buf;

		/* set buffer information */
		lvalue_buffer_length = rvalue_buffer_length;
		lvalue_string_length = rvalue_string_length;

		/* invalidate rvalue, rvalue must be not same instance as lvalue */
		rvalue.__non_standard__invalidate_string();
	} /* lvalue == NULL */
	else /* lvalue != NULL */
	{
		/**
			* lvalue have proper buffer (although maybe empty in length)
			*/

		/* check which one has bigger buffer */
		if      ((lvalue_buffer_length >= rvalue_buffer_length) &&
			(lvalue_buffer_length > (lvalue_string_length + rvalue_string_length)))
		{
			/* lvalue buffer is the biggest, do copy, don't modify rvalue */
			memcpy(lvalue_c_buf + lvalue_string_length, rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

			/* don't need to change lvalue buffer and length information */
			new_string = lvalue_c_buf;
			lvalue_string_length += rvalue_string_length;
		}
		else if ((rvalue_buffer_length >= lvalue_buffer_length) &&
			(rvalue_buffer_length > (lvalue_string_length + rvalue_string_length)))
		{
			/* rvalue buffer is the biggest, STEAL! */

			/* move data in rvalue buffer behind, to make space for lvalue */
			memmove(rvalue_c_buf + lvalue_string_length,
				rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));
			memcpy(rvalue_c_buf, lvalue_c_buf, lvalue_string_length * sizeof(char));

			/* steal buffer */
			new_string = rvalue_c_buf;
			lvalue_string_length += rvalue_string_length;
			lvalue_buffer_length  = rvalue_buffer_length;

			/* free current buffer */
			if (lvalue_c_buf != rvalue_c_buf)
			{
				this->__non_standard__free_string_non_invalidate();
			}

			/* invalidate rvalue */
			if (this != (&rvalue))
			{
				rvalue.__non_standard__invalidate_string();
			}
		}
		else /* ((lvalue string + rvalue string) > lvalue buffer) || ((lvalue string + rvalue string) > rvalue buffer) */
		{
			/* all string cannot fit anyway, so we have to (re)allocate */

#if ((SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION) != 0)
			/**
				* before adding buffer, reduce buffer because we only
				* preserve zero termination from one operand
				*/
			if (rvalue_buffer_length == (rvalue_string_length + 1))
			{
				/**
					* reduce buffer length
					* rvalue_buffer_length must be bigger than 0 here
					*
					* can also done with rvalue_buffer_length = rvalue_string_length
					*/
				rvalue_buffer_length--;
			}
#endif

			/* check if we dealing with same instance */
			if (this == (&rvalue)) /* implicit cast this pointer to const object pointer */
			{
				/**
					* lvalue and rvalue is same instance, use malloc instead of realloc
					* by default we copy as big as both buffer,
					* but if fail, then just copy as big as both string
					* (won't work on lazy allocation, because malloc will always success)
					*/
				new_string = (char *) malloc((lvalue_buffer_length + rvalue_buffer_length) * sizeof(char));
				if (new_string == NULL)
				{
					/* maybe we allocate too much space, just work with string then */
					new_string = (char *) malloc((lvalue_string_length + rvalue_string_length + 1) * sizeof(char));
					if (new_string == NULL)
					{
						/* even with smaller buffer we cannot reserve heap,
						* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return (*this); /* return this instance unchanged */
#endif
					}
					else
					{
						/* set new buffer length */
						lvalue_buffer_length = lvalue_string_length + rvalue_string_length + 1;
					}
				}
				else
				{
					/* set new buffer length */
					lvalue_buffer_length = lvalue_buffer_length + rvalue_buffer_length;
				}

				/**
					* if we reach this, then alloc success
					* copy old data
					*/
				memcpy(new_string,                        lvalue_c_buf, lvalue_string_length * sizeof(char));
				memcpy(new_string + lvalue_string_length, rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

				/* only update string length */
				lvalue_string_length += rvalue_string_length;

				/**
					* free old pointer
					* this may invalidate rvalue object
					* but it is ok since we won't access it again
					*/
				this->__non_standard__free_string_non_invalidate();
			} /* this == rvalue */
			else /* this != rvalue */
			{
				/**
					* lvalue and rvalue are different instance
					* we can use realloc here
					*/
				new_string = (char *) realloc(lvalue_c_buf,
					(lvalue_buffer_length + rvalue_buffer_length) * sizeof(char));
				if (new_string == NULL)
				{
					/**
						* maybe we reallocate too much space,
						* just work with string then, (lvalue pointer
						* still valid if realloc fail)
						*/
					new_string = (char *) realloc(lvalue_c_buf,
						(lvalue_string_length + rvalue_string_length + 1) * sizeof(char));
					if (new_string == NULL)
					{
						/* even with smaller buffer we cannot reserve heap,
						* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return (*this); /* return this instance unchanged */
#endif
					}
					else
					{
						/* set new buffer length */
						lvalue_buffer_length = lvalue_string_length + rvalue_string_length + 1;
					}
				}
				else
				{
					/* set new buffer length */
					lvalue_buffer_length = lvalue_buffer_length + rvalue_buffer_length;
				}


				/**
					* lvalue string already copied by realloc
					* only copy rvalue
					*/
				memcpy(new_string + lvalue_string_length,
					rvalue_c_buf, (rvalue_string_length + 1) * sizeof(char));

				/* only update string length */
				lvalue_string_length += rvalue_string_length;

				/**
					* don't invalidate old buffer,
					* realloc already take care of it
					*/
			} /* this != rvalue */
		} /* ((lvalue string + rvalue string) > lvalue buffer) || ((lvalue string + rvalue string) > rvalue buffer) */
	} /* lvalue != NULL */

	/**
		* set new buffer to this instance
		* any free operation should already done in previous step
		*/
	this->__non_standard__set_new_buffer(new_string, lvalue_buffer_length, lvalue_string_length);

	return (*this);
} /* operator+ */
#endif /* ((__cplusplus) >= 201103L) */

/**
	* with c string
	*/
String &String::operator+=(const char *rvalue)
{
	char    *lvalue               =   this->__non_standard__c_str_non_const();
	size_t   lvalue_buffer_length =   this->__non_standard__get_buffer_length();
	size_t   lvalue_string_length =   this->__non_standard__get_string_length();

	size_t   rvalue_buffer_length;
	size_t   rvalue_string_length;

	char    *new_string;


	/**
		* by specification,
		* if c_buf is NULL than buffer_length must be 0
		* if c_buf is empty_string than string_length must be 0
		*/
	_ASSERT(((lvalue == NULL) && (lvalue_buffer_length == 0)) ||
		((((const char *) lvalue) == (String::empty_string)) && (lvalue_string_length == 0)) ||
		(lvalue != NULL));

	/**
		* by specification, string length is always smaller than buffer
		* (this hold even if c_str is NULL or empty_string)
		* if buffer_length == 0, then string_length == 0 MUST true
		* if buffer_length  > 0, then string_length < buffer_length MUST true
		*/
	_ASSERT(((lvalue_buffer_length == 0) && (lvalue_string_length == 0)) ||
		((lvalue_buffer_length > 0) && (lvalue_string_length < lvalue_buffer_length)));


	/* check validity of rvalue */
	if ((rvalue == NULL) ||
		(rvalue_string_length = strlen(rvalue), rvalue_string_length == 0))
	{
		/* if rvalue is NULL or empty_string, we don't need to do anything */
		return (*this);
	}

	/* rvalue have string content */
	rvalue_buffer_length = rvalue_string_length + 1;


	/**
		* check lvalue, if lvalue buffer is NULL or empty_string then
		* we need to allocate new buffer
		*/
	if ((lvalue == NULL) || (((const char *) lvalue) == (String::empty_string)))
	{
		/**
			* because rvalue is not in move semantic we need to
			* create new string buffer with malloc (cannot steal it from rvalue)
			*/
		new_string = (char *) malloc(rvalue_buffer_length * sizeof(char));

		/* check malloc */
		if (new_string == NULL)
		{
			/* failed to allocate buffer */

#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
			/* unhandled exception, terminate */
			std::terminate(); /* this function is non-return function */
#else
			return (*this); /* return this instance unchanged */
#endif
		}

		/* allocation success because new_string is not NULL */

		/* only copy new string, including c string ending */
		memcpy(new_string, rvalue, (rvalue_string_length + 1) * sizeof(char));

		/**
			* force add '\0' at the end of buffer, to make sure
			* strlen is safe
			*/
		new_string[rvalue_buffer_length - 1] = '\0';

		/* set new buffer information for lvalue */
		lvalue_buffer_length = rvalue_buffer_length;
		lvalue_string_length = rvalue_string_length;
	} /* lvalue == NULL */
	else /* lvalue != NULL */
	{
		/**
			* lvalue have proper buffer (non-NULL pointer)
			* although maybe string is empty in length
			*/

		/* check if lvalue have big enough buffer to both string */
		if ((lvalue_string_length + rvalue_string_length) < lvalue_buffer_length)
		{
			/**
				* we have big enough buffer in lvalue for concatenation
				*/

			/* just copy string without reallocation */
			memcpy(lvalue + lvalue_string_length,
				rvalue, (rvalue_string_length + 1) * sizeof(char));

			/* set new_string to current buffer (effectively don't change current buffer) */
			new_string = lvalue;

			/**
				* since lvalue buffer don't change, don't need to set it
				* just set new string length
				*/
			lvalue_string_length += rvalue_string_length;
		} /* (lvalue string + rvalue string) < lvalue buffer */
		else /* (lvalue string + rvalue string) >= rvalue buffer */
		{
			/**
				* in case of current buffer don't fit for concatenation,
				* realloc buffer and copy because we cannot steal rvalue buffer
				*/

#if ((SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION) != 0)
			/**
				* before adding buffer, reduce buffer size because we only
				* preserve zero termination from one operand
				*/
			rvalue_buffer_length--;
#endif


			/**
				* WARNING! before realloc, remember that it is always possible that
				* rvalue and lvalue are the same instance,
				* in that case, calling realloc can invalidate rvalue buffer,
				* so we need to check whether we dealing with same buffer or not
				* by comparing pointer value
				*/
			if (lvalue == rvalue)
			{
				/**
					* lvalue and rvalue is same instance, use malloc instead of realloc
					* by default we copy as big as both buffer,
					* but if fail, then just copy as big as both string
					* (won't work on lazy allocation, because malloc will always success)
					*/
				new_string = (char *) malloc((lvalue_buffer_length + rvalue_buffer_length) * sizeof(char));
				if (new_string == NULL)
				{
					/* maybe we allocate too much space, just work with string then */
					new_string = (char *) malloc((lvalue_string_length + 1 + rvalue_buffer_length) * sizeof(char));
					if (new_string == NULL)
					{
						/* even with smaller buffer we cannot reserve heap,
						* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return (*this); /* return this instance unchanged */
#endif
					}
					else
					{
						/* set new buffer length */
						lvalue_buffer_length = lvalue_string_length + 1 + rvalue_buffer_length;
					}
				}
				else
				{
					/* set new buffer length */
					lvalue_buffer_length = lvalue_buffer_length + rvalue_buffer_length;
				}

				/**
					* if we reach this, then alloc success
					* copy old data
					*/
				memcpy(new_string,                        lvalue, lvalue_string_length * sizeof(char));
				memcpy(new_string + lvalue_string_length, rvalue, (rvalue_string_length + 1) * sizeof(char));

				/* only update string length */
				lvalue_string_length += rvalue_string_length;

				/**
					* free old pointer
					* this may invalidate rvalue object
					* but it is ok since we won't access it again
					*/
				this->__non_standard__free_string_non_invalidate();
			} /* this == rvalue */
			else /* this != rvalue */
			{
				/**
					* lvalue and rvalue are different instance
					* we can use realloc here
					*/
				new_string = (char *) realloc(lvalue,
					(lvalue_buffer_length + rvalue_buffer_length) * sizeof(char));
				if (new_string == NULL)
				{
					/**
						* maybe we reallocate too much space,
						* just work with string then, (lvalue pointer
						* still valid if realloc fail)
						*/
					new_string = (char *) realloc(lvalue,
						(lvalue_string_length + 1 + rvalue_buffer_length) * sizeof(char));
					if (new_string == NULL)
					{
						/* even with smaller buffer we cannot reserve heap,
						* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return (*this); /* return this instance unchanged */
#endif
					}
					else
					{
						/* set new buffer length */
						lvalue_buffer_length = lvalue_string_length + 1 + rvalue_buffer_length;
					}
				}
				else
				{
					/* set new buffer length */
					lvalue_buffer_length = lvalue_buffer_length + rvalue_buffer_length;
				}


				/**
					* lvalue string already copied by realloc
					* only copy rvalue
					*/
				memcpy(new_string + lvalue_string_length,
					rvalue, (rvalue_string_length + 1) * sizeof(char));

				/* only update string length */
				lvalue_string_length += rvalue_string_length;

				/**
					* don't invalidate old buffer,
					* realloc already take care of it
					*/
			} /* this != rvalue */
		} /* (lvalue string + rvalue string) >= lvalue buffer */
	} /* lvalue != NULL */

	/**
		* set new buffer to this instance
		* any free operation should already done in previous step
		*/
	this->__non_standard__set_new_buffer(new_string, lvalue_buffer_length, lvalue_string_length);

	return (*this);
} /* operator+ */

/**
	* with integer types
	*/

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
String &String::operator+=(byte           rvalue)
{
	char s[5];
	size_t retval = unsigned_integer_to_string<byte>(s, 5, rvalue);
	if ((retval <= 0) || (retval >= 5))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}

	return ((*this) += s);
}
/* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0) */
#else /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */
String &String::operator+=(byte           rvalue)
{
	/* just reuse operator+= for unsigned int */
	return ((*this) += (unsigned int) rvalue);
}
#endif /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */


String &String::operator+=(int            rvalue)
{
	char   s[13];

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
	char *_s;

	size_t buffer_length;

	size_t retval;

	if (rvalue < 0)
	{
		s[0] = '-'; /* add minus sign */
		_s = &(s[1]); /* move 1 element */

		buffer_length = (13 - 1); /* reduce 1 element */

		rvalue *= (-1); /* make positive */
	}
	else
	{
		_s = &(s[0]); /* write from 0 */

		buffer_length = 13; /* use full buffer */
	}

	/* convert to string */
	retval = unsigned_integer_to_string<unsigned int>(_s, buffer_length, (unsigned int) rvalue);
	if ((retval <= 0) || (retval >= buffer_length))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}
/* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0) */
#else /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */
	int retval = snprintf(s, 13, "%d", rvalue);
	if ((retval <= 0) || (retval >= 13))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}
#endif /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */

	return ((*this) += s);
}

String &String::operator+=(unsigned int   rvalue)
{
	char   s[12];

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
	size_t retval = unsigned_integer_to_string<unsigned int>(s, 12, rvalue);
#else
	int retval = snprintf(s, 12, "%u", rvalue);
#endif
	if ((retval <= 0) || (retval >= 12))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}

	return ((*this) += s);
}

String &String::operator+=(long           rvalue)
{
	char   s[23];

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
	char *_s;

	size_t buffer_length;

	size_t retval;

	if (rvalue < 0)
	{
		s[0] = '-'; /* add minus sign */
		_s = &(s[1]); /* move 1 element */

		buffer_length = (23 - 1); /* reduce 1 element */

		rvalue *= (-1); /* make positive */
	}
	else
	{
		_s = &(s[0]); /* write from 0 */

		buffer_length = 23; /* use full buffer */
	}

	/* convert to string */
	retval = unsigned_integer_to_string<unsigned long>(_s, buffer_length, (unsigned long) rvalue);
	if ((retval <= 0) || (retval >= buffer_length))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}
/* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0) */
#else /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */
	int retval = snprintf(s, 23, "%ld", rvalue);
	if ((retval <= 0) || (retval >= 23))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}
#endif /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */

	return ((*this) += s);
}

String &String::operator+=(unsigned long  rvalue)
{
	char   s[22];

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
	size_t retval = unsigned_integer_to_string<unsigned long>(s, 22, rvalue);
#else
	int retval = snprintf(s, 22, "%lu", rvalue);
#endif
	if ((retval <= 0) || (retval >= 22))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}

	return ((*this) += s);
}

/**
	* with float types
	*/
String &String::operator+=(float          rvalue)
{
	return (*this) += ((double) rvalue);
}

String &String::operator+=(double         rvalue)
{
	char s[40];
	/* we don't have function to print double value, force reuse snprintf */
	int retval = snprintf(s, 40, "%f", rvalue);
	if ((retval <= 0) || (retval >= 40))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}

	return ((*this) += s);
}






#if 0
String operator+(String lhs, const String &rhs)
{
	/* reuse += operator */
	lhs += rhs;

	/* return new instance, without deep-copy */
	return lhs;
}

#if ((__cplusplus) >= 201103L) /* C++11 */
String operator+(String lhs, String &&rhs)
{
	/* reuse += operator */
	lhs += rhs;

	/* return new instance, without deep-copy */
	return lhs;
}

String operator+(String &&lhs, String rhs)
{
	/**
		* collect buffer information
		*/

	char    *lhs_c_buf         = lhs.__non_standard__c_str_non_const();
	size_t   lhs_buffer_length = lhs.__non_standard__get_buffer_length();
	size_t   lhs_string_length = lhs.__non_standard__get_string_length();

	char    *rhs_c_buf         = rhs.__non_standard__c_str_non_const();
	size_t   rhs_buffer_length = rhs.__non_standard__get_buffer_length();
	size_t   rhs_string_length = rhs.__non_standard__get_string_length();

	char    *new_string;

	/**
		* by specification,
		* if c_buf is NULL than buffer_length must be 0
		* if c_buf is empty_string than string_length must be 0
		*/
	_ASSERT(((lhs_c_buf == NULL) && (lhs_buffer_length == 0)) ||
		((((const char *) lhs_c_buf) == String::__non_standard__shared_empty_string()) && (lhs_string_length == 0)) ||
		(lhs_c_buf != NULL));

	_ASSERT(((rhs_c_buf == NULL) && (rhs_buffer_length == 0)) ||
		((((const char *) rhs_c_buf) == String::__non_standard__shared_empty_string()) && (rhs_string_length == 0)) ||
		(rhs_c_buf != NULL));

	/**
		* by specification, string length is always smaller than buffer
		* (this hold even if c_str is NULL or empty_string)
		* if buffer_length == 0, then string_length == 0 MUST true
		* if buffer_length  > 0, then string_length < buffer_length MUST true
		*/
	_ASSERT(((lhs_buffer_length == 0) && (lhs_string_length == 0)) ||
		((lhs_buffer_length > 0) && (lhs_string_length < lhs_buffer_length)));

	_ASSERT(((rhs_buffer_length == 0) && (rhs_string_length == 0)) ||
		((rhs_buffer_length > 0) && (rhs_string_length < rhs_buffer_length)));




	/**
		* check if lhs buffer is empty
		* in that case, there are nothing to concat, just return
		*/
	if ((lhs_c_buf == NULL) || (((const char *) lhs_c_buf) == String::__non_standard__shared_empty_string()) ||
		(lhs_buffer_length == 0) || (lhs_string_length == 0))
	{
		/* nothing to concat */
		return rhs;
	}


	/* rhs is instance with proper buffer */


	/**
		* check rhs, if lhs buffer is NULL or empty_string then
		* we need to allocate new buffer
		*/
	if ((rhs_c_buf == NULL) || (((const char *) rhs_c_buf) == String::__non_standard__shared_empty_string()))
	{
		/* we currently don't have buffer, steal from lhs! */
		new_string = lhs_c_buf;

		/* set buffer information */
		rhs_buffer_length = lhs_buffer_length;
		rhs_string_length = lhs_string_length;

		/* invalidate lhs, lhs must be not same instance as rhs */
		lhs.__non_standard__invalidate_string();
	} /* lhs == NULL */
	else /* lhs != NULL */
	{
		/**
			* lhs have proper buffer (although maybe empty in length)
			*/

		/* check which one has bigger buffer */
		if      ((lhs_buffer_length >= rhs_buffer_length) &&
			(lhs_buffer_length > (lhs_string_length + rhs_string_length)))
		{
			/* lhs buffer is the biggest, do copy, then STEAL! */
			memcpy(lhs_c_buf + lhs_string_length, rhs_c_buf, (rhs_string_length + 1) * sizeof(char));

			/* steal buffer */
			new_string = lhs_c_buf;
			rhs_string_length += lhs_string_length;
			rhs_buffer_length  = lhs_buffer_length;

			/* free and invalidate */

			/* free rhs buffer */
			if (lhs_c_buf != rhs_c_buf)
			{
				rhs.__non_standard__free_string_non_invalidate();
			}

			/* invalidate lhs */
			if ((&lhs) != (&rhs))
			{
				lhs.__non_standard__invalidate_string();
			}
		}
		else if ((rhs_buffer_length >= lhs_buffer_length) &&
			(rhs_buffer_length > (lhs_string_length + rhs_string_length)))
		{
			/* move data in rhs buffer behind, to make space for lhs */
			memmove(rhs_c_buf + lhs_string_length,
				rhs_c_buf, (rhs_string_length + 1) * sizeof(char));
			memcpy(rhs_c_buf, lhs_c_buf, lhs_string_length * sizeof(char));

			/* don't update */
			new_string = rhs_c_buf;
			rhs_string_length += lhs_string_length;
		}
		else /* ((lhs string + rhs string) > lhs buffer) || ((lhs string + rhs string) > rhs buffer) */
		{
			/* all string cannot fit anyway, so we have to (re)allocate */

#if ((SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION) != 0)
			/**
				* before adding buffer, reduce buffer because we only
				* preserve zero termination from one operand
				*/
			if (lhs_buffer_length == (lhs_string_length + 1))
			{
				/**
					* reduce buffer length
					* lhs_buffer_length must be bigger than 0 here
					*
					* can also done with lhs_buffer_length = rhs_string_length
					*/
				lhs_buffer_length--;
			}
#endif

			new_string = (char *) realloc(lhs_c_buf,
				(lhs_buffer_length + rhs_buffer_length) * sizeof(char));
			if (new_string == NULL)
			{
				/**
					* maybe we reallocate too much space,
					* just work with string then, (lhs pointer
					* still valid if realloc fail)
					*/
				new_string = (char *) realloc(lhs_c_buf,
					(lhs_string_length + rhs_string_length + 1) * sizeof(char));
				if (new_string == NULL)
				{
					/* even with smaller buffer we cannot reserve heap,
					* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
					/* unhandled exception, terminate */
					std::terminate(); /* this function is non-return function */
#else
					return rhs; /* return this instance unchanged */
#endif
				}
				else
				{
					/* set new buffer length */
					rhs_buffer_length = lhs_string_length + rhs_string_length + 1;
				}
			}
			else
			{
				/* set new buffer length */
				rhs_buffer_length = lhs_buffer_length + rhs_buffer_length;
			}


			/**
				* lhs string already copied by realloc
				* only copy rhs
				*/
			memcpy(new_string + lhs_string_length,
				rhs_c_buf, (rhs_string_length + 1) * sizeof(char));

			/* only update string length */
			rhs_string_length += lhs_string_length;

			/**
				* don't invalidate old buffer,
				* realloc already take care of it
				*/

			/* invalidate string */
			if (lhs_c_buf != rhs_c_buf)
			{
				/* free buffer rhs */
				rhs.__non_standard__free_string_non_invalidate();
			}

			if ((&lhs) != (&rhs))
			{
				/* invalidate buffer lhs */
				lhs.__non_standard__invalidate_string();
			}
		} /* ((lhs string + rhs string) > lhs buffer) || ((lhs string + rhs string) > rhs buffer) */
	} /* lhs != NULL */

	/**
		* set new buffer to this instance
		* any free operation should already done in previous step
		*/
	rhs.__non_standard__set_new_buffer(new_string, rhs_buffer_length, rhs_string_length);

	return rhs;
}

String operator+(String &&lhs, String &&rhs)
{
	String String_lhs(lhs);
	return (String_lhs += rhs);
}
#endif /* ((__cplusplus) >= 201103L) */


String operator+(const char *lhs, const String   &rhs)
{
	String String_lhs(lhs);
	return (String_lhs += rhs);
}
#endif

/* copy semantic */
String operator+(const char *lhs, String         _rhs)
{
	/**
		* collect buffer information
		*/

	size_t   lhs_buffer_length;
	size_t   lhs_string_length;

	char    *rhs               = _rhs.__non_standard__c_str_non_const();
	size_t   rhs_buffer_length = _rhs.__non_standard__get_buffer_length();
	size_t   rhs_string_length = _rhs.__non_standard__get_string_length();

	char    *new_string;


	/**
		* by specification,
		* if c_buf is NULL than buffer_length must be 0
		* if c_buf is empty_string than string_length must be 0
		*/
	_ASSERT(((rhs == NULL) && (rhs_buffer_length == 0)) ||
		((((const char *) rhs) == String::__non_standard__shared_empty_string()) && (rhs_string_length == 0)) ||
		(rhs != NULL));

	/**
		* by specification, string length is always smaller than buffer
		* (this specification hold even if c_str is NULL or empty_string)
		* if buffer_length == 0, then string_length == 0 MUST true
		* if buffer_length  > 0, then string_length < buffer_length MUST true
		*/
	_ASSERT(((rhs_buffer_length == 0) && (rhs_string_length == 0)) ||
		((rhs_buffer_length > 0) && (rhs_string_length < rhs_buffer_length)));


	/* check validity of lhs */
	if ((lhs == NULL) ||
		(lhs_string_length = strlen(lhs), lhs_string_length == 0))
	{
		/* if lhs is NULL or empty_string, we don't need to do anything */
		return _rhs;
	}

	/* lhs have string content */
	lhs_buffer_length = lhs_string_length + 1;


	/**
		* check rhs, if rhs buffer is NULL or empty_string then
		* we need to allocate new buffer
		*/
	if ((rhs == NULL) || (((const char *) rhs) == String::__non_standard__shared_empty_string()))
	{
		/**
			* because lhs is not in move semantic we need to
			* create new string buffer with malloc (cannot steal it from lhs)
			*/
		new_string = (char *) malloc(lhs_buffer_length * sizeof(char));

		/* check malloc */
		if (new_string == NULL)
		{
			/* failed to allocate buffer */

#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
			/* unhandled exception, terminate */
			std::terminate(); /* this function is non-return function */
#else
			return _rhs; /* return _rhs instance unchanged */
#endif
		}

		/* allocation success because new_string is not NULL */

		/* only copy new string, including c string ending */
		memcpy(new_string, lhs, (lhs_string_length + 1) * sizeof(char));

		/**
			* force add '\0' at the end of buffer, to make sure
			* strlen is safe
			*/
		new_string[lhs_buffer_length - 1] = '\0';

		/* set new buffer information for rhs */
		rhs_buffer_length = lhs_buffer_length;
		rhs_string_length = lhs_string_length;
	} /* rhs == NULL */
	else /* rhs != NULL */
	{
		/**
			* rhs have proper buffer (non-NULL pointer)
			* although maybe string is empty in length
			*/

		/* check if rhs have big enough buffer to both string */
		if ((lhs_string_length + rhs_string_length) < rhs_buffer_length)
		{
			/**
				* we have big enough buffer in rhs for concatenation
				*/

			/* just copy string without reallocation */
			memmove(rhs + lhs_string_length,
				rhs, (rhs_string_length + 1) * sizeof(char));
			memcpy(rhs, lhs, lhs_string_length * sizeof(char));

			/* set new_string to current buffer (effectively don't change current buffer) */
			new_string = rhs;

			/**
				* since rhs buffer don't change, don't need to set it
				* just set new string length
				*/
			rhs_string_length += lhs_string_length;
		} /* (lhs string + rhs string) < rhs buffer */
		else /* (lhs string + rhs string) >= lhs buffer */
		{
			/**
				* in case of current buffer don't fit for concatenation,
				* realloc buffer and copy because we cannot steal lhs buffer
				*/

#if ((SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION) != 0)
			/**
				* before adding buffer, reduce buffer size because we only
				* preserve zero termination from one operand
				*/
			lhs_buffer_length--;
#endif


			/**
				* WARNING! before realloc, remember that it is always possible that
				* lhs and rhs are the same instance,
				* in that case, calling realloc can invalidate lhs buffer,
				* so we need to check whether we dealing with same buffer or not
				* by comparing pointer value
				*/
			if (lhs == rhs)
			{
				/**
					* rhs and lhs is same instance, use malloc instead of realloc
					* by default we copy as big as both buffer,
					* but if fail, then just copy as big as both string
					* (won't work on lazy allocation, because malloc will always success)
					*/
				new_string = (char *) malloc((lhs_buffer_length + rhs_buffer_length) * sizeof(char));
				if (new_string == NULL)
				{
					/* maybe we allocate too much space, just work with string then */
					new_string = (char *) malloc((lhs_buffer_length + rhs_string_length + 1) * sizeof(char));
					if (new_string == NULL)
					{
						/* even with smaller buffer we cannot reserve heap,
						* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return _rhs; /* return _rhs instance unchanged */
#endif
					}
					else
					{
						/* set new buffer length */
						rhs_buffer_length = lhs_buffer_length + rhs_string_length + 1;
					}
				}
				else
				{
					/* set new buffer length */
					rhs_buffer_length = lhs_buffer_length + rhs_buffer_length;
				}

				/**
					* if we reach this, then alloc success
					* copy old data
					*/
				memcpy(new_string,                     lhs, lhs_string_length * sizeof(char));
				memcpy(new_string + lhs_string_length, rhs, (rhs_string_length + 1) * sizeof(char));

				/* only update string length */
				rhs_string_length += lhs_string_length;

				/**
					* free old pointer
					* this method may invalidate lhs object
					* but it is ok since we won't access it again
					*/
				_rhs.__non_standard__free_string_non_invalidate();
			} /* lhs == rhs */
			else /* lhs != rhs */
			{
				/**
					* rhs and lhs are different instance
					* we can use realloc here
					*/
				new_string = (char *) realloc(rhs,
					(lhs_buffer_length + rhs_buffer_length) * sizeof(char));
				if (new_string == NULL)
				{
					/**
						* maybe we reallocate too much space,
						* just work with string then, (rhs pointer
						* still valid if realloc fail)
						*/
					new_string = (char *) realloc(rhs,
						(lhs_buffer_length + rhs_string_length + 1) * sizeof(char));
					if (new_string == NULL)
					{
						/* even with smaller buffer we cannot reserve heap,
						* just terminate */
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
						/* unhandled exception, terminate */
						std::terminate(); /* this function is non-return function */
#else
						return _rhs; /* return _rhs instance unchanged */
#endif
					}
					else
					{
						/* set new buffer length */
						rhs_buffer_length = lhs_buffer_length + rhs_string_length + 1;
					}
				}
				else
				{
					/* set new buffer length */
					rhs_buffer_length = lhs_buffer_length + rhs_buffer_length;
				}


				/**
					* rhs string already copied by realloc, but move it
					* and then copy lhs
					*/
				memmove(new_string + lhs_string_length,
					new_string, (rhs_string_length + 1) * sizeof(char));
				memcpy(new_string, lhs, lhs_string_length * sizeof(char));

				/* only update string length */
				rhs_string_length += lhs_string_length;

				/**
					* don't invalidate old buffer,
					* realloc already take care of it
					*/
			} /* lhs != rhs */
		} /* (lhs string + rhs string) >= rhs buffer */
	} /* rhs != NULL */

	/**
		* set new buffer to _rhs instance
		* any free operation should already done in previous step
		*/
	_rhs.__non_standard__set_new_buffer(new_string, rhs_buffer_length, rhs_string_length);

	return _rhs;
}

/* move semantic */
#if ((__cplusplus) >= 201103L) /* C++11 */
String operator+(const char *lhs, String        &&rhs)
{
	/* reuse copy semantic */
	String String_rhs(rhs);
	return (lhs + String_rhs);
}
#endif

String operator+(char          lhs, String  rhs)
{
	char s[] = {lhs, '\0'};

	/* reuse operator+ for const char and string */
	return (s + rhs);
}

/* byte is uint8_t */
#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
String operator+(byte          lhs, String  rhs)
{
	char s[5];
	size_t retval = unsigned_integer_to_string<byte>(s, 5, lhs);
	if ((retval <= 0) || (retval >= 5))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return (*this);
#endif
	}

	return (s + rhs);
}
/* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0) */
#else /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */
String operator+(byte          lhs, String  rhs)
{
	/* just reuse operator+ unsigned int */
	return (((unsigned int) lhs) + rhs);
}
#endif /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */

String operator+(int           lhs, String  rhs)
{
	char s[13];

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
	char *_s;

	size_t buffer_length;

	size_t retval;

	if (lhs < 0)
	{
		s[0] = '-'; /* add minus sign */
		_s = &(s[1]); /* move 1 element */

		buffer_length = (13 - 1); /* reduce 1 element */

		lhs *= (-1); /* make positive */
	}
	else
	{
		_s = &(s[0]); /* write from 0 */

		buffer_length = 13; /* use full buffer */
	}

	/* convert to string */
	retval = unsigned_integer_to_string<unsigned int>(_s, buffer_length, (unsigned int) lhs);
	if ((retval <= 0) || (retval >= buffer_length))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return rhs;
#endif
	}
/* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0) */
#else /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */
	int retval = snprintf(s, 13, "%d", lhs);
	if ((retval <= 0) || (retval >= 13))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		/* abort if snprintf fail */
		std::terminate(); /* this function is non return */
#else
		/**
			* just return rhs instance
			* should return lhs
			*/
		return rhs;
#endif
	}
#endif /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */

	return (s + rhs);
}

String operator+(unsigned int  lhs, String  rhs)
{
	char s[12];

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
	size_t retval = unsigned_integer_to_string<unsigned int>(s, 12, (unsigned int) lhs);
#else
	int retval = snprintf(s, 12, "%u", lhs);
#endif

	if ((retval <= 0) || (retval >= 12))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return rhs;
#endif
	}

	return (s + rhs);
}

String operator+(long          lhs, String  rhs)
{
	char s[23];

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
	char *_s;

	size_t buffer_length;

	size_t retval;

	if (lhs < 0)
	{
		s[0] = '-'; /* add minus sign */
		_s = &(s[1]); /* move 1 element */

		buffer_length = (23 - 1); /* reduce 1 element */

		lhs *= (-1); /* make positive */
	}
	else
	{
		_s = &(s[0]); /* write from 0 */

		buffer_length = 23; /* use full buffer */
	}

	/* convert to string */
	retval = unsigned_integer_to_string<unsigned long>(_s, buffer_length, (unsigned long) lhs);
	if ((retval <= 0) || (retval >= buffer_length))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return rhs;
#endif
	}
/* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0) */
#else /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */
	int retval = snprintf(s, 23, "%ld", lhs);
	if ((retval <= 0) || (retval >= 23))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		/* abort if snprintf fail */
		std::terminate(); /* this function is non return */
#else
		/**
			* just return rhs instance
			* should return lhs
			*/
		return rhs;
#endif
	}
#endif /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */

	return (s + rhs);
}

String operator+(unsigned long lhs, String  rhs)
{
	char s[22];

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
	size_t retval = unsigned_integer_to_string<unsigned long>(s, 22, (unsigned long) lhs);
#else
	int retval = snprintf(s, 22, "%lu", lhs);
#endif

	if ((retval <= 0) || (retval >= 22))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return rhs;
#endif
	}

	return (s + rhs);
}

String operator+(float         lhs, String  rhs)
{
	/* reuse operator+ for double */
	return (((double) lhs) + rhs);
}

String operator+(double        lhs, String  rhs)
{
	char s[40];
	int retval = snprintf(s, 40, "%f", lhs);
	if ((retval <= 0) || (retval >= 40))
	{
#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		return rhs;
#endif
	}

	return (s + rhs);
}


/**
	* operator ==
	*/
bool operator==(const char *lhs, const String &rhs)
{
	/* rotate the operand */
	return (rhs == lhs);
}

/**
	* operator >
	*/
bool operator>(const char *lhs, const String &rhs)
{
	/* rotate operand */
	return (rhs < lhs);
}

/**
	* operator >=
	*/
bool operator>=(const char *lhs, const String &rhs)
{
	/* rotate operand */
	return (rhs <= lhs);
}

/**
	* operator <
	*/
bool operator<(const char *lhs, const String &rhs)
{
	/* rotate operand */
	return (rhs > lhs);
}

/**
	* operator <=
	*/
bool operator<=(const char *lhs, const String &rhs)
{
	/* rotate operand */
	return (rhs >= lhs);
}

/**
	* operator !=
	*/
bool operator!=(const char *lhs, const String &rhs)
{
	/* rotate the operand */
	return (rhs != lhs);
}
