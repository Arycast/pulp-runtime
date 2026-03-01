/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __ARDUINO_CORES_CONFIG_SSTRING_H__
#define __ARDUINO_CORES_CONFIG_SSTRING_H__

/**
	* when set to 1, overloading operator will call abort when
	* dynamic memory allocation function (malloc, realloc, etc.) is fail
	*/
#ifndef SSTRING_CONF_ABORT_ON_ALLOC_FAIL
#define SSTRING_CONF_ABORT_ON_ALLOC_FAIL      1
#endif

/**
	* when set to 1, overloading operator will call abort when
	* snprintf function fail (for conversion from various integer and float
	* type to c string)
	*/
#ifndef SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL
#define SSTRING_CONF_ABORT_ON_SNPRINTF_FAIL      1
#endif


/**
	* remove last byte from operand buffer if we detect
	* buffer has same length as string (+ '\0' termination)
	* can save memory, but add more operation
	*/
#ifndef SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION
#define SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION      1
#endif


/**
	* set whether alphabetic digit for string representation of number
	* with base bigger than 0 should use upper case
	*/
#ifndef SSTRING_CONF_BASE_UPPERCASE
#define SSTRING_CONF_BASE_UPPERCASE      0
#endif


/**
	* should we use snprintf for integer and float value conversion
	*/
#ifndef SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION
#define SSTRING_CONF_USE_SNPRINTF_FOR_NON_BASE_CONVERSION        0
#endif

/**
	* default length of precision for conversion from double to string
	*/
#ifndef SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_DEFAULT_LEN
#define SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_DEFAULT_LEN        6
#endif

/**
	* maximum length of precision for conversion from double to string
	*/
#ifndef SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_MAX_LEN
#define SSTRING_CONF_DOUBLE_TO_STRING_PRECISION_MAX_LEN        16
#endif

#endif /* ! defined(__ARDUINO_CORES_CONFIG_SSTRING_H__) */
