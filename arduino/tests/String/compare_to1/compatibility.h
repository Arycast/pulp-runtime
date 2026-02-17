/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __COMPATIBILITY_H__
#define __COMPATIBILITY_H__

/* C++ standard don't support restrict keyword */
#ifdef __cplusplus

/* define keyword restrict for C++11 or more */
#define  restrict  __restrict

#endif /* defined(__cplusplus) */

#endif /* ! defined(__COMPATIBILITY_H__) */
