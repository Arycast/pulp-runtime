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
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h> /* strlen, memcpy, memmove etc. */
#include <stdlib.h> /* alloc related functions */
#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0)
#include <stdio.h> /* snprintf */
#endif
#include <ctype.h>

/* standard header c++ */
#include <exception> /* for std::terminate */
#include <algorithm> /* for std::reverse */

/* local header */
#include "String.hpp"

/* when do simulation test, include simulation header */
#ifdef SIMULATION_TEST
#include "simulation_test.h"
#endif

struct search_token_s
{
	void    *previous_content;
	size_t   previous_content_length;

	void    *match;
	size_t   match_length;

	void    *next_content;
	size_t   next_content_length;
};

#if ((SSTRING_CONF_SEARCH_ALGORITHM_TWOWAY) == 0)
static const uint8_t *memmem_kmp(const uint8_t *haystack, size_t haystacklen, const uint8_t *needle, size_t needlelen);
static const uint8_t *memrmem_kmp(const uint8_t *haystack, size_t haystacklen, const uint8_t *needle, size_t needlelen);
#else
static const uint8_t *memmem_twoway(const uint8_t *haystack, size_t haystacklen, const uint8_t *needle, size_t needlelen);
static const uint8_t *memrmem_twoway(const uint8_t *haystack, size_t haystacklen, const uint8_t *needle, size_t needlelen);
#endif


static ssize_t mem_replace(void **buffer, size_t *content_length, size_t *buffer_length, const void *match, size_t match_length, const void *replacement, size_t replacement_length, bool content_is_string);

static ssize_t mem_replace_with_smaller_size( void  *buffer,                         size_t *buffer_length, const void *match, size_t match_length, const void *replacement, size_t replacement_length);
static ssize_t mem_replace_with_larger_size(  void **buffer, size_t *content_length, size_t *buffer_length, const void *match, size_t match_length, const void *replacement, size_t replacement_length);
static ssize_t mem_replace_with_same_size(    void  *buffer,                         size_t  buffer_length, const void *match,                      const void *replacement, size_t match_and_replacement_length);

static void *mem_search_tokenize(const void *buffer, size_t buffer_length, const void *match, size_t match_length, struct search_token_s *token);


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

/**
	* @brief Converts a double to a string without FPU or sprintf.
	* @arg str          Output character buffer.
	* @arg str_len      Length of output buffer @arg str, this value should including termination '\0'
	* @arg value        The double value to convert.
	* @arg precision    Number of digits after the decimal (e.g., 6).
	* @return           number of character written to buffer @arg str excluding terimination '\0'
	*                   error if return 0 (which buffer @arg str content is undefined) or str_len
	*                   (which mean buffer @arg str is too small)
	*/
#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
static size_t double_to_string(char *str, size_t str_len, double value, size_t precision)
{
	constexpr const char base_char_map[] = "0123456789";

	bool     add_minus_sign = false; /* indicator if we need to add minus sign */
	size_t   len = 0; /* counter number of character written to buffer */

	/* for separation of integer and floating part */
	uint64_t i_part;
	double   f_part;

	size_t   retval;


	/* check if value is negative number */
	if (value < 0)
	{
		add_minus_sign = true;
	}

	/* cannot write anything if string buffer is empty or invalid */
	if ((str == NULL) ||
		((!add_minus_sign) && (str_len <= 1)) ||
		(  add_minus_sign  && (str_len <= 2))) /* need space for sign */
	{
		/* cannot write to buffer or buffer is not enough */
		return 0;
	}

	/* add sign */
	if (add_minus_sign)
	{
		*str++ = '-';
		len++; /* increment len */

		/* decrease str_len */
		str_len--;

		value = -value; /* make value positive */
	}

	/**
		* Extract integer part
		* Note: Casting double to unsigned long int usually requires a soft-float library
		* if no FPU exists. For pure hardware, you'd process the raw IEEE-754 bits.
		*
		* we don't do rounding
		* for example, if precision == 1 then
		* 0.88     will be 0.8 instead of 0.9
		* 0.999999 will be 0.9 instead of 1.0
		*/
	i_part = (uint64_t) value;

	/* remove integer part from value */
	f_part = value - ((double) i_part);

	/* Convert integer part to string */
	retval = unsigned_integer_to_string<uint64_t>(str, str_len, i_part);
	/* check if error */
	if (retval <= 0)
	{
		/* nothing to write, we expect at least 1 character written ('0') */
		return 0; /* return error */
	}
	else if (retval >= str_len)
	{
		/* digit truncated, buffer is not enough */
		return 0; /* return 0 */
	}

	/* move pointer based on retval */
	str += retval;
	len += retval; /* increment character count */

	str_len -= retval; /* decrease buffer length */


	/* Handle decimal point and fractional part */
	if (precision > 0)
	{
		size_t i;

		/* check if we can add '.' and number as much as required precision */
		if (str_len < (precision + 2))
		{
			return 0; /* not enough for '.' and precision and '\0' */
		}

		/* add fraction separator */
		*str++ = '.';

		/* Convert fractional part to a scaled integer
		* Example: 0.123 -> 123000 (if precision is 6) */
		for (i = 0; i < precision; i++)
		{
			size_t digit;

			/* make fractional part integer */
			f_part *= 10;

			/* get digit by getting integer part only */
			digit = (size_t) f_part;

			/* write to str */
			*str++ = base_char_map[digit];

			/* remove integer part */
			f_part -= ((double) digit);
		}
	}

	/* add termination if buffer still enough */
	if (str_len >= 1)
	{
		*str = '\0';
	}

	/* return number of character written */
	return len;
}
#endif /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0) */


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


static void *__non_standard__memrchr(const void *s, int c, size_t n)
{
	size_t          i;
	const uint8_t *_s;

	/* check input buffer */
	if ((s == NULL) || (n == 0))
	{
		return NULL; /* not found if buffer is invalid or have zero length */
	}

	/* scan buffer s from end position */
	for (i = 0, _s = (const uint8_t *) s, _s += (n - 1); i < n; ++i, --_s)
	{
		/* check if matching byte is found */
		if ((*_s) == ((uint8_t) c))
		{
			/* match found
			* remove qualifier and make it general pointer */
			return (void *) _s;
		}
	}

	/* no match found */
	return NULL;
}

/**
	* simulate memmem and reverse of memmem
	*/
static void *__non_standard__memmem(const void *haystack, size_t haystacklen,
	const void *needle, size_t needlelen)
{
	/* 1st check as always check if haystack length is smaller than needle length */
	if ((haystacklen < needlelen) || (haystacklen == 0) || (needlelen == 0))
	{
		/* not found */
		return NULL;
	}

	/* check some special case */
	if (needlelen == 1)
	{
		/**
			* do memchr when we only search 1 character
			* without typecast to (void *) to remove qualifier this will throw error
			* when compiling in C++ with system standard library
			* because system's string.h provide both (via polymorphism):
			* const void *memchr(const void *,...)
			*       void *memchr(      void *,...)
			*/
		return (void *) memchr(haystack, (int) (*((char *) needle)), haystacklen);
	}

	/* when needlelen more than 0 */
#if ((SSTRING_CONF_SEARCH_ALGORITHM_TWOWAY) == 0)
	/* use KMP algorithm */
	/* remove const qualifier */
	return (void *) memmem_kmp((const uint8_t *) haystack, haystacklen, (const uint8_t *) needle, needlelen);
#else
	/* use two-way algorithm */
	return (void *) memmem_twoway((const uint8_t *) haystack, haystacklen, (const uint8_t *) needle, needlelen);
#endif
}

static void *__non_standard__memrmem(const void *haystack, size_t haystacklen,
	const void *needle, size_t needlelen)
{
	/* 1st check as always check if haystack length is smaller than needle length */
	if ((haystacklen < needlelen) || (haystacklen == 0) || (needlelen == 0))
	{
		/* not found */
		return NULL;
	}

	/* check some special case */
	if (needlelen == 1)
	{
		/* do memchr when we only search 1 character */
		return __non_standard__memrchr(haystack, (int) (*((char *) needle)), haystacklen);
	}

	/* when needle more than 0 */
#if ((SSTRING_CONF_SEARCH_ALGORITHM_TWOWAY) == 0)
	/* use KMP algorithm */
	/* remove const qualifier */
	return (void *) memrmem_kmp((const uint8_t *) haystack, haystacklen, (const uint8_t *) needle, needlelen);
#else
	/* use two-way algorithm */
	return (void *) memrmem_twoway((const uint8_t *) haystack, haystacklen, (const uint8_t *) needle, needlelen);
#endif
}

#if ((SSTRING_CONF_SEARCH_ALGORITHM_TWOWAY) == 0)
/**
	* Helper to build the LPS (Longest Proper Prefix) table for memmem and memrmem operation
	*/
static void kmp_compute_lps_table(const uint8_t *needle, size_t needlelen, std::vector<size_t> &lps_table)
{
	size_t matched = 0;
	size_t i;

	/* set first element to 0 */
	lps_table[0] = 0;

	/* for every needle fragment, calculate longest prefix, suffix match value */
	for (i = 1; i < needlelen; ++i)
	{
		/**
			* here we iterate needle try to build prefix match
			* for i = 1, we build for n[0], n[1]
			* for i = 2, we build for n[0], n[1], n[2]
			* for i = 3, we build for n[0], n[1], n[2], n[3]
			* for i = 4, we build for n[0], n[1], n[2], n[3], n[4]
			* and so on...
			* for every matching value. we save it in lps[i]
			*/

		/**
			* do backtrack
			* backtrack is try to found last character in needle that
			* match with current looked up character
			*
			* if character is same, then prefix is exit from while loop
			* if character is not same, go back based on previous backtrack result
			* to find last matching character
			*
			* (backtrack will stop at lps_table[0] because lps_table[0] guaranteed to be 0)
			*/
		while ((matched > 0) && (needle[i] != needle[matched]))
		{
			matched = lps_table[matched - 1];
		}

		/* if match found, then current character is match with prefix */
		if (needle[i] == needle[matched])
		{
			matched++;
		}

		/* set prefix */
		lps_table[i] = matched;
	}
}

/**
	* Knuth-Morris-Pratt (KMP) algorithm as the backend for memmem
	* intentionally do not check argument, argument checked by front end
	* we working on 8-bit (1 byte) unit
	*/
static const uint8_t *memmem_kmp(const uint8_t *haystack, size_t haystacklen, const uint8_t *needle, size_t needlelen)
{
	/**
		* Allocate LPS table on the stack for small needles, or heap for large ones
		* since C++ don't support VLA (Variable-Length Arrays), use vector
		* to allocate the memory
		*/
	std::vector<size_t> lps_table(needlelen);

	size_t haystack_index; /* index for haystack */
	size_t needle_index; /* index for needle */

	/* we hope allocation is success */
	if ((lps_table.data()) == NULL)
	{
		/* cannot allocate lps table */
		return NULL;
	}

	/* generate lps */
	kmp_compute_lps_table(needle, needlelen, lps_table);

	/* search */
	needle_index = 0;
	for (haystack_index = 0; haystack_index < haystacklen; ++haystack_index)
	{
		while ((needle_index > 0) && (haystack[haystack_index] != needle[needle_index]))
		{
			needle_index = lps_table[needle_index - 1]; /* The "Magic": Skip unnecessary comparisons */
		}

		/* check if match */
		if (haystack[haystack_index] == needle[needle_index])
		{
			needle_index++;
		}

		/* all needle match with haystack segment */
		if (needle_index == needlelen)
		{
			/* return haystack pointer that align with start of the needle */
			return (haystack + haystack_index - needlelen + 1);
		}
	}

	return NULL;
}

/**
	* same with memmem_kmp, but search in reverse direction
	* also skip argument check
	*/
static const uint8_t *memrmem_kmp(const uint8_t *haystack, size_t haystacklen, const uint8_t *needle, size_t needlelen)
{
	/* allocate reversed version of needle */
	std::vector<uint8_t>  needle_reversed(needlelen); /* reversed version of needle */
	uint8_t              *needle_reversed_p; /* pointer to needle_reversed internal buffer */

	/* allocate lps_table */
	std::vector<size_t>   lps_table_reverse(needlelen);

	size_t needle_index;
	size_t haystack_index;

	size_t i, j;

	/* check if allocation success */
	if ((needle_reversed_p = needle_reversed.data(), needle_reversed_p == NULL) || ((lps_table_reverse.data()) == NULL))
	{
		/**
			* should throw exception, but just return null if we fail to allocate
			* vector will be free'd automatically
			*/
		return NULL;
	}

	/**
		* fill needle_reversed as inverse of needle
		* start to use needle_reversed_p to access buffer
		* and do not use container object needle_reversed
		*/
	for (i = 0, j = needlelen - 1; i < needlelen; ++i, --j)
	{
		needle_reversed_p[i] = needle[j];
	}

	/**
		* build lps_table
		* since lps table lookup will use needle index, and
		* we will use reversed needle, lps_table will also reversed
		*/
	kmp_compute_lps_table(needle_reversed_p, needlelen, lps_table_reverse);

	/**
		* Scan haystack from end to beginning, but set needle index to 0 because
		* needle we use is reversed
		* use i for counting forward
		*/
	needle_index = 0;
	for (i = 0, haystack_index = haystacklen - 1; i < haystacklen; ++i, --haystack_index)
	{
		/* basically same with memmem_kmp */
		while ((needle_index > 0) && (haystack[haystack_index] != needle_reversed_p[needle_index]))
		{
			needle_index = lps_table_reverse[needle_index - 1];
		}

		if (haystack[haystack_index] == needle_reversed_p[needle_index])
		{
			needle_index++;
		}

		if (needle_index == needlelen)
		{
			return (haystack + haystack_index);
		}
	}

	return NULL;
}

/* ((SSTRING_CONF_SEARCH_ALGORITHM_TWOWAY) == 0) */
#else /* ((SSTRING_CONF_SEARCH_ALGORITHM_TWOWAY) != 0) */

/**
	* Helper: Finds the maximal suffix of a string
	*/
static size_t max_suffix(const uint8_t *needle, size_t needlelen, bool reversed)
{
	size_t i = 0, j = 1, k = 1, p = 1;
	while ((j + k) <= needlelen)
	{
		uint8_t a = needle[i + k - 1];
		uint8_t b = needle[j + k - 1];

		/* Compare based on the 'reversed' flag to find different orderings */
		if (reversed ? (a < b) : (a > b)) /* a and b comparison is part of Cesari-Crochemore Theorem */
		{
			j += k;
			k = 1;
			p = j - i;
		}
		else if (a == b)
		{
			if (k == p)
			{
				j += p;
				k = 1;
			}
			else
			{
				k++;
			}
		}
		else
		{
			i = j;
			j = i + 1;
			k = p = 1;
		}
	}
	return i;
}

/**
	* libc two-way algorithm
	*/
static const uint8_t *memmem_twoway(const uint8_t *haystack, size_t haystacklen, const uint8_t *needle, size_t needlelen)
{
	/* 1. Pre-processing: Find the critical factorization point */
	size_t i = max_suffix(needle, needlelen, false);
	size_t j = max_suffix(needle, needlelen, true);
	size_t cut = (i > j) ? i : j; /* MAX(i, j) */

	/* 2. Search Logic */
	size_t pos = 0;
	while (pos <= haystacklen - needlelen)
	{
		/* Start matching from the 'cut' to the right */
		size_t k = cut; /* MAX(cut, 0) */

		/* Match the right part */
		while ((k < needlelen) && (needle[k] == haystack[pos + k]))
		{
			k++;
		}

		if (k < needlelen)
		{
			/* Mismatch in the right part */
			pos += (k - cut + 1);
		}
		else
		{
			/* k >= needlelen */

			/* Right part matches, now check the left part (0 to cut) */
			size_t f = cut;
			while ((f > 0) && (needle[f - 1] == haystack[pos + f - 1]))
			{
				f--;
			}

			/* check if all left part is full match */
			if (f == 0)
			{
				return (haystack + pos); /* Full match! */
			}

			pos++; /* Shift and try again */
		}
	}

	return NULL;
}

static const uint8_t *memrmem_twoway(const uint8_t *haystack, size_t haystacklen, const uint8_t *needle, size_t needlelen)
{
	/* 1. Pre-processing: Find the critical factorization point */
	size_t i = max_suffix(needle, needlelen, false);
	size_t j = max_suffix(needle, needlelen, true);
	size_t cut = (i > j) ? i : j; /* MAX(i, j) */

	/* 2. Search Logic (Starting from the end) */
	size_t pos = haystacklen - needlelen;

	while (1)
	{
		/* In reverse search, we verify the "left" part (0 to cut) first */
		size_t k = 0;
		while (k <= cut && needle[k] == haystack[pos + k])
		{
			k++;
		}

		if (k <= cut)
		{
			/* Mismatch in the left part.
			* Shift logic: we move backward. */
			size_t shift = cut - k + 1;

			/* exit loop condition */
			if (shift > pos)
			{
				break; /* Cannot shift further left */
			}

			pos -= shift;
		}
		else
		{
			/* Left part matches, now check the right part (cut + 1 to needlelen - 1) */
			size_t r = cut + 1;

			while ((r < needlelen) && (needle[r] == haystack[pos + r]))
			{
				r++;
			}

			if (r == needlelen)
			{
				/* Full match found! */
				return (haystack + pos);
			}

			/* exit loop condition */
			if (pos == 0)
			{
				break;
			}

			pos--; /* Default shift */
		}
	}

	return NULL;
}

#endif /* ((SSTRING_CONF_SEARCH_ALGORITHM_TWOWAY) != 0) */


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

String::String(float         value, unsigned int precision) : String((double) value, precision)
{
}

String::String(double        value) : String(value, SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_DEFAULT_LEN)
{
}

#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)
String::String(double        value, unsigned int precision)
{
	char *new_string;
	size_t buffer_length;

	size_t retval;

	/* check if user give too large precision */
	if (precision > SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_MAX_LEN)
	{
		/* to much precision, reduce it to maximum */
		precision = SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_MAX_LEN;
	}

	/* set buffer_length */
	buffer_length = 23 + precision; /* 23: 1 sign + 20 digit + 1 '.' symbol + 1 '\0' symbol */

	/* allocate buffer to write */
	new_string = (char *) malloc(buffer_length * sizeof(char));
	if (new_string == NULL)
	{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
		std::terminate();
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	/* convert double value to string */
	retval = double_to_string(new_string, buffer_length, value, (size_t) precision);
	if ((retval <= 0) || (retval >= buffer_length))
	{
		free(new_string); /* free new_string before return error */

#if ((SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL) != 0)
		std::terminate();
#else
		this->__non_standard__set_new_buffer(NULL, 0);
#endif
		return;
	}

	/* allocation success */
	_ASSERT(new_string != NULL);
	_ASSERT(retval < buffer_length);

	/* put into buffer */
	this->__non_standard__set_new_buffer(new_string, buffer_length, retval);
}
/* (SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0 */
#else /* (SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0 */
String::String(double        value, unsigned int precision)
{
	constexpr size_t buffer_length = 40;
	size_t string_length;

	char *new_string = (char *) malloc(buffer_length * sizeof(char));

	int retval;

	/* cannot use precision */
	(void) precision;

	/* check if allocation success */
	if (new_string == NULL)
	{
#if ((SSTRING_CONF_ABORT_ON_ALLOC_FAIL) != 0)
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

	/* string_length is snprintf return value */
	string_length = (size_t) retval;

	/* allocation success */
	_ASSERT(new_string != NULL);
	_ASSERT(string_length < buffer_length);

	/* put into buffer */
	this->__non_standard__set_new_buffer(new_string, buffer_length, string_length);
}
#endif /* (SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0 */





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
#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)

	constexpr size_t buffer_length = 23 + SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_DEFAULT_LEN;

	char s[buffer_length];

	/* we don't have function to print double value, force reuse snprintf */
	size_t retval = double_to_string(s, buffer_length, rvalue, SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_DEFAULT_LEN);
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
#endif /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */


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
#if ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) == 0)

	constexpr size_t buffer_length = 23 + SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_DEFAULT_LEN;
	char s[23 + buffer_length];
	size_t retval = double_to_string(s, buffer_length, lhs, SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_DEFAULT_LEN);
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

#endif  /* ((SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION) != 0) */

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


unsigned int String::indexOf(char           val, unsigned int from) const
{
	size_t       string_length = this->__non_standard__get_string_length();
	const char  *s = this->c_str();

	/* check if requested index is beyond string data */
	if (((size_t) from) >= string_length)
	{
		return (unsigned int) (-1);
	}

	/* check if we search end of string character */
	if (val == '\0')
	{
		/* end of string */
		return (unsigned int) string_length;
	}
	else if ((s == NULL) || (s == (String::empty_string)) || (string_length == 0))
	{
		/* fail to search if user want non-empty character in empty/invalid string */
		return (unsigned int) (-1);
	}
	else
	{
		const char *p = (const char *) memchr(s + from, (int) val, string_length - from);
		if (p == NULL)
		{
			return (unsigned int) (-1);
		}
		else
		{
			return (unsigned int) (p - s);
		}
	}
}

unsigned int String::indexOf(const char    *val, unsigned int from) const
{
	size_t       string_length = this->__non_standard__get_string_length();
	const char  *s             = this->c_str();
	const char  *p;

	size_t val_length; /* val string length, don't calculate val_length before we sure that val is not NULL */

	/* get val length */
	if (val == NULL)
	{
		/* return false regardless */
		return (unsigned int) (-1);
	}

	/* get val/needle length */
	val_length = strlen(val);

	if (from > string_length)
	{
		/* nothing to search */
		return (unsigned int) (-1);
	}
	else if (val_length == 0)
	{
		/* same behaviour as search '\0' in string, return string length */
		return (unsigned int) string_length;
	}
	else if (from == string_length)
	{
		/* val_length > 0, but user want to search from end of string */
		return (unsigned int) (-1);
	}

	/**
		* val must be not null, and proper/non-empty string,
		* rely on memmem argument check to check argument validity
		*/
	p = (const char *) __non_standard__memmem(s + from, string_length - from, val, val_length);
	if (p == NULL)
	{
		/* not found */
		return (unsigned int) (-1);
	}
	else
	{
		return (unsigned int) (p - s);
	}
}

unsigned int String::indexOf(const String  &val, unsigned int from) const
{
	size_t         string_length = this->__non_standard__get_string_length();
	const char  *  s             = this->c_str();

	size_t         val_length    =   val.__non_standard__get_string_length();
	const char  *s_val           =   val.c_str();

	const char  *p;

	if (from > string_length)
	{
		/* nothing to search */
		return (unsigned int) (-1);
	}
	else if (val_length == 0)
	{
		/* same behaviour as search '\0' in string, return string length */
		return (unsigned int) string_length;
	}
	else if (from == string_length)
	{
		/* val_length > 0, but user want to search from end of string */
		return (unsigned int) (-1);
	}

	/**
		* val must be not null, and proper/non-empty string,
		* rely on memmem argument check to check argument validity
		*/
	p = (const char *) __non_standard__memmem(s + from, string_length - from, s_val, val_length);
	if (p == NULL)
	{
		/* not found */
		return (unsigned int) (-1);
	}
	else
	{
		return (unsigned int) (p - s);
	}
}

unsigned int String::lastIndexOf(char           val, unsigned int from) const
{
	size_t       string_length = this->__non_standard__get_string_length();
	const char  *s = this->c_str();
	const char  *p;

	/* check if current instance have string or not */
	if ((s == NULL) || (s == (String::empty_string)) || (string_length == 0) || (val == '\0'))
	{
		/* fail to search if user want non-empty character in empty/invalid string */
		return (unsigned int) (-1);
	}

	/* this instance have valid string */

	/* check from argument */
#if 0
	if (from == 0)
	{
		/* only need to match single character  */
		return (val == (*s)) ? 0 : (unsigned int) (-1);
	}
	else
#endif
	if (((size_t) from) >= string_length) /* check if requested index is beyond string length */
	{
		/* shrink search space */
		from = (unsigned int) (string_length - 1); /* string_length must be not 0 */
	}

	/* search */
	p = (const char *) __non_standard__memrchr(s, (int) val, from + 1);
	if (p == NULL)
	{
		return (unsigned int) (-1);
	}
	else
	{
		return (unsigned int) (p - s);
	}
}

unsigned int String::lastIndexOf(const char    *val, unsigned int from) const
{
	size_t       string_length = this->__non_standard__get_string_length();
	const char  *s = this->c_str();
	const char  *p;

	size_t       val_length;

	/* check if current instance have string or not */
	if ((s == NULL) || (s == (String::empty_string)) || (string_length == 0) ||
		(val == NULL) || (val_length = strlen(val), val_length == 0))
	{
		/* fail to search if user want non-empty character in empty/invalid string */
		return (unsigned int) (-1);
	}

	/**
		* this instance have valid string
		* and val_length should not 0
		*/

	/* check from argument */
	if (((size_t) from) >= string_length) /* check if requested index is beyond string length */
	{
		/* shrink search space */
		from = (unsigned int) (string_length - 1); /* string_length must be not 0 */
	}

	/* search */
	p = (const char *) __non_standard__memrmem(s, from + 1, val, val_length);
	if (p == NULL)
	{
		return (unsigned int) (-1);
	}
	else
	{
		return (unsigned int) (p - s);
	}
}

unsigned int String::lastIndexOf(const String  &val, unsigned int from) const
{
	size_t         string_length = this->__non_standard__get_string_length();
	const char    *s             = this->c_str();

	size_t         val_length    =   val.__non_standard__get_string_length();
	const char  *s_val           =   val.c_str();

	const char  *p;

	/* check if current instance have string or not */
	if ((s == NULL) || (s == (String::empty_string)) || (string_length == 0) ||
		(s_val == NULL) || (s_val == (String::empty_string)) || (val_length == 0))
	{
		/* fail to search if user want non-empty character in empty/invalid string */
		return (unsigned int) (-1);
	}

	/**
		* this instance have valid string
		* and val_length should not 0
		*/

	/* check from argument */
	if (((size_t) from) >= string_length) /* check if requested index is beyond string length */
	{
		/* shrink search space */
		from = (unsigned int) (string_length - 1); /* string_length must be not 0 */
	}

	/* search */
	p = (const char *) __non_standard__memrmem(s, from + 1, s_val, val_length);
	if (p == NULL)
	{
		return (unsigned int) (-1);
	}
	else
	{
		return (unsigned int) (p - s);
	}
}

int String::reserve(unsigned int size)
{
	/* check if user want to free all buffer */
	if (size == 0)
	{
		/* invalidate string */
		this->__non_standard__free_string_non_invalidate().__non_standard__invalidate_string();
	}
	else
	{
		/* requested size must be > 0 */

		/* get current buffer */
		char *s = this->__non_standard__c_str_non_const();

		/* get current length */
		/*size_t  buffer_length = this->__non_standard__get_buffer_length();*/
		size_t  string_length = this->__non_standard__get_string_length();

		/* realloc if needed, we allow buffer to shrink smaller than current size */
		if ((s == NULL) || (s == (String::empty_string)))
		{
			/* allocate buffer */
			char *new_string = (char *) malloc(size * sizeof(char));
			if (new_string == NULL)
			{
				/* by standard, return 0 when failure */
				return 0;
			}

			/* set first character to '\0' */
			*new_string = '\0';

			/* set buffer */
			this->__non_standard__set_new_buffer(new_string, size, 0);
		}
		else
		{
			/* realloc */
			char *new_string = (char *) realloc(s, size * sizeof(char));
			if (new_string == NULL)
			{
				/* realloc fail, old buffer unchanged */
				return 0;
			}

			/**
				* check new size
				* only check with string_length, dont need to check with buffer_length
				*/
			if (size <= string_length)
			{
				/* string_length changed (truncated) */
				string_length = size - 1; /* string length should less than size */

				/* truncate string */
				new_string[string_length] = '\0';
			}

			/* set buffer */
			this->__non_standard__set_new_buffer(new_string, size, string_length);
		}
	}

	/* return success */
	return 1;
}

void String::remove(unsigned int index)
{
	/**
		* we can effectively remove all character after index by set current index to '\0'
		* out-of-bound and string length changing already handled by operator
		*/
	(*this)[index] = '\0';
}

void String::remove(unsigned int index, unsigned int count)
{
	char     *s, *_s;
	size_t    buffer_length;
	size_t    string_length = this->__non_standard__get_string_length();

	size_t    length_to_move;

	if ((index >= string_length) || (count == 0))
	{
		/* nothing to remove */
		return;
	}

	/* count must be non zero and index is inside string */

	/**
		* check if we need to move string after count or all string
		* after index are lost
		*/
	if ((index + count) >= string_length)
	{
		/* fall back to simple remove method */
		this->remove(index);

		/* operation finished */
		return;
	}

	/* if we reach here, there is string to move */
	length_to_move = string_length - (index + count);

	/* get current buffer */
	s             = this->__non_standard__c_str_non_const();
	buffer_length = this->__non_standard__get_buffer_length();

	/* move s to index */
	_s = s + index;

	/* move end of string to beginning */
	memmove(_s, _s + count, length_to_move);

	/* add '\0' */
	_s[length_to_move] = '\0';

	/* set buffer, buffer_length and s not changed */
	this->__non_standard__set_new_buffer(s, buffer_length, index + length_to_move);
}

void String::replace(const char   *substring1, const char   *substring2)
{
	char         *buffer = this->__non_standard__c_str_non_const();
	size_t        buffer_length = this->__non_standard__get_buffer_length();
	size_t        string_length = this->__non_standard__get_string_length();

	size_t        substring1_length;
	size_t        substring2_length;

	void        *_buffer = (void *) buffer;

	/* don't replacement anything if match or current string is empty */
	if ((buffer == NULL) || (buffer == (String::empty_string)) || (string_length == 0) ||
		(substring1 == NULL) || (substring1_length = strlen(substring1), substring1_length == 0))
	{
		/* nothing to replace */
		return;
	}

	/* guard againts empty replacement */
	if (substring2 == NULL)
	{
		/* when substring2_length set to 0, substring2 value don't relevant */
		substring2_length = 0;
	}
	else
	{
		/* we also allow substring2_length 0 here */
		substring2_length = strlen(substring2);
	}

	/* use mem_replace */
	mem_replace(&_buffer, &string_length, &buffer_length, substring1, substring1_length, substring2, substring2_length, true);

	/* make sure buffer_length > string_length */
	_ASSERT(buffer_length > string_length);

	/* set termination string, since we use mem-based replacement */
	*(((char *) _buffer) + string_length) = '\0';

	/* replace buffer without reallocation */
	this->__non_standard__set_new_buffer((char *) _buffer, buffer_length, string_length);
}

void String::replace(const char   *substring1, const String &substring2)
{
	char         *buffer = this->__non_standard__c_str_non_const();
	size_t        buffer_length = this->__non_standard__get_buffer_length();
	size_t        string_length = this->__non_standard__get_string_length();

	size_t        substring1_length;
	size_t        substring2_length = substring2.__non_standard__get_string_length();

	void        *_buffer = (void *) buffer;

	/* don't replacement anything if match or current string is empty */
	if ((buffer == NULL) || (buffer == (String::empty_string)) || (string_length == 0) ||
		(substring1 == NULL) || (substring1_length = strlen(substring1), substring1_length == 0))
	{
		/* nothing to replace */
		return;
	}

	/* use mem_replace */
	mem_replace(&_buffer, &string_length, &buffer_length, substring1, substring1_length, substring2.c_str(), substring2_length, true);

	/* make sure buffer_length > string_length */
	_ASSERT(buffer_length > string_length);

	/* set termination string, since we use mem-based replacement */
	*(((char *) _buffer) + string_length) = '\0';

	/* replace buffer without reallocation */
	this->__non_standard__set_new_buffer((char *) _buffer, buffer_length, string_length);
}

void String::replace(const String &substring1, const char   *substring2)
{
	char         *buffer = this->__non_standard__c_str_non_const();
	size_t        buffer_length = this->__non_standard__get_buffer_length();
	size_t        string_length = this->__non_standard__get_string_length();

	const char   *substring1_buffer        = substring1.c_str();
	size_t        substring1_string_length = substring1.__non_standard__get_string_length();

	/*size_t        substring1_length;*/
	size_t        substring2_length;

	void        *_buffer = (void *) buffer;

	/* don't replacement anything if match or current string is empty */
	if ((buffer == NULL) || (buffer == (String::empty_string)) || (string_length == 0) ||
		(substring1_buffer == NULL) || (substring1_buffer == (String::empty_string)) || (substring1_string_length == 0))
	{
		/* nothing to replace */
		return;
	}

	/* guard againts empty replacement */
	if (substring2 == NULL)
	{
		/* when substring2_length set to 0, substring2 value don't relevant */
		substring2_length = 0;
	}
	else
	{
		/* we also allow substring2_length 0 here */
		substring2_length = strlen(substring2);
	}

	/* use mem_replace */
	mem_replace(&_buffer, &string_length, &buffer_length, substring1_buffer, substring1_string_length, substring2, substring2_length, true);

	/* make sure buffer_length > string_length */
	_ASSERT(buffer_length > string_length);

	/* set termination string, since we use mem-based replacement */
	*(((char *) _buffer) + string_length) = '\0';

	/* replace buffer without reallocation */
	this->__non_standard__set_new_buffer((char *) _buffer, buffer_length, string_length);
}

void String::replace(const String &substring1, const String &substring2)
{
	char         *buffer = this->__non_standard__c_str_non_const();
	size_t        buffer_length = this->__non_standard__get_buffer_length();
	size_t        string_length = this->__non_standard__get_string_length();

	const char   *substring1_buffer        = substring1.c_str();
	size_t        substring1_string_length = substring1.__non_standard__get_string_length();

	/*size_t        substring1_length;*/
	size_t        substring2_length = substring2.__non_standard__get_string_length();

	void        *_buffer = (void *) buffer;

	/* don't replacement anything if match or current string is empty */
	if ((buffer == NULL) || (buffer == (String::empty_string)) || (string_length == 0) ||
		(substring1_buffer == NULL) || (substring1_buffer == (String::empty_string)) || (substring1_string_length == 0))
	{
		/* nothing to replace */
		return;
	}

	/* use mem_replace */
	mem_replace(&_buffer, &string_length, &buffer_length, substring1_buffer, substring1_string_length, substring2.c_str(), substring2_length, true);

	/* make sure buffer_length > string_length */
	_ASSERT(buffer_length > string_length);

	/* set termination string, since we use mem-based replacement */
	*(((char *) _buffer) + string_length) = '\0';

	/* replace buffer without reallocation */
	this->__non_standard__set_new_buffer((char *) _buffer, buffer_length, string_length);
}


/**
	* inplace replacement with buffer can be reallocate and buffer_length can be changed
	* return number of replacement occur, negative number if error happen
	* return 0 means no replacement
	* return -1 means error happen before any replacement can be done
	* return -2 means error happen before 2nd replacement can be done (1st replacement is success)
	* return -3 means error happen before 3rd replacement can be done (1st and 2nd replacement is success)
	* and so on
	* return 1 means 1 replacement is done
	* return 2 means 2 replacement is done
	* and so on
	* when error happen, buffer content and buffer len maybe changed, but allocation
	* is still valid and length is keep to be valid
	* content before replacemtn
	*/
static ssize_t mem_replace(void **buffer, size_t *content_length, size_t *buffer_length, const void *match, size_t match_length, const void *replacement, size_t replacement_length, bool content_is_string)
{
	/* call suitable replacement function based on match_length and replacement_length */
	if      (replacement_length == match_length)
	{
		return mem_replace_with_same_size(*buffer, *content_length, match, replacement, replacement_length);
	}
	else if (replacement_length  > match_length)
	{
		/* special handling for string, add 1 to content_length so zero termination is always allocated */
		size_t    extra_content_length = *content_length;
		ssize_t   retval;
		if (content_is_string)
		{
			extra_content_length++; /* increment extra_content_length */
		}
		retval = mem_replace_with_larger_size(buffer, &extra_content_length, buffer_length, match, match_length, replacement, replacement_length);
		*content_length = (content_is_string) ? extra_content_length - 1 : extra_content_length; /* decrement again if needed */
		return retval;
	}
	else  /* replacement_length  < match_length */
	{
		return mem_replace_with_smaller_size(*buffer, content_length, match, match_length, replacement, replacement_length);
	}
}

static ssize_t mem_replace_with_smaller_size(void *buffer, size_t *buffer_length, const void *match, size_t match_length, const void *replacement, size_t replacement_length)
{
	void                       *next_search_buffer        =   buffer;
	size_t                      next_search_buffer_length =  *buffer_length;

	size_t                      buffer_new_length = 0;

	/* token */
	struct search_token_s       token;

	size_t                      replacement_counter = 0;

	void                       *found;

	/* search and replace */
	while (found = mem_search_tokenize(next_search_buffer, next_search_buffer_length, match, match_length, &token), found != NULL)
	{
		/* replace only if replacement_length > 0, in case replacement_length == 0, replacement can be NULL */
		if (replacement_length > 0)
		{
			memcpy(found, replacement, replacement_length);
		}

		/* search next token */
		next_search_buffer        = (void *) (((uint8_t *) found) + replacement_length);
		next_search_buffer_length = token.next_content_length;

		memmove(next_search_buffer, token.next_content, token.next_content_length);

		/* increment counter */
		replacement_counter++;

		/* set buffer_new_length */
		buffer_new_length += (token.previous_content_length + replacement_length);
	}

	/* add (last) next_content_length only if replacement_counter bigger than 0 */
	if (replacement_counter > 0)
	{
		buffer_new_length += token.next_content_length;
	}

	/* set new buffer length */
	*buffer_length = buffer_new_length;

	/* return number of replacement */
	return (ssize_t) replacement_counter;
}

static ssize_t mem_replace_with_larger_size(void **buffer, size_t *content_length, size_t *buffer_length, const void *match, size_t match_length, const void *replacement, size_t replacement_length)
{
	void                       *next_search_buffer        = *buffer;
	size_t                      next_search_buffer_length = *content_length;

	void                       *current_buffer            = *buffer;
	size_t                      current_buffer_length     = *buffer_length;

	/*size_t                      delta                     = replacement_length - match_length;*/

	/* accumulation of processed content */
	size_t                      new_content_length = 0;

	/* token */
	struct search_token_s       token;

	size_t                      replacement_counter = 0;

	void                       *found;

	/* search and replace */
	while (found = mem_search_tokenize(next_search_buffer, next_search_buffer_length, match, match_length, &token), found != NULL)
	{
		/* found matching data */
		size_t        needed_space;

		/* increment replacement_counter before processing buffer */
		++replacement_counter;

		/* accumulate content length */
		new_content_length += token.previous_content_length;

		/* space needed to store data */
		needed_space = new_content_length + replacement_length + token.next_content_length;

		/* check if current buffer length is enough to store replacement */
		if (needed_space > current_buffer_length)
		{
			/* make current buffer same size as predicted content size/needed space */
			current_buffer_length = needed_space;

			/* realloc original buffer */
			current_buffer = realloc(current_buffer, current_buffer_length);
			if (current_buffer == NULL)
			{
				/**
					* failed to realloc, but current_buffer is left untouced
					* current_buffer should be already saved in previous loop run
					*/
				return ((ssize_t) replacement_counter) * (-1); /* return negative value of replacement counter */
			}

			/**
				* realloc success
				* we need to move pointer to new buffer
				*
				* pointer set by token is invalid
				* only rely on found
				*/
			found = (void *) (((uint8_t *) current_buffer) + new_content_length);
		}

		/* move data */
		memmove(((uint8_t *) found) + replacement_length, ((uint8_t *) found) + match_length, token.next_content_length);

		/* copy data */
		memcpy(found, replacement, replacement_length);

		/* setup new search point */
		next_search_buffer = (void *) (((uint8_t *) found) + replacement_length);
		next_search_buffer_length = token.next_content_length;

		/* save buffer every run */
		*buffer         = current_buffer;
		*buffer_length  = current_buffer_length;

		/* update content length with needed space */
		*content_length = needed_space;
	}

	/* content length, buffer, and buffer_length already set in loop */
	return (ssize_t) replacement_counter;
}

static ssize_t mem_replace_with_same_size(void *buffer, size_t buffer_length, const void *match, const void *replacement, size_t match_and_replacement_length)
{
	void                       *next_search_buffer        =  buffer;
	size_t                      next_search_buffer_length =  buffer_length;

	/* token */
	struct search_token_s       token;

	size_t                      replacement_counter = 0;

	void                       *found;

	/* search and replace */
	while (found = mem_search_tokenize(next_search_buffer, next_search_buffer_length, match, match_and_replacement_length, &token), found != NULL)
	{
		/* replace */
		memcpy(found, replacement, match_and_replacement_length);

		/* search next token */
		next_search_buffer        = token.next_content;
		next_search_buffer_length = token.next_content_length;

		/* increment counter */
		replacement_counter++;
	}

	/* return number of replacement */
	return (ssize_t) replacement_counter;
}

static void *mem_search_tokenize(const void *buffer, size_t buffer_length, const void *match, size_t match_length, struct search_token_s *token)
{
	/* use memmem to search for token */
	void *found = __non_standard__memmem(buffer, buffer_length, match, match_length);

	/* check return value */
	if (found == NULL)
	{
		/* not found */
		return NULL;
	}
	else
	{
		size_t previous_content_length = ((uint8_t *) found) - ((uint8_t *) buffer);

		/* previous content */
		token->previous_content        = (void *) buffer; /* remove qualifier */
		token->previous_content_length = previous_content_length;

		/* match content */
		token->match                   = found;
		token->match_length            = match_length;

		/* next content */
		token->next_content            = (void *) (((uint8_t *) found) + match_length);
		token->next_content_length     = buffer_length - match_length - previous_content_length;

		/* return found */
		return found;
	}
}


void String::toLowerCase(void)
{
	char *s;
	size_t i;
	for (i = 0, s = this->__non_standard__c_str_non_const(); i < (this->__non_standard__get_string_length()); ++i, ++s)
	{
		int s_lower = tolower((int) (*s));
		*s = (char) s_lower;
	}
}

void String::toUpperCase(void)
{
	char *s;
	size_t i;
	for (i = 0, s = this->__non_standard__c_str_non_const(); i < (this->__non_standard__get_string_length()); ++i, ++s)
	{
		int s_upper = toupper((int) (*s));
		*s = (char) s_upper;
	}
}

/**
	* this trim behave different from what specified by arduino
	* instead trim done at start and end of whole string, this
	* trim remove extra space and tab per line, so this trim works
	* for multiline string
	*/
void String::trim(void)
{
	char *s = this->__non_standard__c_str_non_const();

	char *read;
	char *write;

	if ((s == NULL) || (s == (String::empty_string)) || (this->__non_standard__get_string_length() == 0))
	{
		/* nothing to be done */
		return;
	}

	/* pointer to read and write in-place */
	read = s;
	write = s;

	/* trim as long we not found termination */
	while ((*read) != '\0')
	{
		char c;

		char *content_start;
		char *content_end;

		/* 1. Skip leading whitespace for the current line
		* We check for isspace but make sure we don't skip the newline itself! */
		while (c = (*read), (c != '\0') && (c != '\r') && (c != '\n') && isspace((int) c))
		{
			read++;
		}

		content_start = read;
		content_end = read;

		/* 2. Scan the line to find its end and track the last non-space character */
		while (c = (*read), (c != '\0') && (c != '\r') && (c != '\n'))
		{
			if (! isspace((int) c))
			{
				content_end = read + 1; /* Mark the position just after the last non-space */
			}
			read++;
		}

		/* 3. Copy the "useful" content of the line to the write position */
		while (content_start < content_end)
		{
			*write++ = *content_start++;
		}

		/* 4. If we stopped because of a newline, preserve it and move to next line */
		if (c = (*read), (c == '\r') || (c == '\n'))
		{
			*write++ = c;
			read++;
		}
	}

	/* 5. Always null-terminate the newly shortened string */
	*write = '\0';

	/**
		* set new buffer/string length
		* do not remove buffer in case of resulting string is zero length
		* because current buffer may allocated by reserve
		*/
	this->__non_standard__set_new_buffer(s, this->__non_standard__get_buffer_length(), write - s);
}
