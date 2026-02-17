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
	* but intended to only comply with C
	*
	* this file is replacement for avr's pgmspace.h for pulpissimo
	*/

#ifndef __ARDUINO_CORES_PGMSPACE_H__
#define __ARDUINO_CORES_PGMSPACE_H__

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX.1-2001 header onwards */
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L))
#include <strings.h>
#endif

/* use compatibility helper from pulp libc */
#include "compatibility.h"

/**
	* PROGMEM is intended to be used for read-only data (const data),
	* so put it in .rodata section instead of .text
	* NOTE: __attribute__ keyword is GNU extension,
	*       please make work-around for other compiler!
	*/
#define    PROGMEM       __attribute__ ((section(".rodata")))

/**
	* general definition for read pgm value from pgm address
	* put const so this macro cannot be abused as l-value in statement
	*/
#define  __pgm_read_value_from_pointer(type, address)     (*((const type *) (address)))

/* read pgm value based on type */
#define    pgm_read_char(address)                   __pgm_read_value_from_pointer(char, address)
#define    pgm_read_unsigned_char(address)          __pgm_read_value_from_pointer(unsigned char, address)
#define    pgm_read_signed_char                       pgm_read_char
#define    pgm_read_u8(address)                     __pgm_read_value_from_pointer(uint8_t, address)
#define    pgm_read_i8(address)                     __pgm_read_value_from_pointer(int8_t, address)
#define    pgm_read_short(address)                  __pgm_read_value_from_pointer(short, address)
#define    pgm_read_unsigned_short(address)         __pgm_read_value_from_pointer(unsigned short, address)
#define    pgm_read_u16(address)                    __pgm_read_value_from_pointer(uint16_t, address)
#define    pgm_read_i16(address)                    __pgm_read_value_from_pointer(int16_t, address)
#define    pgm_read_int(address)                    __pgm_read_value_from_pointer(int, address)
#define    pgm_read_signed                            pgm_read_int
#define    pgm_read_unsigned                          pgm_read_unsigned_int
#define    pgm_read_signed_int                        pgm_read_int
#define    pgm_read_unsigned_int(address)           __pgm_read_value_from_pointer(unsigned int, address)
/*#define    pgm_read_u24(address)                    __pgm_read_value_from_pointer(uint24_t, address)*/ /* uint24_t not a valid type */
/*#define    pgm_read_i24(address)                    __pgm_read_value_from_pointer(int24_t, address)*/ /*  int24_t not a valid type */
#define    pgm_read_u32(address)                    __pgm_read_value_from_pointer(uint32_t, address)
#define    pgm_read_i32(address)                    __pgm_read_value_from_pointer(int32_t, address)
#define    pgm_read_long(address)                   __pgm_read_value_from_pointer(long, address)
#define    pgm_read_unsigned_long(address)          __pgm_read_value_from_pointer(unsigned long, address)
/*#define    pgm_read_long_long(address)              __pgm_read_value_from_pointer(long long, address)*/ /* long long is extension */
/*#define    pgm_read_unsigned_long_long(address)     __pgm_read_value_from_pointer(unsigned long long, address)*/ /* long long is extension */
#define    pgm_read_u64(address)                    __pgm_read_value_from_pointer(uint64_t, address)
#define    pgm_read_i64(address)                    __pgm_read_value_from_pointer(int64_t, address)
#define    pgm_read_float(address)                  __pgm_read_value_from_pointer(float, address)
#define    pgm_read_double(address)                 __pgm_read_value_from_pointer(double, address)
/*#define    pgm_read_long_double(address)            __pgm_read_value_from_pointer(long double, address)*/ /* long double is extension */


/* read pgm value based on type on far address */
#define    pgm_read_char_far                          pgm_read_char
#define    pgm_read_unsigned_char_far                 pgm_read_unsigned_char
#define    pgm_read_signed_char_far                   pgm_read_signed_char
#define    pgm_read_u8_far                            pgm_read_u8
#define    pgm_read_i8_far                            pgm_read_i8
#define    pgm_read_short_far                         pgm_read_short
#define    pgm_read_unsigned_short_far                pgm_read_unsigned_short
#define    pgm_read_u16_far                           pgm_read_u16
#define    pgm_read_i16_far                           pgm_read_i16
#define    pgm_read_int_far                           pgm_read_int
#define    pgm_read_signed_far                        pgm_read_signed
#define    pgm_read_unsigned_far                      pgm_read_unsigned
#define    pgm_read_signed_int_far                    pgm_read_signed_int
#define    pgm_read_unsigned_int_far                  pgm_read_unsigned_int
/*#define    pgm_read_u24_far                           pgm_read_u24*/
/*#define    pgm_read_i24_far                           pgm_read_i24*/
#define    pgm_read_u32_far                           pgm_read_u32
#define    pgm_read_i32_far                           pgm_read_i32
#define    pgm_read_long_far                          pgm_read_long
#define    pgm_read_unsigned_long_far                 pgm_read_unsigned_long
/*#define    pgm_read_long_long_far                     pgm_read_long_long*/
/*#define    pgm_read_unsigned_long_long_far            pgm_read_unsigned_long_long*/
#define    pgm_read_u64_far                           pgm_read_u64
#define    pgm_read_i64_far                           pgm_read_i64
#define    pgm_read_float_far                         pgm_read_float
#define    pgm_read_double_far                        pgm_read_double
/*#define    pgm_read_long_double_far                   pgm_read_long_double*/

/* definition for pgm pointer */

/* string (zero-terminated array of char) */
#ifndef PGM_P
#define PGM_P        const char *
#endif

/* general pointer */
#ifndef PGM_VOID_P
#define PGM_VOID_P   const void *
#endif



/* declaration for string const */
#ifndef PSTR
#define PSTR(s)       (s)
#endif

#ifndef PSTR_FAR
#define PSTR_FAR(s)   (s)
#endif

/* constants string for storing in flash */
#ifndef F
#define F(s)          (s)
#endif

/* for various string type */
#ifndef __FlashStringHelper
#define __FlashStringHelper           char

/* also define this symbol to help prevent overloading */
#define __FLASHSTRINGHELPER_IS_CHAR   1
#endif


/* read based on integer size */
#define pgm_read_byte_near                     pgm_read_byte
#define pgm_read_word_near                     pgm_read_word
#define pgm_read_dword_near                    pgm_read_dword
#define pgm_read_qword_near                    pgm_read_qword
#define pgm_read_float_near                    pgm_read_float
#define pgm_read_ptr_near                      pgm_read_ptr

#define pgm_read_byte_far                      pgm_read_byte
#define pgm_read_word_far                      pgm_read_word
#define pgm_read_dword_far                     pgm_read_dword
#define pgm_read_float_far                     pgm_read_float
#define pgm_read_qword_far                     pgm_read_qword
#define pgm_read_ptr_far                       pgm_read_ptr

#define pgm_read_byte                          pgm_read_u8
#define pgm_read_word                          pgm_read_u32
#define pgm_read_dword                         pgm_read_u32
#define pgm_read_qword                         pgm_read_u64
/*#define pgm_read_float                         pgm_read_float*/
#define pgm_read_ptr(address)                __pgm_read_value_from_pointer(void *, address)

/**
	* get (far) address of a variable
	* for near address, just use usual '&' operator
	*/
#define pgm_get_far_address(variable)       ((const void *) (&(variable)))



/* prevent definition for extension and bsd */

/* pgm pointer */
#define strlen_P         strlen
#define memchr_P         memchr
#define memcmp_P         memcmp
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L))
	#define memccpy_P        memccpy       /* POSIX.1-2008 */
/* (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
#else /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
	#ifdef __cplusplus
	extern "C"
	{
	#endif
		void *  memccpy_P(void * restrict dest, const void * restrict src,
			int c, size_t n);
	#ifdef __cplusplus
	} /* extern "C" */
	#endif /* defined(__cplusplus) */
#endif /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
#define memcpy_P         memcpy
/*#define memmem_P         memmem*/        /* extension (available in BSD) */
/*#define memrchr_P        memrchr*/       /* extension */
#define strcat_P         strcat
#define strchr_P         strchr
/*#define strchrnul_P      strchrnul*/     /* extension */
#define strcmp_P         strcmp
#define strcpy_P         strcpy
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L))
	#define stpcpy_P         stpcpy        /* POSIX.1-2008 */
/* (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
#else /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
	#ifdef __cplusplus
	extern "C"
	{
	#endif /* defined(__cplusplus) */
		char *  stpcpy_P(char * restrict dst, const char * restrict src);
	#ifdef __cplusplus
	}
	#endif /* defined(__cplusplus) */
#endif /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L))
	#define strcasecmp_P     strcasecmp    /* POSIX.1-2001 */
/* (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */
#else /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */
	#ifdef __cplusplus
	extern "C"
	{
	#endif /* defined(__cplusplus) */
		int     strcasecmp_P(const char *s1, const char *s2);
	#ifdef __cplusplus
	}
	#endif /* defined(__cplusplus) */
#endif /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */
/*#define strcasestr_P     strcasestr*/    /* extension */
#define strcspn_P        strcspn
/*#define strlcat_P        strlcat*/       /* BSD */
/*#define strlcpy_P        strlcpy*/       /* BSD */
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L))
	#define strnlen_P        strnlen       /* POSIX.1-2008 */
/* (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
#else /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
	#ifdef __cplusplus
	extern "C"
	{
	#endif /* defined(__cplusplus) */
		size_t  strnlen_P(const char *s, size_t maxlen);
	#ifdef __cplusplus
	}
	#endif /* defined(__cplusplus) */
#endif /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)) */
#define strncmp_P        strncmp
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L))
	#define strncasecmp_P    strncasecmp   /* POSIX.1-2001 */
/* (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */
#else /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */
#ifdef __cplusplus
	extern "C"
	{
	#endif /* defined(__cplusplus) */
		int     strncasecmp_P(const char *s1, const char *s2, size_t n);
	#ifdef __cplusplus
	}
	#endif /* defined(__cplusplus) */
#endif /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */
#define strncat_P        strncat
#define strncpy_P        strncpy
#define strpbrk_P        strpbrk
#define strrchr_P        strrchr
/*#define strsep_P         strsep*/        /* BSD */
#define strspn_P         strspn
#define strstr_P         strstr
#define strtok_P         strtok
#if (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L))
	#define strtok_rP        strtok_r      /* POSIX.1-2001 */
/* (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */
#else /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */
	#ifdef __cplusplus
	extern "C"
	{
	#endif /* defined(__cplusplus) */
		char *  strtok_rP(char * restrict str, const char * restrict delim,
			char ** restrict saveptr);
	#ifdef __cplusplus
	}
	#endif /* defined(__cplusplus) */
#endif /* ! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)) */

/* pgm pointer for far pointer */
#define strlen_PF        strlen_P
#define strnlen_PF       strnlen_P     /* POSIX.1-2008 */
#define memcpy_PF        memcpy_P
#define strcpy_PF        strcpy_P
#define stpcpy_PF        stpcpy_P      /* POSIX.1-2008 */
#define strncpy_PF       strncpy_P
#define strcat_PF        strcat_P
/*#define strlcat_PF       strlcat_P*/     /* BSD */
#define strncat_PF       strncat_P
#define strcmp_PF        strcmp_P
#define strncmp_PF       strncmp_P
#define strcasecmp_PF    strcasecmp_P  /* POSIX.1-2001 */
#define strncasecmp_PF   strncasecmp_P /* POSIX.1-2001 */
#define strchr_PF        strchr_P
#define strstr_PF        strstr_P
/*#define strlcpy_PF       strlcpy_P*/     /* BSD */
#define memcmp_PF        memcmp_P

#endif /* __ARDUINO_CORES_PGMSPACE_H__ */
