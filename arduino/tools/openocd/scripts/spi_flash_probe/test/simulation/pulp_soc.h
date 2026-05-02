/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __PULP_SOC_H__
#define __PULP_SOC_H__

#ifdef __cplusplus
extern "C"
{
#endif

static inline void hal_apb_soc_pad_set_function(unsigned int pad, unsigned int func)
{
	/* do nothing */
	(void) pad;
	(void) func;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(__PULP_SOC_H__) */
