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
	* for variables of arduino
	* see https://docs.arduino.cc/language-reference/#variables
	*/

#ifndef __ARDUINO_CORES_VARIABLES_H__
#define __ARDUINO_CORES_VARIABLES_H__

/* include stdbool.h for definition of true and false */
#include <stdbool.h>

/* include stddef.h for definition of size_t */
#include <stddef.h>

/* include stdint.h for definition of various fixed size integer */
#include <stdint.h>

/* platform custom definition, this is depend on setting in board variants */
#include "board/platform_board.h"

/* for PROGMEM and various pgm definition */
#include "pgmspace.h"

/**
	* **************************************************
	* Constants
	* **************************************************
	*/

/**
	* Floating Point Constants
	*
	* these constant are mandatory by POSIX but when XSI is used
	* C11 and C++11 do not include these standard
	*/

/* by default, do not define various math constants */
#define __ARDUINO_CORES_VARIABLES_DO_NOT_DEFINE_MATH_CONSTANTS    1

/* only redefine these constant when compiler do not use XSI */
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L))

	/**
		* we compile this code with POSIX standard
		* check if XSI feature test macro is available
		*/
	#ifdef _XOPEN_SOURCE

		/**
			* XSI test macro is available,
			* assume all math constants are defined by math.h
			*/
		#include <math.h>

	/* defined(_XOPEN_SOURCE) */
	#else /* ! defined(_XOPEN_SOURCE) */

		/**
			* compilation is done in POSIX standard, but without XSI test macro
			* assume math constants are not defined
			*/

		/* remove macro for disable math constants definition */
		#undef __ARDUINO_CORES_VARIABLES_DO_NOT_DEFINE_MATH_CONSTANTS

	#endif /* ! defined(_XOPEN_SOURCE) */

/* defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L) */
#else /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */

	/**
		* we are not using POSIX standard
		* remove macro for disable math constants definition
		*/
	#undef __ARDUINO_CORES_VARIABLES_DO_NOT_DEFINE_MATH_CONSTANTS


#endif /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */



/* define various maths constant if this macro is undefined  */
#ifndef __ARDUINO_CORES_VARIABLES_DO_NOT_DEFINE_MATH_CONSTANTS


/* natural number constant e */
#ifdef M_E
	/* warn if we redefine this constant */
	#warning     arduino/cores/variables.h redefine M_E
#endif /* defined(M_E) */
#define      M_E          2.7182818284590452354

/* log2 of e   {log2(e)} */
#ifdef M_LOG2E
	#warning     arduino/cores/variables.h redefine M_LOG2E
#endif /* defined(M_LOG2E) */
#define      M_LOG2E      1.4426950408889634074

/* log10 of e   {log10(e)} */
#ifdef M_LOG10E
	#warning     arduino/cores/variables.h redefine M_LOG10E
#endif /* defined(M_LOG10E) */
#define      M_LOG10E     0.43429448190325182765

/* natural logarithmic of 2 {ln(2)} */
#ifdef M_LN2
	#warning     arduino/cores/variables.h redefine M_LN2
#endif /* defined(M_LN2) */
#define      M_LN2        0.693147180559945309417

/* natural logarithmic of 10 {ln(10)} */
#ifdef M_LN10
	#warning     arduino/cores/variables.h redefine M_LN10
#endif /* defined(M_LN10) */
#define      M_LN10       2.30258509299404568402

/* trigonometric constant pi */
#ifdef M_PI
	#warning     arduino/cores/variables.h redefine M_PI
#endif /* defined(M_PI) */
#define      M_PI         3.14159265358979323846

/* pi / 2 */
#ifdef M_PI_2
	#warning     arduino/cores/variables.h redefine M_PI_2
#endif /* defined(M_PI_2) */
#define      M_PI_2       1.57079632679489661923

/* pi / 4 */
#ifdef M_PI_4
	#warning     arduino/cores/variables.h redefine M_PI_4
#endif /* defined(M_PI_4) */
#define      M_PI_4       0.78539816339744830962

/* 1 / pi */
#ifdef M_1_PI
	#warning     arduino/cores/variables.h redefine M_1_PI
#endif /* defined(M_1_PI) */
#define      M_1_PI       0.31830988618379067154

/* 2 / pi */
#ifdef M_2_PI
	#warning     arduino/cores/variables.h redefine M_2_PI
#endif /* defined(M_2_PI) */
#define      M_2_PI       0.63661977236758134308

/* 2 / sqrt(pi) */
#ifdef M_2_SQRTPI
	#warning     arduino/cores/variables.h redefine M_2_SQRTPI
#endif /* defined(M_2_SQRTPI) */
#define      M_2_SQRTPI   1.12837916709551257390

/* fundamental irrational number sqrt(2) */
#ifdef M_SQRT2
	#warning     arduino/cores/variables.h redefine M_SQRT2
#endif /* defined(M_SQRT2) */
#define      M_SQRT2      1.41421356237309504880

/* sqrt(1 / 2) */
#ifdef M_SQRT1_2
	#warning     arduino/cores/variables.h redefine M_SQRT1_2
#endif /* defined(M_SQRT1_2) */
#define      M_SQRT1_2    0.70710678118654752440


/**
	* these constants probably defined by
	* other standard (BSD) and when using
	* XSI, we won't have these constants
	*/

/* pi * 2 */
#ifdef M_TWOPI
	#warning     arduino/cores/variables.h redefine M_TWOPI
#endif /* defined(M_TWOPI) */
#define      M_TWOPI      6.283185307179586476925286766559

/* pi * 3 / 4 */
#ifdef M_3PI_4
	#warning     arduino/cores/variables.h redefine M_3PI_4
#endif /* defined(M_3PI_4) */
#define      M_3PI_4      2.3561944901923448370e0

/* sqrt(pi) */
#ifdef M_SQRTPI
	#warning     arduino/cores/variables.h redefine M_SQRTPI
#endif /* defined(M_SQRTPI) */
#define      M_SQRTPI     1.77245385090551602792981

/**
	* lower part of ln(2)
	* ln(2) = ln(2)_hi + ln(2)_lo
	*/
#ifdef M_LN2LO
	#warning     arduino/cores/variables.h redefine M_LN2LO
#endif /* defined(M_LN2LO) */
#define      M_LN2LO      1.9082149292705877000e-10

/* higher part of ln(2) */
#ifdef M_LN2HI
	#warning     arduino/cores/variables.h redefine M_LN2HI
#endif /* defined(M_LN2HI) */
#define      M_LN2HI      6.9314718036912381649e-1

/* sqrt(3) */
#ifdef M_SQRT3
	#warning     arduino/cores/variables.h redefine M_SQRT3
#endif /* defined(M_SQRT3) */
#define      M_SQRT3      1.73205080756887719000

/* 1 / ln(10) */
#ifdef M_IVLN10
	#warning     arduino/cores/variables.h redefine M_IVLN10
#endif /* defined(M_IVLN10) */
#define      M_IVLN10     0.43429448190325182765

/* log2(e) */
#ifdef M_LOG2_E
	#warning     arduino/cores/variables.h redefine M_LOG2_E
#endif /* defined(M_LOG2_E) */
#define      M_LOG2_E     M_LOG2E

/* 1 / ln(2) */
#ifdef M_INVLN2
	#warning     arduino/cores/variables.h redefine M_INVLN2
#endif /* defined(M_INVLN2) */
#define      M_INVLN2     1.4426950408889633870e0


#endif /* ! defined(__ARDUINO_CORES_VARIABLES_DO_NOT_DEFINE_MATH_CONSTANTS) */


/**
	* definition of HIGH and LOW
	*
	* arduino language reference manual require us to define these keywords
	* with HIGH = true = 1 and LOW = false = 0
	*/

/* do not use suffix u so this constant acceptable
* for int data type */
/*#define  HIGH  1
#define  LOW   0*/

/**
	* make high and low definition align with bool definition,
	* this require inclusion of stdbool.h
	* c standard explictly define true = 1 and false = 0
	*/
#define  HIGH         ((int) (true))
#define  LOW          ((int) (false))

/**
	* definition of PinMode: INPUT, INPUT_PULLUP, OUTPUT, etc.
	*
	* arduino language reference manual only require us to define these keywords
	* without clear definition of data type and value
	* just use int then
	*/
#define  INPUT              0
#define  OUTPUT             1
#define  INPUT_PULLUP       2
#define  INPUT_PULLDOWN     3
#define  OUTPUT_OPENDRAIN   4

#define  NOT_A_PIN         99

/**
	* interrupt mode definition
	*/
/*#define   LOW*/       /* to trigger the interrupt whenever the pin is low. */
#define   CHANGE  11  /* to trigger the interrupt whenever
					the pin changes value. */
#define   RISING  12  /* to trigger when the pin goes from low to high. */
#define   FALLING 13  /* for when the pin goes from high to low. */
/*#define   HIGH*/      /* to trigger the interrupt whenever the pin is high. */


/**
	* definition of LED_BUILTIN
	* use the one provided by platform board
	*/
#ifndef  LED_BUILTIN
#define  LED_BUILTIN   BOARD_LED_BUILTIN
#endif /* ! defined(LED_BUILTIN) */

/**
	* definition of true and false
	*/
/* already given by stdbool.h */

/**
	* bit order
	*/
#define  LSBFIRST         0
#define  MSBFIRST         1

/**
	* keyword for numbering base for String()
	* implement as global keyword, instead of private defintion of String class
	* so we don't have to use class as prefix or namespace
	* e.g. String::HEX
	*/
#define  BIN            2
#define  OCT            8
#define  DEC           10
#define  HEX           16

/**
	* **************************************************
	* Data Types
	* **************************************************
	*/
/* array   : already part of C and C++ syntax */

/* bool    : already defined by inclusion of stdbool.h */

/* boolean : another non-standard name for bool */
typedef  bool           boolean;

/* byte    : 8 bit unsigned, another name for uint8_t */
typedef  uint8_t        byte;

/* char    : native type of standard C and C++ */

/* double  : native type of standard C and C++ */

/* float   : native type of standard C and C++ */

/* int     : native type of standard C and C++ */

/* long    : native type of standard C and C++ */

/* short   : native type of standard C and C++ */

/* size_t  : defined by inclusion of stddef.h */

/* string  : already part of C and C++ syntax */

/**
	* class String()
	* this type cannot be used in C, thus not available in C code
	*/
/* included in Arduino.h */

/* unsigned char    : native type of standard C and C++ */

/* unsigned int     : native type of standard C and C++ */

/* unsigned long    : native type of standard C and C++ */

/* void    : native type of standard C and C++ */

/* word    : can store at least 16-bit unsigned value, just use int */
typedef  unsigned int   word;

/**
	* **************************************************
	* Variable Scope and Qualifiers
	* **************************************************
	*/
/* const    : native type of standard C and C++ */

/* static   : native type of standard C and C++ */

/* volatile : native type of standard C and C++ */

/**
	* **************************************************
	* Conversion
	* **************************************************
	*/
/* byte()          : provided by C and C++ standard */

/* char()          : provided by C and C++ standard */

/* float()         : provided by C and C++ standard */

/* int()           : provided by C and C++ standard */

/* long()          : provided by C and C++ standard */

/* (unsigned int)  : provided by C and C++ standard */

/* (unsigned long) : provided by C and C++ standard */

/**
	* word()
	* for C++, allow polymorphism:
	*     word(value)
	*     word(value_high, value_low)
	*/
/* NOT YET IMPLEMENTED!!! */
/*#ifdef __cplusplus
#endif*/ /* defined(__cplusplus) */

/**
	* **************************************************
	* Utilities
	* **************************************************
	*/
/* PROGMEM         : provided by pgmspace.h */

/* sizeof()        : provided by C and C++ standard  */

#endif /* ! defined(__ARDUINO_CORES_VARIABLES_H__) */
