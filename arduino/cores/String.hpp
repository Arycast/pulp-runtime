/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/**
	* this header file is intended to be included in C++ code
	* in case this header included in C-capable header,
	* wrap inclusion with __cplusplus definition check, e.g.:
	*
	* #ifdef __cplusplus
	* #include <String.hpp>
	* #endif
	*
	* this file is implementation of String() class of arduino
	* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/
	*/

#ifndef __ARDUINO_CORES_SSTRING_HPP__
#define __ARDUINO_CORES_SSTRING_HPP__

#ifdef SIMULATION_TEST
#include "simulation_test.h"
#else
#include "variables.h" /* for various data type, including native data type */
#include "pgmspace.h" /* for various string macro */
#endif

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

#include <stdio.h>

/* standard header c++ */
#include <exception> /* for std::terminate */

/* local header */


/**
	* class declaration
	*/
/* forward declare class */
class String;


/* implement binary operator as non-member function */

/**
	* operator +
	* ALWAYS RETURN BY VALUE EVEN IF WE DEALING WITH MOVE-SEMANTIC!
	*
	* operator + IS NON-COMMUTATIVE OPERATOR for this class, e.g. (a + b) != (b + a)
	*/

#if 0
String operator+(String    lhs, const String   &rhs);

#if ((__cplusplus) >= 201103L) /* C++11 */
String operator+(String    lhs, String        &&rhs);

String operator+(String  &&lhs, String          rhs);

String operator+(String  &&lhs, String        &&rhs);
#endif /* ((__cplusplus) >= 201103L) */


String operator+(const char *lhs, const String     &rhs);
#endif

String operator+(const char *lhs, String            rhs);

#if ((__cplusplus) >= 201103L) /* C++11 */
String operator+(const char *lhs, String          &&rhs);
#endif


/**
	* class definition
	*/
class String
{
private:
	static constexpr const char  *empty_string = "";
	static char                   dummy_char_storage;

	char                         *c_str_buf;
	size_t                        c_str_buf_len;
	size_t                        c_str_buf_strlen;

public:
	/**
		* part of big three
		* copy constructor
		*/
	/* constructors */
	String(void);
	String(const char *str);
	/* copy constructor */
	String(const String   &rvalue);
#if (__cplusplus >= 201103L)
	/* move constructor */
	String(String        &&rvalue);
#endif

	/* destructor */
	~String(void);


	/**
		* class method:
		*/

	/**
		* regular public class method
		*/

	/**
		* method charAt
		* Access a particular character of the String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/charAt
		*/
	inline char charAt(unsigned int n) const
	{
		return (*this)[n];
	}

	/**
		* method compareTo and compareToIgnoreCase
		* Compares two Strings, testing whether one comes before or
		* after the other, or whether they’re equal.
		* The strings are compared character by character,
		* using the ASCII values of the characters.
		* That means, for example, that a comes before b but after A.
		* Numbers come before letters.
		* in case of compareToIgnoreCase, case different is ignored
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/compareTo
		*/
	inline int compareTo(const char     *str, bool ignore_case = false) const /* not part of standard */
	{
		/* get c string representation of myString2 */
		const char  *lvalue_c_str         = this->c_str();
		size_t       lvalue_string_length = this->__non_standard__get_string_length();

		const char  *rvalue_c_str = str;
		/*size_t       rvalue_string_length;*/

		if      ((lvalue_string_length == 0) && (rvalue_c_str == NULL))
		{
			return 0;
		}
		else if ((lvalue_string_length  > 0) && (rvalue_c_str == NULL))
		{
			return 1;
		}
		else
		{
			/* overide string if it NULL */
			if (lvalue_c_str == NULL)
			{
				lvalue_c_str = String::empty_string;
			}

			/* use strncasecmp from pgmspace */
			if (ignore_case)
			{
				return strncasecmp_PF(lvalue_c_str, rvalue_c_str,
					lvalue_string_length + 1);
			}
			else
			{
				return strncmp(lvalue_c_str, rvalue_c_str,
					lvalue_string_length + 1);
			}
		}
	}

	inline int compareTo(const String   &myString2, bool ignore_case = false) const
	{
		size_t lvalue_string_length = this->__non_standard__get_string_length();
		size_t rvalue_string_length = myString2.__non_standard__get_string_length();

		if      ((lvalue_string_length == 0) && (rvalue_string_length == 0))
		{
			return 0;
		}
		else if ((lvalue_string_length == 0) && (rvalue_string_length  > 0))
		{
			return (-1);
		}
		else if ((lvalue_string_length  > 0) && (rvalue_string_length == 0))
		{
			return 1;
		}
		else
		{
			/* get c string representation of myString2 */
			const char *lvalue_c_str = this->c_str();
			const char *rvalue_c_str = myString2.c_str();

			/* overide string if it NULL */
			if (lvalue_c_str == NULL)
			{
				lvalue_c_str = String::empty_string;
			}

			if (rvalue_c_str == NULL)
			{
				rvalue_c_str = String::empty_string;
			}

			/* use strncasecmp from pgmspace */
			if (ignore_case)
			{
				return strncasecmp_PF(lvalue_c_str, rvalue_c_str,
					((lvalue_string_length > rvalue_string_length) ? rvalue_string_length : lvalue_string_length) + 1);
			}
			else
			{
				return strncmp(lvalue_c_str, rvalue_c_str,
					((lvalue_string_length > rvalue_string_length) ? rvalue_string_length : lvalue_string_length) + 1);
			}
		}
	}

	inline int compareToIgnoreCase(const char     *str) const /* not part of standard */
	{
		return this->compareTo(str, true);
	}

	inline int compareToIgnoreCase(const String   &myString2) const /* not part of standard */
	{
		return this->compareTo(myString2, true);
	}

	/**
		* method concat
		* Appends the parameter to a String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/concat
		*/
	inline bool concat(const char     *parameter)
	{
		/**
			* check if we try to concat with empty string or null
			* int that case, nothing should change in this instance
			*/
		/*if ((parameter == NULL) || (strlen(parameter) == 0))*/
		if ((parameter == NULL) || (parameter[0] == 0)) /* avoid strlen because it expensive */
		{
			/* return ok because we don't have to change anything */
			return true;
		}

		return this->__non_standard__concat_non_check_argument(parameter);
	}

	inline bool concat(char            parameter)
	{
		char s[] = {parameter, '\0'};
		return this->__non_standard__concat_non_check_argument(s);
	}

	/* uint8_t */
	inline bool concat(byte            parameter)
	{
		/* reuse unsigned int */
		return this->concat((unsigned int) parameter);
	}

	inline bool concat(int             parameter)
	{
		char s[12];
		int retval = snprintf(s, 12, "%d", parameter);
		if ((retval <= 0) || (retval >= 12))
		{
			return false;
		}

		return this->__non_standard__concat_non_check_argument(s);
	}

	inline bool concat(unsigned int    parameter)
	{
		char s[12];
		int retval = snprintf(s, 12, "%u", parameter);
		if ((retval <= 0) || (retval >= 12))
		{
			return false;
		}

		return this->__non_standard__concat_non_check_argument(s);
	}

	inline bool concat(long            parameter)
	{
		char s[22];
		int retval;

		/* snprintf defined by C99 */
		retval = snprintf(s, 22, "%ld", parameter);
		if ((retval <= 0) || (retval >= 22))
		{
			/**
				* based on standard (C99) snprintf failed
				* when return negative number or
				* more or equal to buffer length
				*/
			return false;
		}

		return this->__non_standard__concat_non_check_argument(s);
	}

	inline bool concat(unsigned long   parameter)
	{
		char s[22];
		int retval;

		retval = snprintf(s, 22, "%lu", parameter);
		if ((retval <= 0) || (retval >= 22))
		{
			return false;
		}

		return this->__non_standard__concat_non_check_argument(s);
	}

	inline bool concat(float           parameter)
	{
		return concat((double) parameter);
	}

	inline bool concat(double          parameter)
	{
		char s[40];
		int retval;

		retval = snprintf(s, 40, "%f", parameter);
		if ((retval <= 0) || (retval >= 40))
		{
			return false;
		}

		return this->__non_standard__concat_non_check_argument(s);
	}

	inline bool concat(const String   &parameter)
	{
		if (parameter.__non_standard__get_string_length() == 0)
		{
			return true;
		}

		return this->__non_standard__concat_non_check_argument(parameter);
	}

#if ((__cplusplus) >= 201103L)
	inline bool concat(String        &&parameter) /* non-standard */
	{
		if (parameter.__non_standard__get_string_length() == 0)
		{
			return true;
		}

		return this->__non_standard__concat_non_check_argument(parameter);
	}
#endif

	/* helper (implement as private method) */
private:
	inline bool __non_standard__concat_non_check_argument(const char     *parameter)
	{
		const char *s     = this->c_str();
		size_t      s_len = this->__non_standard__get_string_length();

		/* append parameter */
		(*this) += parameter;

		/* check if successful */
		if ((s != (this->c_str())) || (s_len != (this->__non_standard__get_string_length())))
		{
			return true; /* malloc/realloc happen */
		}
		else
		{
			/* unsuccesfully add string */
			return false;
		}
	}

	inline bool __non_standard__concat_non_check_argument(const String   &parameter)
	{
		const char *s     = this->c_str();
		size_t      s_len = this->__non_standard__get_string_length();

		/* append parameter */
		(*this) += parameter;

		/* check if successful */
		if ((s != (this->c_str())) || (s_len != (this->__non_standard__get_string_length())))
		{
			return true; /* malloc/realloc happen */
		}
		else
		{
			/* unsuccesfully add string */
			return false;
		}
	}

#if ((__cplusplus) >= 201103L)
	inline bool __non_standard__concat_non_check_argument(String        &&parameter)
	{
		const char *s     = this->c_str();
		size_t      s_len = this->__non_standard__get_string_length();

		/* append parameter */
		(*this) += parameter;

		/* check if successful */
		if ((s != (this->c_str())) || (s_len != (this->__non_standard__get_string_length())))
		{
			return true; /* malloc/realloc happen */
		}
		else
		{
			/* unsuccesfully add string */
			return false;
		}
	}
#endif

public:
	/**
		* method c_str
		* Converts the contents of a String as a C-style,
		* null-terminated string.
		* Note that this gives direct access to the internal String buffer
		* and should be used with care.
		* In particular, you should never modify the string through
		* the pointer returned. When you modify the String object,
		* or when it is destroyed, any pointer previously returned by c_str()
		* becomes invalid and should not be used any longer.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/c_str
		*/
	inline const char * c_str(void) const
	{
		const char *s = this->__non_standard__c_str_non_const();
		return (s == NULL) ? (String::empty_string) : s;
	}

	/**
		* same as c_str but without const qualifier
		* this method don't modify instance, therefore
		* can be called by constant instance
		*/
	inline       char * __non_standard__c_str_non_const(void) const
	{
		/**
			* check if current pointer refer to empty_string
			* if so, return NULL
			*/
		return this->c_str_buf;
	}


	/**
		* method endsWith
		* Tests whether or not a String ends with
		* the characters of another String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/endsWith
		*/
	bool endsWith(const char   *str) const; /* not part of arduino standard */
	inline bool endsWith(const String &myString2) const
	{
		/* get c string representation of myString2 */
		const char *str = myString2.c_str();

		/**
			* check if str is NULL;
			* if so, return false,
			* if str is not NULL, evaluate with endsWith string version
			*/
		return (str == NULL) ? (this->c_str() == NULL) : (this->endsWith(str));
	}

	/**
		* method equals
		* Compares two Strings for equality.
		* The comparison is case-sensitive,
		* meaning the String "hello" is not equal to the String "HELLO".
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/equals
		*/
	inline bool equals(const char    *str) const /* not part of standard */
	{
		/**
			* reuse compareTo(str) and check if it return 0
			* method compareTo should handle case of str (and c_str()) is NULL
			* gracefully
			*/
		return ((this->compareTo(str)) == 0);
	}
	inline bool equals(const String  &myString2) const
	{
		return this->equals(myString2.c_str());
	}

	/**
		* method equalsIgnoreCase
		* Compares two Strings for equality.
		* The comparison is not case-sensitive,
		* meaning the String("hello") is equal to the String("HELLO").
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/equalsIgnoreCase
		*/
	inline bool equalsIgnoreCase(const char    *str) const
	{
		return ((this->compareToIgnoreCase(str)) == 0);
	}
	inline bool equalsIgnoreCase(const String  &myString2) const
	{
		return this->equalsIgnoreCase(myString2.c_str());
	}

	/**
		* method getBytes
		* Copies the String’s characters to the supplied buffer.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/getBytes
		*/
	inline void getBytes(byte *buf, unsigned int len) const
	{
		char *buf_char_p = (char *) buf;
		this->toCharArray(buf_char_p, len);
	}

	/**
		* method indexOf
		* Locates a character or String within another String.
		* By default, searches from the beginning of the String,
		* but can also start from a given index, allowing for the locating
		* of all instances of the character or String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/indexOf
		*/
	inline unsigned int indexOf(char           val) const
	{
		return this->indexOf(val, (unsigned int) 0);
	}
	inline unsigned int indexOf(const char    *val) const /* not part of standard */
	{
		return this->indexOf(val, (unsigned int) 0);
	}
	inline unsigned int indexOf(const String  &val) const
	{
		return this->indexOf(val, (unsigned int) 0);
	}
	unsigned int indexOf(char           val, unsigned int from) const;
	unsigned int indexOf(const char    *val, unsigned int from) const; /* not part of standard */
	inline unsigned int indexOf(const String  &val, unsigned int from) const
	{
		return this->indexOf(val.c_str(), from);
	}

	/**
		* method lastIndexOf
		* Locates a character or String within another String.
		* By default, searches from the end of the String,
		* but can also work backwards from a given index,
		* allowing for the locating of all instances of the character or String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/lastIndexOf
		*/
	inline unsigned int lastIndexOf(char           val) const
	{
		return this->lastIndexOf(val, (unsigned int) 0);
	}
	inline unsigned int lastIndexOf(const char    *val) const
	{
		return this->lastIndexOf(val, (unsigned int) 0);
	}
	inline unsigned int lastIndexOf(const String  &val) const
	{
		return this->lastIndexOf(val, (unsigned int) 0);
	}
	unsigned int lastIndexOf(char           val, unsigned int from) const;
	unsigned int lastIndexOf(const char    *val, unsigned int from) const;
	inline unsigned int lastIndexOf(const String  &val, unsigned int from) const
	{
		return this->lastIndexOf(val.c_str(), from);
	}

	/**
		* method length
		* Returns the length of the String, in characters.
		* (Note that this doesn’t include a trailing null character.)
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/length
		*/
	unsigned int length(void) const;

	/**
		* method remove
		* Modify in place a String removing chars from the provided index
		* to the end of the String or from the provided index
		* to index plus count.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/remove
		*/
	void remove(unsigned int index);
	void remove(unsigned int index, unsigned int count);

	/**
		* method replace
		* The String replace() function allows you to replace all instances
		* of a given character with another character.
		* You can also use replace to replace substrings of
		* a String with a different substring.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/replace
		*/
	void replace(const char   *substring1, const char   *substring2); /* not part of arduino standard */
	inline void replace(const String &substring1, const String &substring2)
	{
		this->replace(substring1.c_str(), substring2.c_str());
	}

	/**
		* method reserve
		* The String reserve() function allows you to allocate
		* a buffer in memory for manipulating Strings.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/reserve
		*/
	int reserve(unsigned int size);

	/**
		* method setCharAt
		* Sets a character of the String.
		* Has no effect on indices outside the existing length of the String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/setCharAt
		*/
	void setCharAt(unsigned int index, char c);

	/**
		* method startsWith
		* Tests whether or not a String starts with the characters
		* of another String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/startsWith
		*/
	bool startsWith(const char    *str) const; /* not part of arduino standard */
	inline bool startsWith(const String  &myString2) const
	{
		return this->startsWith(myString2.c_str());
	}

	/**
		* method substring
		* Get a substring of a String.
		* The starting index is inclusive (the corresponding character
		* is included in the substring), but the optional ending index
		* is exclusive (the corresponding character
		* is not included in the substring).
		* If the ending index is omitted,
		* the substring continues to the end of the String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/substring
		*/
	String  substring(unsigned int from) const;
	String  substring(unsigned int from, unsigned int to) const;

	/**
		* method toCharArray
		* Copies the String’s characters to the supplied buffer.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/toCharArray
		*/
	void toCharArray(char *buf, unsigned int len) const;

	/**
		* method toDouble
		* Converts a valid String to a double.
		* The input String should start with a digit.
		* If the String contains non-digit characters,
		* the function will stop performing the conversion.
		* For example, the Strings "123.45", "123", and "123fish"
		* are converted to 123.45, 123.00, and 123.00 respectively.
		* \Note that "123.456" is approximated with 123.46.
		* Note too that floats have only 6-7 decimal digits of precision and
		* that longer Strings might be truncated.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/toDouble
		*/
	double toDouble(void) const;

	/**
		* method toInt
		* Converts a valid String to an integer.
		* The input String should start with an integer number.
		* If the String contains non-integer numbers,
		* the function will stop performing the conversion.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/toInt
		*/
	long toInt(void) const;

	/**
		* method toFloat
		* Converts a valid String to a float.
		* The input String should start with a digit.
		* If the String contains non-digit characters,
		* the function will stop performing the conversion.
		* For example, the Strings "123.45", "123", and "123fish"
		* are converted to 123.45, 123.00, and 123.00 respectively.
		* Note that "123.456" is approximated with 123.46.
		* Note too that floats have only 6-7 decimal digits of precision and
		* that longer Strings might be truncated.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/toFloat
		*/
	inline float toFloat(void) const
	{
		return (float) (this->toDouble());
	}

	/**
		* method toLowerCase
		* Get a lower-case version of a String.
		* The toLowerCase() function modifies the String in place
		* rather than returning a new one.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/toLowerCase
		*/
	void toLowerCase(void);

	/**
		* method toUpperCase
		* Get an upper-case version of a String.
		* The toUpperCase() modifies the String in place
		* rather than returning a new one.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/toUpperCase
		*/
	void toUpperCase(void);

	/**
		* method trim
		* Get a version of the String with any leading and
		* trailing whitespace removed. The trim() function modifies
		* the String in place rather than returning a new one.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/trim
		*/
	void trim(void);


	/**
		* operator overiding
		* member function
		*/

	/**
		* copy assignment operator (big three)
		* as part of initialization
		* e.g. String a = b;
		*
		* should return *this
		*/
	String &operator=(const char     *rvalue);
	/* copy semantic */
	String &operator=(const String   &rvalue);
#if (__cplusplus >= 201103L) /* C++11 */
	/* move semantic */
	String &operator=(String        &&rvalue);
#endif


	/**
		* operator [] (element access)
		* indexing with square bracket
		*
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/elementAccess"
		*/
	/* r-value operator */
	char   operator[](size_t index) const
	{
		/* get current instance string pointer */
		const char *s = this->c_str();

		/* check if buffer is empty */
		if ((s == NULL) || (s == (this->empty_string)))
		{
			return '\0';
		}
		else
		{
			/* check if we attempt to access out of bound memory */
			size_t s_len = strlen(s);

			/* in attempt of accessing out of bound memory, return '\0' */
			if (index >= s_len)
			{
				return '\0';
			}
			else
			{
				/* in bound memory */
				return s[index];
			}
		}
	}

	/**
		* l-value operator, although this method don't modify object
		* the nature of l-value operator is to be modified by r-value
		* therefore don't put const here
		*/
	char  &operator[](size_t index)
	{
		char *s = this->__non_standard__c_str_non_const();

		/* check if we try to write invalid buffer or read-only buffer */
		if ((s == NULL) || (((const char *) s) == (this->empty_string)))
		{
			/* set dummy storage to '\0' in case this operator used as rvalue */
			String::dummy_char_storage = '\0';

			/* return dummy storage */
			return (String::dummy_char_storage);
		}
		else
		{
			/* check if we attempt to access memory outside of buffer
			* (not outside of string) */
			if (index >= (this->__non_standard__get_buffer_length()))
			{
				/* set dummy storage to '\0' in case this operator used as rvalue */
				String::dummy_char_storage = '\0';

				/* return dummy storage */
				return (String::dummy_char_storage);
			}
			else
			{
				return s[index];
			}
		}
	}


	/**
		* operator + (concatenation)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/concatenation"
		*/
	/* with c string */
	inline String operator+(const char *rhs) const
	{
		/* create new instance, since + operator shouldn't modify lhs operand */
		String lhs(*this);

		/* reuse += operator (const char *) */
		return (lhs += rhs);
	}

	/* with copy semantic */
	inline String operator+(const String &rhs) const
	{
		String lhs(*this);

		/* reuse += operator for string */
		return (lhs += rhs);
	}

	/* with move semantic */
#if (__cplusplus >= 201103L)
	inline String operator+(String &&rhs) const
	{
		/*String String_rhs(rhs);*/
		String lhs(*this);

		/* reuse + operator for string */
		return (lhs += rhs);
	}
#endif


	/**
		* operator += (append)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/append"
		*/
	/**
		* with c string
		*/
	String &operator+=(const char *rvalue);

	/**
		* copy semantic
		*/
	String &operator+=(const String &rvalue);

	/**
		* move semantic
		*/
#if ((__cplusplus) >= 201103L) /* C++11 */
	String &operator+=(String &&rvalue);
#endif /* ((__cplusplus) >= 201103L) */


	/**
		* operator == (comparison)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/comparison"
		*/

	/**
		* operator > (greater than)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/greaterThan"
		*/

	/**
		* operator >= (greater than or equal to)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/greaterThanOrEqualTo"
		*/

	/**
		* operator < (less than)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/lessThan"
		*/

	/**
		* operator <= (less than or equal to)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/lessThanOrEqualTo"
		*/

	/**
		* operator != (different from)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/differentFrom"
		*/

	/**
		* set new string to this instance, new argument should be dynamically
		* allocated buffer (created with malloc, realloc, etc.)
		*
		* you can chain this function
		*/
	inline String & __non_standard__set_new_buffer(char *new_buffer, size_t buffer_length)
	{
		/* if new_buffer is NULL or buffer_length is 0,
		* then automatically set new_buffer to NULL */
		if ((new_buffer == NULL) || (new_buffer == ((char *) (String::empty_string))) ||
			(buffer_length == 0))
		{
			return this->__non_standard__set_new_buffer((char *) String::empty_string, 0, 0);
		}
		else
		{
			/**
				* if not, then calculate string length
				* to guarantee that strlen not accesing out-of-bound memory,
				* set end of buffer to '\0'
				*/
			new_buffer[buffer_length - 1] = '\0';

			/* we can safely use strlen here */
			return this->__non_standard__set_new_buffer(new_buffer, buffer_length, strlen(new_buffer));
		}
	}

	inline String & __non_standard__set_new_buffer(char *new_buffer, size_t buffer_length, size_t string_length)
	{
		/* string_length should less than buffer_length */
		if (string_length >= buffer_length)
		{
			string_length = (buffer_length <= 1) ? (0) : (buffer_length - 1);
		}

		this->c_str_buf = new_buffer;
		this->c_str_buf_len = buffer_length;
		this->c_str_buf_strlen = string_length;

		return (*this);
	}

	inline size_t      __non_standard__get_buffer_length(void) const
	{
		return (this->c_str_buf_len);
	}

	inline size_t      __non_standard__get_string_length(void) const
	{
		return (this->c_str_buf_strlen);
	}

	/**
		* you can chain this function
		*/
	inline String & __non_standard__invalidate_string(void)
	{
		this->__non_standard__set_new_buffer(NULL, 0);

		return (*this);
	}

	/**
		* you can chain this function
		*/
	inline String & __non_standard__free_string_non_invalidate(void)
	{
		/* remove const qualifier */
		void *s = this->__non_standard__c_str_non_const();

		/* check if current instance had allocate string */
		if ((s == NULL) || (s == ((void *) (this->empty_string))))
		{
			/* do nothing if this instance have empty string */
		}
		else
		{
			/* current instance had allocated buffer, free it */
			free(s);

			/**
				* this function intentionally don't invalidate!
				* call to c_str() still return free'd buffer
				* usage of this buffer after free is a memory leak
				*
				* to invalidate, call __non_standard__invalidate_string()
				*/
		}

		return (*this);
	}


	static inline constexpr const char *__non_standard__shared_empty_string(void)
	{
		return String::empty_string;
	}
};

#endif /* defined(__ARDUINO_CORES_SSTRING_HPP__) */
