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
	* remove last byte from operand buffer if we detect
	* buffer has same length as string (+ '\0' termination)
	* can save memory, but add more operation
	*/
#ifndef SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION
#define SSTRING_CONF_OPERATOR_PLUS_REMOVE_ZERO_TERMINATION      1
#endif


#endif /* ! defined(__ARDUINO_CORES_CONFIG_SSTRING_H__) */
