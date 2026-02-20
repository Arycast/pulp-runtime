/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/**
	* mocking for simulation/testing
	*/

#ifndef __SIMULATION_TEST_H__

#ifndef SIMULATION_TEST
#error this headers hould be used only for simulation/test
#endif

#include <stdint.h>

typedef uint8_t byte;

/* mocking strncasecmp_* as dummy non functional */
#define strncasecmp_PF                 strncasecmp_P
#define strncasecmp_P(s1, s2, size)    (0)

/*#define BIN 2*/
/*#define OCT 8*/
#define DEC 10
/*#define HEX 16*/

#endif /* ! defined(__SIMULATION_TEST_H__) */
