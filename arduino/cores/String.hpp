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
/*#include "pgmspace.h"*/ /* for various string macro */
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
#include <string.h> /* strlen, memcpy, memmove, etc. */
#include <stdlib.h> /* alloc and free related functions */

/*#include <stdio.h>*/

#include <vector> /* for std::vector */
/* standard header c++ */
#if ((__cplusplus) >= 201103L) /* C++11 */
#include <type_traits> /* for std::is_same and std::enable_if */
#endif
#include <utility> /* for std::move */

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

String operator+(String    lhs, char            rhs);
String operator+(String    lhs, byte            rhs);
String operator+(String    lhs, int             rhs);
String operator+(String    lhs, unsigned int    rhs);
String operator+(String    lhs, long            rhs);
String operator+(String    lhs, unsigned long   rhs);
String operator+(String    lhs, float           rhs);
String operator+(String    lhs, double          rhs);

#if ((__cplusplus) >= 201103L) /* C++11 */
String operator+(String    lhs, String        &&rhs);

String operator+(String  &&lhs, String          rhs);

String operator+(String  &&lhs, char            rhs);
String operator+(String  &&lhs, byte            rhs);
String operator+(String  &&lhs, int             rhs);
String operator+(String  &&lhs, unsigned int    rhs);
String operator+(String  &&lhs, long            rhs);
String operator+(String  &&lhs, unsigned long   rhs);
String operator+(String  &&lhs, float           rhs);
String operator+(String  &&lhs, double          rhs);

String operator+(String  &&lhs, String        &&rhs);

#endif /* ((__cplusplus) >= 201103L) */


String operator+(const char *lhs, const String     &rhs);
#endif

/* copy semantic */
String operator+(const char *lhs, String            rhs);

String operator+(char           lhs, String  rhs);
String operator+(byte           lhs, String  rhs);
String operator+(int            lhs, String  rhs);
String operator+(unsigned int   lhs, String  rhs);
String operator+(long           lhs, String  rhs);
String operator+(unsigned long  lhs, String  rhs);
String operator+(float          lhs, String  rhs);
String operator+(double         lhs, String  rhs);

/* move semantic */
#if ((__cplusplus) >= 201103L) /* C++11 */
String operator+(const char *lhs, String          &&rhs);

/**
	* don't need to provide move semantic, because it don't make sense
	* to use move semantic
	* if we use move semantic, we need to allocate/create
	* new object anyway, so just let compiler allocate it
	* automatically with copy semantic
	*/
/*String operator+(char           lhs, String       &&rhs);
String operator+(byte           lhs, String       &&rhs);
String operator+(int            lhs, String       &&rhs);
String operator+(unsigned int   lhs, String       &&rhs);
String operator+(long           lhs, String       &&rhs);
String operator+(unsigned long  lhs, String       &&rhs);
String operator+(float          lhs, String       &&rhs);
String operator+(double         lhs, String       &&rhs);*/
#endif


/**
	* operator ==
	*/
bool operator==(const char *lhs, const String &rhs);

/**
	* operator >
	*/
bool operator>(const char *lhs, const String &rhs);

/**
	* operator >=
	*/
bool operator>=(const char *lhs, const String &rhs);

/**
	* operator <
	*/
bool operator<(const char *lhs, const String &rhs);

/**
	* operator <=
	*/
bool operator<=(const char *lhs, const String &rhs);

/**
	* operator !=
	*/
bool operator!=(const char *lhs, const String &rhs);

/**
	* class definition
	*/
class String
{
private:
	static constexpr const char  *empty_string = "";
	/*static char                   dummy_char_storage;*/

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
	String(char          value);
	String(byte          value);
	String(byte          value, unsigned int base);
	String(int           value);
	String(int           value, unsigned int base);
	String(unsigned int  value);
	String(unsigned int  value, unsigned int base);
	explicit String(long          value);
	explicit String(long          value, unsigned int base);
	explicit String(unsigned long value);
	explicit String(unsigned long value, unsigned int base);
	String(float         value);
	String(float         value, unsigned int precision);
	String(double        value);
	String(double        value, unsigned int precision);
	/* copy constructor */
	String(const String   &rvalue);
#if ((__cplusplus) >= 201103L)
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
		return (*this)[(size_t) n];
	}

#if ((__cplusplus) >= 201103L) /* C++11 */
	template <typename T>
	inline typename std::enable_if<                     /* use SFINAE (Substitution Failure Is Not An Error). */
		std::is_same<T, size_t>::value &&               /* T must be size_t */
		! std::is_same<size_t, unsigned int>::value,    /* size_t must NOT be unsigned int */
		char                                            /* The actual return type */
	>::type
	charAt(T n) const /* non-standard */
	{
		return (*this)[n];
	}
#else
	inline char charAt(unsigned long n) const /* non-standard */
	{
		return (*this)[(size_t) n];
	}
#endif

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

			/* if we ignore case, use strncasecmp instead of strncmp */
			if (ignore_case)
			{
				return strncasecmp(lvalue_c_str, rvalue_c_str,
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

			/* if we ignore case, use strncasecmp instead of strncmp */
			if (ignore_case)
			{
				return strncasecmp(lvalue_c_str, rvalue_c_str,
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

		return this->__non_standard__concat_wrapper<const char *>(parameter);
	}

	inline bool concat(char            parameter)
	{
		char s[] = {parameter, '\0'};
		return this->__non_standard__concat_wrapper<const char *>(s);
	}

	/* uint8_t */
	inline bool concat(byte            parameter)
	{
		return this->__non_standard__concat_wrapper<byte>(parameter);
	}

	inline bool concat(int             parameter)
	{
		return this->__non_standard__concat_wrapper<int>(parameter);
	}

	inline bool concat(unsigned int    parameter)
	{
		return this->__non_standard__concat_wrapper<unsigned int>(parameter);
	}

	inline bool concat(long            parameter)
	{
		return this->__non_standard__concat_wrapper<long>(parameter);
	}

	inline bool concat(unsigned long   parameter)
	{
		return this->__non_standard__concat_wrapper<unsigned long>(parameter);
	}

	inline bool concat(float           parameter)
	{
		return this->__non_standard__concat_wrapper<float>(parameter);
	}

	inline bool concat(double          parameter)
	{
		return this->__non_standard__concat_wrapper<double>(parameter);
	}

	inline bool concat(const String   &parameter)
	{
		if (parameter.__non_standard__get_string_length() == 0)
		{
			return true;
		}

		return this->__non_standard__concat_wrapper<const String &>(parameter);
	}

#if ((__cplusplus) >= 201103L)
	inline bool concat(String        &&parameter) /* non-standard */
	{
		if (parameter.__non_standard__get_string_length() == 0)
		{
			return true;
		}

		return this->__non_standard__concat_wrapper<String &&>(std::move(parameter));
	}
#endif

	/* helper (implement as private method) */
private:
	template <typename T>
	inline bool __non_standard__concat_wrapper(T parameter)
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

#if 0
#if ((__cplusplus) >= 201103L)
	inline bool __non_standard__concat_wrapper_move_semantic(String        &&parameter)
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
#endif /* ((__cplusplus) >= 201103L) */
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

private:
	/**
		* helper code for matching start or end string
		* to be used by startsWith and endsWith
		*
		* if @arg check_end == true, then checking is done in the beginning of the string
		* if @arg check_end == false, then checking is done in the end of the string
		* if @arg s_len < 0, then length of string @arg s will be calculated
		* if @arg s_len >= 0, then matching will be done without calculating string @arg s length
		* @arg s length is assumed to be @arg s_len
		* this function accept @arg s is NULL, in that case @arg s_len is ignored and function is instantly return true
		* this function also accept @arg s_len is 0 and @arg s is empty string (only contain '\0') which also
		* this function instatly return true
		*/
	inline bool __non_standard__compare_edge_segment(const char *s, ssize_t s_len = -1, bool check_end = false) const
	{
		size_t   _s_len;

		/* this instance */
		const char    *instance_c_str = this->c_str();
		size_t         instance_len   = this->__non_standard__get_string_length();

		if ((s == NULL) || (s_len == 0))
		{
			/* must be true if compare with empty string */
			return true;
		}

		/* check if we need to calculate string length */
		if (s_len < 0)
		{
			/* re-calculate string length */
			_s_len = strlen(s);

			/* check if string is empty */
			if (_s_len == 0)
			{
				return true;
			}
		}
		else
		{
			_s_len = (size_t) s_len; /* s_len > 0 */
		}

		/* check current instance */
		if ((instance_c_str == NULL) || (instance_c_str == (String::empty_string)) ||
			(instance_len == 0) || (instance_len < _s_len))
		{
			/* must be false because we try to compare non-empty string
			* with this empty instance */
			return false;
		}

		/* check if we compare with start of string, or end of string */
		if (check_end)
		{
			instance_c_str += (instance_len - _s_len);
		}

		if (memcmp(instance_c_str, s, _s_len * sizeof(char)) == 0)
		{
			return true; /* when memcmp return 0, string is match */
		}
		else
		{
			return false;
		}
	}

public:
	/**
		* method endsWith
		* Tests whether or not a String ends with
		* the characters of another String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/endsWith
		*/
	inline bool endsWith(const char   *str) const /* not part of arduino standard */
	{
		return this->__non_standard__compare_edge_segment(str, -1, true);
	}

	inline bool endsWith(const String &myString2) const
	{
		return this->__non_standard__compare_edge_segment(myString2.c_str(),
			myString2.__non_standard__get_string_length(), true);
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
		/* reuse compareTo method */
		int retval = this->compareTo(str);
		return (retval == 0);
	}
	inline bool equals(const String  &myString2) const
	{
		/* reuse compareTo method */
		int retval = this->compareTo(myString2);
		return (retval == 0);
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
		/* reuse compareTo method */
		int retval = this->compareToIgnoreCase(str);
		return (retval == 0);
	}
	inline bool equalsIgnoreCase(const String  &myString2) const
	{
		int retval = this->compareToIgnoreCase(myString2);
		return (retval == 0);
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
	unsigned int indexOf(const String  &val, unsigned int from) const;

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
		size_t string_length = this->__non_standard__get_string_length();
		if (string_length <= 0)
		{
			/* nothing to search when string length is 0 */
			return (-1);
		}

		/* search from end index of string */
		return this->lastIndexOf(val, (unsigned int) (string_length - 1));
	}
	inline unsigned int lastIndexOf(const char    *val) const
	{
		size_t string_length = this->__non_standard__get_string_length();
		if (string_length <= 0)
		{
			/* nothing to search when string length is 0 */
			return (-1);
		}

		/* search from end index of string */
		return this->lastIndexOf(val, (unsigned int) (string_length - 1));
	}
	inline unsigned int lastIndexOf(const String  &val) const
	{
		size_t string_length = this->__non_standard__get_string_length();
		if (string_length <= 0)
		{
			/* nothing to search when string length is 0 */
			return (-1);
		}

		/* search from end index of string */
		return this->lastIndexOf(val, (unsigned int) (string_length - 1));
	}
	unsigned int lastIndexOf(char           val, unsigned int from) const;
	unsigned int lastIndexOf(const char    *val, unsigned int from) const;
	unsigned int lastIndexOf(const String  &val, unsigned int from) const;

	/**
		* method length
		* Returns the length of the String, in characters.
		* (Note that this doesn’t include a trailing null character.)
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/length
		*/
	unsigned int length(void) const
	{
		return (unsigned int) (this->__non_standard__get_string_length());
	}

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
	void replace(const char   *substring1, const String &substring2);
	void replace(const String &substring1, const char   *substring2);
	void replace(const String &substring1, const String &substring2);

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
	inline void setCharAt(unsigned int index, char c)
	{
		(*this)[(size_t) index] = c;
	}

#if ((__cplusplus) >= 201103L) /* C++11 */
	template <typename T>
	inline typename std::enable_if<                     /* use SFINAE (Substitution Failure Is Not An Error). */
		std::is_same<T, size_t>::value &&               /* T must be size_t */
		! std::is_same<size_t, unsigned int>::value,    /* size_t must NOT be unsigned int */
		void                                            /* The actual return type */
	>::type
	setCharAt(T index, char c) const /* non-standard */
	{
		(*this)[index] = c;
	}
#else
	inline void setCharAt(unsigned long index, char c) const /* non-standard */
	{
		(*this)[(size_t) index] = c;
	}
#endif


	/**
		* method startsWith
		* Tests whether or not a String starts with the characters
		* of another String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/startsWith
		*/
	inline bool startsWith(const char    *str) const /* not part of arduino standard */
	{
		return this->__non_standard__compare_edge_segment(str);
	}

	inline bool startsWith(const String  &myString2) const
	{
		return this->__non_standard__compare_edge_segment(
			myString2.c_str(), myString2.__non_standard__get_string_length());
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
	inline String  substring(unsigned int from) const
	{
		size_t len = this->__non_standard__get_string_length();
		return substring(from, (unsigned int) len);
	}

	inline String  substring(unsigned int from, unsigned int to) const
	{
		size_t len = this->__non_standard__get_string_length();

		/* if argument to is beyond len, then trim it to len  */
		if (((size_t) to) > len)
		{
			to = (unsigned int) len;
		}

		/* check if from is smaller than to (character to is not included in substring) */
		if (from < to)
		{
			const size_t character_to_copy = to - from;

			/* Dynamically allocates a char array of size character to copy + 1 */
			std::vector<char> buffer(character_to_copy + 1);

			/* to make memcpy work here, don't resize vector! */

			/* copy from source buffer */
			memcpy(buffer.data(), this->c_str() + from, character_to_copy * sizeof(char));

			/* add string termination */
			buffer[character_to_copy] = '\0';

			/* create object */
			do
			{
				String s(buffer.data());
				return s;
			}
			while (0);
		}
		else
		{
			/* return new empty string */
			do
			{
				String s;
				return s;
			}
			while (0);
		}
	}

	/**
		* method toCharArray
		* Copies the String’s characters to the supplied buffer.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/toCharArray
		*/
	void toCharArray(char *buf, unsigned int len) const
	{
		const char    *instance_c_str = this->c_str();
		size_t         instance_len   = this->__non_standard__get_string_length();

		size_t        _len = (size_t) len;

		/* no storage to copy */
		if ((buf == NULL) || (_len == 0))
		{
			return;
		}

		/* check current instance */
		if ((instance_c_str == NULL) || (instance_c_str == (String::empty_string)) || (instance_len == 0) || (_len == 1))
		{
			buf[0] = '\0'; /* just write string termination */
			return;
		}

		/* check if destination buffer has enough space */
		if (_len > instance_len)
		{
			/* shrink len */
			_len = instance_len + 1; /* add string termination */
		}

		/* copy string up to len - 1 */
		memcpy(buf, instance_c_str, _len - 1);

		/* add termination string */
		buf[_len - 1] = '\0';
	}


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
	String &operator=(char            rvalue);
	String &operator=(byte            rvalue);
	String &operator=(int             rvalue);
	String &operator=(unsigned int    rvalue);
	String &operator=(long            rvalue);
	String &operator=(unsigned long   rvalue);
	String &operator=(double          rvalue);
	String &operator=(float           rvalue);
	/* copy semantic */
	String &operator=(const String   &rvalue);
#if ((__cplusplus) >= 201103L) /* C++11 */
	/* move semantic */
	String &operator=(String        &&rvalue);
#endif


	/**
		* operator [] (element access)
		* indexing with square bracket
		*
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/elementAccess"
		*/

	/**
		* this proxy class is made as work around for possibility of
		* user changing string data directly
		*
		* e.g.
		* if s string value now is "0123456789", and user decide to do:
		* s[4] = '\0'
		* then we need to change c_str_buf_strlen
		* it is not possible if we just return char pointer to user and user can modify it as their wish
		*
		* but in case user doing some pointer magic like:
		* char *p = &(s[4]);
		* *p = '\0';
		* then call to method length() will be different from strlen(s.c_str())
		* this behaviour is well defined and expected
		* because String is container class and not plain array
		*/
	struct __non_standard__indexing_proxy_s
	{
		String  &parent;
		size_t   selected_index;

		/* L-VALUE: This is called when we write to the index */
		inline __non_standard__indexing_proxy_s &operator=(char value)
		{
			/* get current instance of c string */
			char       *s         = (this->parent).__non_standard__c_str_non_const();
			size_t      s_buf_len = (this->parent).__non_standard__get_buffer_length();
			size_t      s_str_len = (this->parent).__non_standard__get_string_length();

			/* check if we try to write invalid buffer or read-only buffer */
			if ((s == NULL) || (((const char *) s) == (String::empty_string)))
			{
				/* set dummy storage to '\0' in case this operator used as rvalue */
				/*String::dummy_char_storage = value;*/
			}
			else
			{
				/* check if we attempt to access memory outside of buffer
				* (not outside of string) */
				if      ((this->selected_index) >= s_buf_len)
				{
					/* user attempt to write to index outside buffer
					* do nothing */
				}
				else if ((this->selected_index)  > s_str_len)
				{
					/* inside buffer, but outside string, just write data */
					s[this->selected_index] = value;
				}
				else if ((this->selected_index) == s_str_len)
				{
					/* user attempt to write to end of string and just before buffer end */
					if (value == '\0')
					{
						/* it is ok only to write termination */
						s[this->selected_index] = '\0';
					}
					else
					{
						/* get next index */
						size_t next_index = (this->selected_index) + 1;

						/* should not write anything to end of string ONLY IF next_index is buffer end */
						if (next_index == s_buf_len)
						{
							/* do nothing */
						}
						else
						{
							/* it is ok to write anything, but make sure to add '\0' after the string */
							s[this->selected_index] = value; /* can write any character */
							s[          next_index] = '\0'; /* add termination */
							(this->parent).__non_standard__set_new_buffer(s, s_buf_len, next_index);
						}
					}
				}
				else /* ((this->selected_index)  < s_str_len) */
				{
					/* we need to modify character inside buffer */
					if (value == '\0')
					{
						/* somehow user want to add '\0', this may cause string length to change */
						s[this->selected_index] = '\0';

						/* user insert '\0' inside string, update string length */
						(this->parent).__non_standard__set_new_buffer(s, s_buf_len, this->selected_index);
					}
					else
					{
						/* user want to add '\0' outside string buffer */
						s[this->selected_index] = value;
					}
				}
			}

			return (*this); /* return struct instance and not String instance */
		}

		/* R-VALUE: This is called when we read from the index */
		inline operator char() const /* return type is automatically char */
		{
			/**
				* reuse charAt
				* why use charAt instead of implement indexing or use operator indexing?
				* we already implement operator indexing with const instance (r-value)
				* but that operator only able to use when instance is const
				* in struct, parent is not const, so by calling charAt which then
				* call r-value operator[]
				*/
			return (this->parent).charAt(this->selected_index);
		}
	}; /* we need to add ';' to struct declaration */


	/* r-value operator */
	char   operator[](size_t index) const
	{
		/* in case we use r-value operator, don't need to use proxy */

		/* get current instance string pointer */
		const char *s         = this->c_str();
		size_t      s_buf_len = this->__non_standard__get_buffer_length();

		/* check if buffer is empty */
		if ((s == NULL) || (s == (String::empty_string)) || (index >= s_buf_len))
		{
			return '\0';
		}
		else
		{
			/**
				* faithfully return any character inside buffer, even if the selected character
				* is outside string
				*/
			return s[index];
		}
	}

	/**
		* l-value operator, although this method don't modify object
		* the nature of l-value operator is to be modified by r-value
		* therefore don't put const here
		*/
	inline struct __non_standard__indexing_proxy_s  operator[](size_t index)
	{
#if ((__cplusplus) >= 202002L)
		struct String::__non_standard__indexing_proxy_s proxy = {.parent = (*this), .selected_index = index};
#else
		struct String::__non_standard__indexing_proxy_s proxy = {*this, index};
#endif
		return proxy;
	}


	/**
		* operator + (concatenation)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/concatenation"
		*/
private:
	template <typename T>
	String __non_standard__operator_plus_wrapper(T rhs) const
	{
		/* create new instance (copy semantic), since + operator shouldn't modify lhs operand */
		String lhs(*this);

		/* reuse += operator */
		return (lhs += rhs);
	}

#if 0
#if ((__cplusplus) >= 201103L) /* C++11 */
	String __non_standard__operator_plus_wrapper_move_semantic(String  &&rhs) const
	{
		/* create new instance (copy semantic), since + operator shouldn't modify lhs operand */
		String lhs(*this);

		/* reuse += operator */
		return (lhs += rhs);
	}
#endif /* ((__cplusplus) >= 201103L) */
#endif

public:
	/* with c string */
	inline String operator+(const char *rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<const char *>(rhs);
	}

	/* with copy semantic */
	inline String operator+(const String &rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<const String &>(rhs);
	}

	/* with move semantic */
#if ((__cplusplus) >= 201103L)
	inline String operator+(String &&rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<String &&>(std::move(rhs));
	}
#endif

	inline String operator+(char          rhs) const
	{
		char s[] = {rhs, '\0'};
		return ((*this) + s); /* reuse operator+ for string */
	}

	/* uint8_t */
	inline String operator+(byte          rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<byte>(rhs);
	}

	inline String operator+(int           rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<int>(rhs);
	}

	inline String operator+(unsigned int  rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<unsigned int>(rhs);
	}

	inline String operator+(long          rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<long>(rhs);
	}

	inline String operator+(unsigned long rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<unsigned long>(rhs);
	}

	inline String operator+(float         rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<float>(rhs);
	}

	inline String operator+(double        rhs) const
	{
		return this->__non_standard__operator_plus_wrapper<double>(rhs);
	}

	/**
		* operator += (append)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/append"
		*/
	/**
		* with c string
		*/
	String &operator+=(const char *rvalue);

	/**
		* with integer types
		*/
	inline String &operator+=(char           rvalue)
	{
		char s[] = {rvalue, '\0'};
		return ((*this) += s); /* reuse operator += with c string rvalue */
	}

	String &operator+=(byte           rvalue);

	String &operator+=(int            rvalue);

	String &operator+=(unsigned int   rvalue);

	String &operator+=(long           rvalue);

	String &operator+=(unsigned long  rvalue);

	/**
		* with float types
		*/
	String &operator+=(float          rvalue);

	String &operator+=(double         rvalue);


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
	inline bool operator==(const char *rvalue) const
	{
		return this->equals(rvalue);
	}

	inline bool operator==(const String &rvalue) const
	{
		return this->equals(rvalue);
	}

	/**
		* operator > (greater than)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/greaterThan"
		*/
	inline bool operator>(const char *rvalue) const
	{
		/* reuse compareTo method */
		int retval = this->compareTo(rvalue);
		return (retval > 0);
	}

	inline bool operator>(const String &rvalue) const
	{
		int retval = this->compareTo(rvalue);
		return (retval > 0);
	}

	/**
		* operator >= (greater than or equal to)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/greaterThanOrEqualTo"
		*/
	inline bool operator>=(const char *rvalue) const
	{
		int retval = this->compareTo(rvalue);
		return (retval >= 0);
	}

	inline bool operator>=(const String &rvalue) const
	{
		int retval = this->compareTo(rvalue);
		return (retval >= 0);
	}

	/**
		* operator < (less than)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/lessThan"
		*/
	inline bool operator<(const char *rvalue) const
	{
		int retval = this->compareTo(rvalue);
		return (retval < 0);
	}

	inline bool operator<(const String &rvalue) const
	{
		int retval = this->compareTo(rvalue);
		return (retval < 0);
	}

	/**
		* operator <= (less than or equal to)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/lessThanOrEqualTo"
		*/
	inline bool operator<=(const char *rvalue) const
	{
		int retval = this->compareTo(rvalue);
		return (retval <= 0);
	}

	inline bool operator<=(const String &rvalue) const
	{
		int retval = this->compareTo(rvalue);
		return (retval <= 0);
	}

	/**
		* operator != (different from)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/differentFrom"
		*/
	inline bool operator!=(const char *rvalue) const
	{
		return ! (this->equals(rvalue));
	}

	inline bool operator!=(const String &rvalue) const
	{
		return ! (this->equals(rvalue));
	}

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
			return this->__non_standard__set_new_buffer((char *) (String::empty_string), 0, 0);
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

	inline size_t   __non_standard__get_buffer_length(void) const
	{
		return (this->c_str_buf_len);
	}

	inline size_t   __non_standard__get_string_length(void) const
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
		if ((s == NULL) || (s == ((void *) (String::empty_string))))
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


/**
	* for testing
	*/
#ifdef SIMULATION_TEST
size_t unsigned_char_to_string_export(char *str, size_t str_len, unsigned char value, unsigned int base);
size_t int_to_string_export(char *str, size_t str_len, int value, unsigned int base);
size_t unsigned_int_to_string_export(char *str, size_t str_len, unsigned int value, unsigned int base);
size_t long_to_string_export(char *str, size_t str_len, long value, unsigned int base);
size_t unsigned_long_to_string_export(char *str, size_t str_len, unsigned long value, unsigned int base);
#endif

#endif /* defined(__ARDUINO_CORES_SSTRING_HPP__) */
