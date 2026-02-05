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

#include "variables.h" /* for various data type, including native data type */
#include "pgmspace.h" /* for various string macro */

/**
	* string class
	*/
class String
{
public:
	/**
		* part of big three
		* copy constructor
		*/
	String(const String   &myString2);
#if (__cplusplus >= 201103L)
	/* move constructor */
	String(const String  &&myString2)
#endif
	~String();


	/**
		* class method:
		*/

public:

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
		if (n > this.length())
		{
			return '\0';
		}
		else
		{
			const char *str = this.c_str();
			return (str == NULL) ? '\0' : str[n];
		}
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
	int compareTo(const char     *str) const; /* not part of standard */
	inline int compareTo(const String   &myString2) const
	{
		/* get c string representation of myString2 */
		const char *str = myString2.c_str();

		/* if str NULL, then check if self/this NULL too */
		return (str == NULL) ? (this.c_str() == NULL) : this.compareTo(str);
	}
	int compareToIgnoreCase(const char     *str) const; /* not part of standard */
	inline int compareToIgnoreCase(const String   &myString2) const /* not part of standard */
	{
		/* get c string representation of myString2 */
		const char *str = myString2.c_str();

		/* if str NULL, then check if self/this NULL too */
		return (str == NULL) ?
			(this.c_str() == NULL) :
			this.compareToIgnoreCase(str);
	}

	/**
		* method concat
		* Appends the parameter to a String.
		*
		* https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Functions/concat
		*/
	bool concat(const char     *parameter);
	bool concat(char            parameter);
	bool concat(byte            parameter); /* uint8_t */
	bool concat(int             parameter);
	bool concat(unsigned int    parameter);
	bool concat(long            parameter);
	bool concat(unsigned long   parameter);
	bool concat(float           parameter);
	bool concat(double          parameter);
	inline bool concat(const String   &parameter)
	{
		/**
			* parameter may return NULL, concat(const char *s) should handle
			* argument is NULL case gracefully
			*/
		return this.concat(parameter.c_str());
	}


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
	const char * c_str(void) const;

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
		return (str == NULL) ? (this.c_str() == NULL) : this.endsWith(str);
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
		return (this.compareTo(str) == 0);
	}
	inline bool equals(const String  &myString2) const
	{
		return this.equals(myString2.c_str());
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
		return (this.compareToIgnoreCase(str) == 0);
	}
	inline bool equalsIgnoreCase(const String  &myString2) const
	{
		return this.equalsIgnoreCase(myString2.c_str());
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
		this.toCharArray(buf_char_p, len);
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
		return this.indexOf(val, (unsigned int) 0);
	}
	inline unsigned int indexOf(const char    *val) const /* not part of standard */
	{
		return this.indexOf(val, (unsigned int) 0);
	}
	inline unsigned int indexOf(const String  &val) const
	{
		return this.indexOf(val, (unsigned int) 0);
	}
	unsigned int indexOf(char           val, unsigned int from) const;
	unsigned int indexOf(const char    *val, unsigned int from) const; /* not part of standard */
	inline unsigned int indexOf(const String  &val, unsigned int from) const
	{
		return this.indexOf(val.c_str(), from);
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
		return this.lastIndexOf(val, (unsigned int) 0);
	}
	inline unsigned int lastIndexOf(const char    *val) const
	{
		return this.lastIndexOf(val, (unsigned int) 0);
	}
	inline unsigned int lastIndexOf(const String  &val) const
	{
		return this.lastIndexOf(val, (unsigned int) 0);
	}
	unsigned int lastIndexOf(char           val, unsigned int from) const;
	unsigned int lastIndexOf(const char    *val, unsigned int from) const;
	inline unsigned int lastIndexOf(const String  &val, unsigned int from) const
	{
		return this.lastIndexOf(val.c_str(), from);
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
		this.replace(substring1.c_str(), substring2.c_str());
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
		return this.startsWith(myString2.c_str());
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
		return (float) this.toDouble();
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
	String &operator=(const String &rhs);
#if (__cplusplus >= 201103L) /* C++11 */
	/* move assignment operator */
	String &operator=(const String &&rhs);
#endif


	/**
		* operator [] (element access)
		* indexing with square bracket
		*
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/elementAccess"
		*/
	/* r-value operator */
	char   operator[](size_t index) const;
	/* l-value operator */
	char  &operator[](size_t index);


	/**
		* operator + (concatenation)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/concatenation"
		*/

	/**
		* operator += (append)
		* "https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/Operators/append"
		*/

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
};

/* implement binary operator as non-member function */

/**
	* operator +
	*/

#endif /* defined(__ARDUINO_CORES_SSTRING_HPP__) */
