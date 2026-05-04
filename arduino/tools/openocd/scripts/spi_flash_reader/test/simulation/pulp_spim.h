/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __PULP_SPIM_H__
#define __PULP_SPIM_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
	* custom types declarations
	*/

struct spim_conf_s;

struct spim_s;

/**
	* custom types definition
	*/

enum spim_cs_e
{
	SPIM_CS_AUTO,
	SPIM_CS_KEEP,
	SPIM_CS_NONE,
};

struct spim_conf_s
{
	int8_t   id;
	int8_t   cs;
	int32_t  max_baudrate;
};

struct spim_s
{
	uint8_t *buffer_tx;
	uint8_t *buffer_rx;
	size_t   buffer_len;

	size_t   filled_len;
};

/* custom type redefinition */
typedef   struct spim_conf_s   spim_conf_t;
typedef   struct spim_s        spim_t;

/**
	* public/exported global variables
	*/



/**
	* public/exported global functions
	*/

/* initialize config to default */
void spim_conf_init(spim_conf_t *spim_conf);

/* open spi device */
spim_t *spim_open(const spim_conf_t *spim_conf);

/* close spi device */
void spim_close(spim_t *spim);

/* transfer full duplex TRx spi */
void spim_transfer(spim_t * restrict spim,
	const void * restrict tx_data,
	void * restrict rx_data, size_t bit_len, enum spim_cs_e mode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(__PULP_SPIM_H__) */
