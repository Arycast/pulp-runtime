/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h> /* stddef is included by string.h */

#include <stdlib.h>
#include <stdio.h>

#include "pulp.h"

#include "debugger_writer.h"


/**
	* start address to write spi flash
	*/
#ifndef START_WRITE_ADDRESS
#define START_WRITE_ADDRESS    0x00000000
#endif

/**
	* line ending, for simulation should set to "\n"
	* for platform deployment, should set to "\r\n"
	*/
#ifndef LINE_END
#define LINE_END   "\r\n"
#endif

/**
	* check if flash size is defined, use default value 64 Mib if not defined
	*/
#ifndef SPI_FLASH_SIZE_BYTES
	#if (defined(SIMULATION) && ((SIMULATION) != 0))
		#warning  SPI_FLASH_SIZE_BYTES is not defined in simulation, \
			include "test/simulation/spi_flash_simulation_posix.h" \
			to define SPI_FLASH_SIZE_BYTES; if another flash size is desired, \
			please define SPI_FLASH_SIZE_BITS instead of SPI_FLASH_SIZE_BYTES \
			SPI_FLASH_SIZE_BYTES is calculated based on SPI_FLASH_SIZE_BITS value
	#endif
	#warning  SPI_FLASH_SIZE_BYTES is not defined, set default to 64 Mbit
	#define SPI_FLASH_SIZE_BYTES    ((64 * 1024 * 1024) / 8)
#endif

/**
	* check if sector size is changed by user
	* you may use different sector size for simulation
	* but the simulation result may not reflect with
	* actual hardware behavior
	*/
#ifdef SPI_FLASH_SECTOR_SIZE_BYTES
	#warning  SPI_FLASH_SECTOR_SIZE_BYTES is defined \
		(default sector size is 4 kiB if \
		SPI_FLASH_SECTOR_SIZE_BYTES is undefined), \
		except you know what your doing, \
		please undefine SPI_FLASH_SECTOR_SIZE_BYTES
#else
	#define SPI_FLASH_SECTOR_SIZE_BYTES    (4 * 1024)
#endif

/**
	* check if page size is changed by user
	* you may use different page size for simulation
	* but the simulation result may not reflect with
	* actual hardware behavior
	*/
#ifdef SPI_FLASH_PAGE_SIZE_BYTES
	#warning  SPI_FLASH_PAGE_SIZE_BYTES is defined \
		(default page size is 256 bytes if \
		SPI_FLASH_PAGE_SIZE_BYTES is undefined), \
		except you know what your doing, \
		please undefine SPI_FLASH_PAGE_SIZE_BYTES
#else
	#define SPI_FLASH_PAGE_SIZE_BYTES    256
#endif


/* check and make sure page size is smaller than sector size */
#if ((SPI_FLASH_SECTOR_SIZE_BYTES) < (SPI_FLASH_PAGE_SIZE_BYTES))
	#error  SPI_FLASH_SECTOR_SIZE_BYTES value should be bigger than \
		SPI_FLASH_PAGE_SIZE_BYTES
#elif (((SPI_FLASH_SECTOR_SIZE_BYTES) % (SPI_FLASH_PAGE_SIZE_BYTES)) != 0)
	#error  SPI_FLASH_SECTOR_SIZE_BYTES should be divisible by \
		SPI_FLASH_PAGE_SIZE_BYTES \
		(SPI_FLASH_SECTOR_SIZE_BYTES = SPI_FLASH_PAGE_SIZE_BYTES * N ; \
		where N is integer)
#endif


/**
	* check whether we start to write flash from sector start or not
	* if not, give warning
	*/
#if (((START_WRITE_ADDRESS) % (SPI_FLASH_SECTOR_SIZE_BYTES)) != 0)
	#warning  START_WRITE_ADDRESS not align with start of sector, \
		sector write will be done twice per session; \
		set START_WRITE_ADDRESS to any of sector start address \
		(SPI_FLASH_SECTOR_SIZE_BYTES * n, with n = {0, 1, 2, 3, ...})
#endif

/**
	* check if DEBUGGER_DATA_BLOCK_SIZE is defined (from debugger_writer.h)
	* and have same value with SPI_FLASH_SECTOR_SIZE_BYTES
	*/
#ifndef DEBUGGER_DATA_BLOCK_SIZE
	#error  DEBUGGER_DATA_BLOCK_SIZE is not defined, \
		please include "debugger_writer.h"!
#else
	#if ((SPI_FLASH_SECTOR_SIZE_BYTES) != (DEBUGGER_DATA_BLOCK_SIZE))
		#error  SPI_FLASH_SECTOR_SIZE_BYTES value is not same with \
			DEBUGGER_DATA_BLOCK_SIZE, debugger need to assume that buffer size \
			is exacly 1 spi flash sector size
	#endif
#endif


/* default don't write disable spi flash */
#ifndef SPI_FLASH_CALL_WRITE_DISABLE
#define SPI_FLASH_CALL_WRITE_DISABLE 0
#endif

/**
	* **************************************************
	* custom type declaration
	*/

/**
	* **************************************************
	* custom type definition
	*/

/**
	* **************************************************
	* private function declaration
	*/

/**
	* write enable and disable
	*/
static void spi_nor_flash_write_enable(spim_t *spim);
#if ((SPI_FLASH_CALL_WRITE_DISABLE) > 0)
static void spi_nor_flash_write_disable(spim_t *spim);
#endif /* ((SPI_FLASH_CALL_WRITE_DISABLE) > 0) */

/* check if flash is busy */
static bool spi_nor_flash_is_busy(spim_t *spim);
static void spi_nor_flash_wait_flash_ready(spim_t *spim);

/**
	* sector access
	*/

/* erase sector */
static void    spi_nor_flash_erase_sector(spim_t *spim, uint32_t sector);

/* write full sector */
static void    spi_nor_flash_write_sector(spim_t * restrict spim, uint32_t sector, const void * restrict data);

/* read full sector */
static size_t  spi_nor_flash_read_sector(spim_t * restrict spim, uint32_t sector, void * restrict data);


/* write data to random address */
static void    spi_nor_flash_write_data(spim_t * restrict spim, uint32_t base_address, const void * restrict data, size_t size);


/**
	* required by crt0.S
	*/
void pe_start(void);



/**
	* **************************************************
	* private variable declaration & definition
	*/

uint8_t global_shared_buffer[SPI_FLASH_SECTOR_SIZE_BYTES];

/**
	* **************************************************
	* private & public function definition
	*/

int main(void)
{
	spim_t        *spim;
	spim_conf_t    spim_conf;

	uint8_t        buffer[SPI_FLASH_SECTOR_SIZE_BYTES];
	size_t         buffer_len;

	size_t         base_address = START_WRITE_ADDRESS;


	/*printf("Flashing External NOR Flash" LINE_END);*/
	puts("Flashing External NOR Flash");


	/**
		* **************************************************
		* initialize spi configuration
		*/

	/*printf("Initializing spim object" LINE_END);*/
	puts("Initializing spim object");

	/* initialize config */
	spim_conf_init(&spim_conf);

	/* set necessary configuration */
	spim_conf.max_baudrate = 1000000;
	spim_conf.id = 0;
	spim_conf.cs = 0;


	/**
		* **************************************************
		* initialize spi
		*/

	/*printf("Opening spim connection" LINE_END);*/
	puts("Opening spim connection");

	/* open spim device based on configuration */
	spim = spim_open(&spim_conf);
	if (spim == NULL)
	{
		return -1;
	}

	do
	{
		/* get data from debugger */
		data_from_debugger(buffer, &buffer_len);

		/* write data if debugger set length to non-zero */
		if (buffer_len > 0)
		{
			printf("Writing new data to external flash " \
				"at address: 0x%08lx, with length: %lu" LINE_END,
				(unsigned long) base_address, (unsigned long) buffer_len);

			/**
				* write data to spi flash
				*/
			spi_nor_flash_write_data(spim, (uint32_t) base_address, buffer, buffer_len);

			/* move address */
			base_address += buffer_len;
		}


		/**
			* keep looping until debugger fill data less than sector size
			*/
	}
	while (buffer_len >= DEBUGGER_DATA_BLOCK_SIZE);

	/* call write disable if needed */
#if ((SPI_FLASH_CALL_WRITE_DISABLE) > 0)
	spi_nor_flash_write_disable(spim);
#endif /* ((SPI_FLASH_CALL_WRITE_DISABLE) > 0) */

	/**
		* **************************************************
		* close spi
		*/

	spim_close(spim);
	spim = NULL; /* prevent usage of spim */

	puts("SPI closed");

	puts("Done");

	return 0;
}

/**
	* needed by crt0.S
	*/
void pe_start(void)
{
}

static void spi_nor_flash_write_enable(spim_t *spim)
{
	uint8_t cmd[1];
	uint8_t spi_rx[1];

	cmd[0] = 0x06; /* Write Enable */

	spim_transfer(spim, cmd, spi_rx, 1 * 8, SPIM_CS_AUTO);
}

#if ((SPI_FLASH_CALL_WRITE_DISABLE) > 0)
static void spi_nor_flash_write_disable(spim_t *spim)
{
	uint8_t cmd[1];
	uint8_t spi_rx[1];

	cmd[0] = 0x04; /* Write Disable */

	spim_transfer(spim, cmd, spi_rx, 1 * 8, SPIM_CS_AUTO);
}
#endif /* ((SPI_FLASH_CALL_WRITE_DISABLE) > 0) */

static bool spi_nor_flash_is_busy(spim_t *spim)
{
	uint8_t cmd[2];
	uint8_t spi_rx[2];

	cmd[0] = 0x05; /* Read Status Register-1 */
	cmd[1] = 0x00;

	spi_rx[0] = 0x00;
	spi_rx[1] = 0x00;

	spim_transfer(spim, cmd, spi_rx, 2 * 8, SPIM_CS_AUTO);

	return (spi_rx[1] & 0x01) == 0x01;
}

static void spi_nor_flash_wait_flash_ready(spim_t *spim)
{
	while (spi_nor_flash_is_busy(spim))
	{
	}
}


static void spi_nor_flash_erase_sector(spim_t *spim, uint32_t sector)
{
	const uint32_t NUMBER_OF_SECTOR =
		SPI_FLASH_SIZE_BYTES / SPI_FLASH_SECTOR_SIZE_BYTES;

	const uint32_t base_address =
		sector * SPI_FLASH_SECTOR_SIZE_BYTES;

	uint8_t cmd[4];
	uint8_t spi_rx[4];

	/* make sure we erase correct sector */
	if (sector >= NUMBER_OF_SECTOR)
	{
		/* nothing to erase */
		return;
	}

	/**
		* enable writing
		*/
	spi_nor_flash_write_enable(spim);

	/**
		* make erase command
		* address used in erase sector is base address and not sector number
		*/
	cmd[0] = 0x20;
	cmd[1] = (base_address >> 16) & 0xff; /* msB fragment send first */
	cmd[2] = (base_address >>  8) & 0xff;
	cmd[3] =  base_address        & 0xff; /* lsB fragment send last */

	/* send erase command */
	spim_transfer(spim, cmd, spi_rx, 4 * 8, SPIM_CS_AUTO);

	/* wait until flash ready */
	spi_nor_flash_wait_flash_ready(spim);
}

/**
	* write full sector info nor flash in page bases
	* this function expect data buffer size is same or larger than sector size
	*/
static void spi_nor_flash_write_sector(spim_t * restrict spim, uint32_t sector, const void * restrict data)
{
	const uint32_t NUMBER_OF_SECTOR =
		SPI_FLASH_SIZE_BYTES / SPI_FLASH_SECTOR_SIZE_BYTES;

	const size_t   NUMBER_OF_PAGE_PER_SECTOR =
		SPI_FLASH_SECTOR_SIZE_BYTES / SPI_FLASH_PAGE_SIZE_BYTES;

	uint32_t base_address =
		sector * SPI_FLASH_SECTOR_SIZE_BYTES;

	/* allocate in page basis */
	uint8_t cmd[4];
	uint8_t spi_rx[SPI_FLASH_PAGE_SIZE_BYTES];

	const uint8_t * restrict _data = data;

	size_t i, data_offset;

	/* make sure we write correct sector */
	if (sector >= NUMBER_OF_SECTOR)
	{
		/* nothing to write */
		return;
	}

	/**
		* enable writing
		*/
	spi_nor_flash_write_enable(spim);

	/* initialize command to page program */
	cmd[0] = 0x02; /* page program */

	/* loop write page */
	for (i = 0, data_offset = 0;
		i < NUMBER_OF_PAGE_PER_SECTOR;
		++i, data_offset += SPI_FLASH_PAGE_SIZE_BYTES, base_address += SPI_FLASH_PAGE_SIZE_BYTES)
	{
		cmd[1] = (base_address >> 16) & 0xff;
		cmd[2] = (base_address >>  8) & 0xff;
		cmd[3] =  base_address        & 0xff;

		/* send command, keep cs */
		spim_transfer(spim, cmd, spi_rx, 4 * 8, SPIM_CS_KEEP);

		/* send data */
		spim_transfer(spim, &(_data[data_offset]), spi_rx, SPI_FLASH_PAGE_SIZE_BYTES * 8, SPIM_CS_AUTO);

		/* wait idle */
		spi_nor_flash_wait_flash_ready(spim);
	}
}

/**
	* read full sector
	* this function expect data buffer is as big as 1 sector
	* return number of data written
	*/
static size_t spi_nor_flash_read_sector(spim_t * restrict spim, uint32_t sector, void * restrict data)
{
	const uint32_t NUMBER_OF_SECTOR =
		SPI_FLASH_SIZE_BYTES / SPI_FLASH_SECTOR_SIZE_BYTES;

	const size_t   NUMBER_OF_PAGE_PER_SECTOR =
		SPI_FLASH_SECTOR_SIZE_BYTES / SPI_FLASH_PAGE_SIZE_BYTES;

	const uint32_t base_address =
		sector * SPI_FLASH_SECTOR_SIZE_BYTES;

	/* allocate in page basis */
	uint8_t cmd[SPI_FLASH_PAGE_SIZE_BYTES];
	uint8_t spi_rx[4];

	uint8_t * restrict _data = data;

	size_t i, data_offset;

	size_t number_of_data_received = 0;

	/* make sure we write correct sector */
	if (sector >= NUMBER_OF_SECTOR)
	{
		/* nothing to write */
		return 0;
	}

	/* empty out cmd */
	memset(cmd, 0, SPI_FLASH_PAGE_SIZE_BYTES * sizeof(uint8_t));

	/* initialize command to page program */
	cmd[0] = 0x03; /* page program */
	cmd[1] = (base_address >> 16) & 0xff;
	cmd[2] = (base_address >>  8) & 0xff;
	cmd[3] =  base_address        & 0xff;

	/* send command read, keep cs low */
	spim_transfer(spim, cmd, spi_rx, 4 * 8, SPIM_CS_KEEP);

	/* make cmd empty again for transfer data so tx will send 0x00 */
	cmd[0] = 0x00;
	cmd[1] = 0x00;
	cmd[2] = 0x00;
	cmd[3] = 0x00;

	/* loop write page */
	for (i = 1, data_offset = 0; /* start i from 1 to skip last data */
		i < NUMBER_OF_PAGE_PER_SECTOR;
		++i, data_offset += SPI_FLASH_PAGE_SIZE_BYTES)
	{
		/* read data */
		spim_transfer(spim, cmd, &(_data[data_offset]), SPI_FLASH_PAGE_SIZE_BYTES * 8, SPIM_CS_KEEP);
		number_of_data_received += SPI_FLASH_PAGE_SIZE_BYTES;
	}

	spim_transfer(spim, cmd, &(_data[data_offset]), SPI_FLASH_PAGE_SIZE_BYTES * 8, SPIM_CS_AUTO);
	number_of_data_received += SPI_FLASH_PAGE_SIZE_BYTES;

	return number_of_data_received;
}

static void    spi_nor_flash_write_data(spim_t * restrict spim, uint32_t base_address, const void * restrict data, size_t size)
{
	/* we should not read data again, use _data! */
	const uint8_t * restrict _data = data;

	/* make sure data is not NULL */
	if (_data == NULL)
	{
		return;
	}

	/* write per sector */
	while ((size > 0) && (base_address < SPI_FLASH_SIZE_BYTES))
	{
		bool read_sector = false;

		size_t write_size;

		/* get base address */
		uint32_t sector = base_address / SPI_FLASH_SECTOR_SIZE_BYTES;
		uint32_t next_sector_base_address;

		/**
			* check if we need to read sector
			* condition to read sector are (only one being fulfilled is enough):
			* 1. base_address don't align with current sector
			* 2. base_address + size don't align with current sector end
			*    and less than current sector
			*/
		uint32_t sector_offset_bytes = base_address % SPI_FLASH_SECTOR_SIZE_BYTES;
		if (sector_offset_bytes > 0)
		{
			read_sector = true;
		}

		/* calculate how much data we will write to temporary buffer */
		next_sector_base_address = (sector + 1) * SPI_FLASH_SECTOR_SIZE_BYTES;

		/* check if size is outside current sector boundary */
		if ((base_address + size) >= next_sector_base_address)
		{
			/**
				* data that need to be written to flash is
				* will overflow to next sector or end exactly at
				* end of sector
				*/
			write_size = next_sector_base_address - base_address;
		}
		else
		{
			/**
				* data that need to be written will less than sector size
				* try to keep data in flash at the end of sector
				*/
			write_size = size;

			/* keep data at the end of sector */
			read_sector = true;
		}

		/**
			* read sector if necessary
			*/
		if (read_sector)
		{
			/* store read data in global buffer to save stack */
			size_t retval = spi_nor_flash_read_sector(spim, sector, global_shared_buffer);

			/* we should read exactly 1 sector */
			if (retval != SPI_FLASH_SECTOR_SIZE_BYTES)
			{
				/**
					* fail to read or sector/address is outside the boundary
					* treat as fail with grace
					*/
				return;
			}
		}
		else
		{
			/**
				* if we don't need to read sector, make sure:
				* 1. sector_offset_bytes == 0
				* 2. write_size == SPI_FLASH_SECTOR_SIZE_BYTES
				*/
			if ((sector_offset_bytes != 0) || (write_size != SPI_FLASH_SECTOR_SIZE_BYTES))
			{
				/* abort because this is flaw in program logic */
				abort();
			}
		}

		/* write data to buffer */
		memcpy(&(global_shared_buffer[sector_offset_bytes]), _data, write_size * sizeof(uint8_t));

		/* erase flash */
		spi_nor_flash_erase_sector(spim, sector);

		/* write to flash */
		spi_nor_flash_write_sector(spim, sector, global_shared_buffer);

		/* move buffer */
		_data         += write_size;
		size          -= write_size;

		/* move to next sector */
		base_address  += (uint32_t) write_size;

		/* loop until size is 0 */
	}
}
