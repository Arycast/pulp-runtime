/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __ASSERT_HELPER_H__
#define __ASSERT_HELPER_H__

#if (! ((defined(NDEBUG)) || (defined(__riscv__))))
#include <assert.h>
#endif



#if ((defined(NDEBUG)) || (defined(__riscv__)))
#define      _ASSERT(a)
#else
#define      _ASSERT(a)     assert(a)
#endif

#endif /* ! defined(__ASSERT_HELPER_H__) */
