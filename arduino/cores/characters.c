#include <stdbool.h>
#include <ctype.h>

#include "characters.h"

/**
	* Analyze if a char is alpha (that is a letter).
	* Returns true if the input char contains a letter.
	*/
bool isAlpha(char thisChar)
{
	/**
		* if thisChar is NOT an alpha character, then isalpha will return 0
		* then if we compare the return value with fixed value 0,
		* we can only return false (because thisChar is not an alpha character)
		* if we compare it "not equal to" fixed value 0
		*/
	return (isalpha(thisChar) != 0);
}

/**
	* Analyze if a char is alphanumeric (that is a letter or a number).
	* Returns true if the input char contains either a number or a letter.
	*/
bool isAlphaNumeric(char thisChar)
{
	return (isalnum(thisChar) != 0);
}

/**
	* Analyze if a char is ASCII.
	* Returns true if the input char contains an ASCII character.
	*/
bool isAscii(char thisChar)
{
	/* we can only use isascii() in POSIX.1-2001 with XSI */
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L) && \
	defined(_XOPEN_SOURCE))
	/* use XSI-compliant function */
	return (isascii(thisChar) != 0);
#else
	/* XSI standard is not available nor used */
	return ((((int) thisChar) >= 0) && (((int) thisChar) <= 127));
#endif
}

/**
	* Analyze if a char is a control character.
	* Returns true if the input char is a control character.
	*/
bool isControl(char thisChar)
{
	return (iscntrl(thisChar) != 0);
}

/**
	* Analyze if a char is a digit (that is a number).
	* Returns true if the input char is a number.
	*/
bool isDigit(char thisChar)
{
	return (isdigit(thisChar) != 0);
}

/**
	* Analyze if a char is printable with some content
	* (space is printable but has no content).
	* Returns true if the input char is printable.
	*/
bool isGraph(char thisChar)
{
	return (isgraph(thisChar) != 0);
}

/**
	* Analyze if a char is a hexadecimal digit (A-F, 0-9).
	* Returns true if the input char contains a hexadecimal digit.
	*/
bool isHexadecimalDigit(char thisChar)
{
	return (isxdigit(thisChar) != 0);
}

/**
	* Analyze if a char is lower case (that is a letter in lower case).
	* Returns true if the input char contains a letter in lower case.
	*/
bool isLowerCase(char thisChar)
{
	return (islower(thisChar) != 0);
}

/**
	* Analyze if a char is printable
	* (that is any character that produces an output, even a blank space).
	* Returns true if the input char is printable.
	*/
bool isPrintable(char thisChar)
{
	return (isprint(thisChar) != 0);
}

/**
	* Analyze if a char is punctuation (that is a comma,
	* a semicolon, an exclamation mark and so on).
	* Returns true if the input char is punctuation.
	*/
bool isPunct(char thisChar)
{
	return (ispunct(thisChar) != 0);
}

/**
	* Analyze if a char is a white-space character.
	* Returns true if the input char is a space,
	* form feed ('\f'), newline ('\n'), carriage return ('\r'),
	* horizontal tab ('\t'), or vertical tab ('\v').
	*/
bool isSpace(char thisChar)
{
	return (isspace(thisChar) != 0);
}

/**
	* Analyze if a char is upper case (that is, a letter in upper case).
	* Returns true if the input char is upper case.
	*/
bool isUpperCase(char thisChar)
{
	return (isupper(thisChar) != 0);
}

/**
	* Analyze if a char is a space character.
	* Returns true if the input char is a space or horizontal tab ('\t').
	*/
bool isWhitespace(char thisChar)
{
	return (isblank(thisChar) != 0);
}
