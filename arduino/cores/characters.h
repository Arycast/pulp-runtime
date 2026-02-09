/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/**
	* this file can be included in both C and C++ code
	*
	* this file implement language reference requirement
	* for functions: characters of arduino
	* see https://docs.arduino.cc/language-reference/#variables
	*/

/**
	* apparently we cannot use C11 feature directly inside C++11
	* so, instead to use C11's ctype.h here, put c function
	* in separated C source file
	* this has cost of jumping code instead of run code inplace
	*/

#ifndef __ARDUINO_CORES_CHARACTERS_H__
#define __ARDUINO_CORES_CHARACTERS_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* function details are available in C source code */

bool isAlpha(char thisChar);
bool isAlphaNumeric(char thisChar);
bool isAscii(char thisChar);
bool isControl(char thisChar);
bool isDigit(char thisChar);
bool isGraph(char thisChar);
bool isHexadecimalDigit(char thisChar);
bool isLowerCase(char thisChar);
bool isPrintable(char thisChar);
bool isPunct(char thisChar);
bool isSpace(char thisChar);
bool isUpperCase(char thisChar);
bool isWhitespace(char thisChar);

#ifdef __cplusplus
}
#endif

#endif /* ! defined(__ARDUINO_CORES_CHARACTERS_H__) */
