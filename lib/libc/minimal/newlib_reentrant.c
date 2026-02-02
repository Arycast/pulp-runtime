/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/**
	* implement global reentrant symbol
	* for single thread
	*/

#include <sys/reent.h>

struct _reent   global_impure_ptr[1] = {0};
struct _reent  *      _impure_ptr = &(global_impure_ptr[0]);
