/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __SPI_FLASH_SIMULATION_POSIX_H__
#define __SPI_FLASH_SIMULATION_POSIX_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
	* spi flash size in bits
	*/
#ifndef SPI_FLASH_SIZE_BITS
#define SPI_FLASH_SIZE_BITS (64 * 1024 * 1024) /* W25Q64JV: 64 Mib */
/*#define SPI_FLASH_SIZE_BITS (128 * 1024 * 1024)*/ /* W25Q128FV: 128 Mib */
#endif

#ifndef SPI_FLASH_SIZE_BYTES
#define SPI_FLASH_SIZE_BYTES ((SPI_FLASH_SIZE_BITS) / 8)
#endif

/**
	* file to store flash data, prepend with '/' if
	* absolute path is given, do not prepend with '/'
	* to put file relative to this tester executable
	*/
#ifndef FLASH_FILE
#define FLASH_FILE    "flash_content.bin"
#endif

/* default 4 kiB */
#ifndef SPI_FLASH_SECTOR_SIZE_BYTES
#define SPI_FLASH_SECTOR_SIZE_BYTES    (4 * 1024)
#endif

/**
	* initialize flash file
	* 1. create and allocate file if file don't exist
	* 2. truncate file if file size is smaller than SPI_FLASH_SIZE_BYTES
	*/
int  spi_flash_simulation_init(void);
void spi_flash_simulation_fini(void);

/* trx spi flash is handled here */
int spi_flash_trx_session(const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_stop);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(__SPI_FLASH_SIMULATION_POSIX_H__) */
