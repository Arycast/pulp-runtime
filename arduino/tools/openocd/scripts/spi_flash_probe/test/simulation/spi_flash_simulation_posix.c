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
#include <string.h> /* stddef.h already included here */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <errno.h>
#include <assert.h>


#include "spi_flash_simulation_posix.h"


/**
	* this code requires at least POSIX.1-2001 / SUSv3 to be compiled
	* or cross compiled
	* (although POSIX.1-2008 / SUSv4 is prefered)
	*/
#if ((! defined(_POSIX_C_SOURCE)) || ((_POSIX_C_SOURCE) < 200112L))
	#error please compile this code with (at least) POSIX.1-2001 standard, \
		for gcc, add -D_POSIX_C_SOURCE=200112L
#endif

/**
	* **************************************************
	* private custom type declaration
	*/
struct spi_flash_command_callback_s;

/**
	* **************************************************
	* private custom type definition
	*/
struct spi_flash_command_callback_s
{
	int (*callback)(uint8_t code,
		const uint8_t * restrict tx,
		uint8_t * restrict rx,
		size_t len,
		bool is_start,
		bool is_stop);

	const char *function_name;
};

typedef struct spi_flash_command_callback_s spi_flash_command_callback_t;

/**
	* **************************************************
	* private function declaration
	*/
/* callback argument */
#define _CALLBACK_FUNCTION_ARG_  \
	uint8_t code, \
	const uint8_t * restrict tx, \
	uint8_t * restrict rx, \
	size_t len, \
	bool is_start, \
	bool is_stop

/* callback for handling request */
static int write_enable(_CALLBACK_FUNCTION_ARG_);
static int volatile_sr_write_enable(_CALLBACK_FUNCTION_ARG_);
static int write_disable(_CALLBACK_FUNCTION_ARG_);

static int release_power_down_or_id(_CALLBACK_FUNCTION_ARG_);
static int manufacturer_or_device_id(_CALLBACK_FUNCTION_ARG_);
static int jedec_id(_CALLBACK_FUNCTION_ARG_);
static int read_unique_id(_CALLBACK_FUNCTION_ARG_);

static int read_data(_CALLBACK_FUNCTION_ARG_);
static int fast_read(_CALLBACK_FUNCTION_ARG_);

static int page_program(_CALLBACK_FUNCTION_ARG_);

static int sector_erase_4kb(_CALLBACK_FUNCTION_ARG_);
static int block_erase_32kb(_CALLBACK_FUNCTION_ARG_);
static int block_erase_64kb(_CALLBACK_FUNCTION_ARG_);
static int chip_erase(_CALLBACK_FUNCTION_ARG_);

static int read_status_register_1(_CALLBACK_FUNCTION_ARG_);
static int write_status_register_1(_CALLBACK_FUNCTION_ARG_);
static int read_status_register_2(_CALLBACK_FUNCTION_ARG_);
static int write_status_register_2(_CALLBACK_FUNCTION_ARG_);
static int read_status_register_3(_CALLBACK_FUNCTION_ARG_);
static int write_status_register_3(_CALLBACK_FUNCTION_ARG_);

static int read_sfdp_register(_CALLBACK_FUNCTION_ARG_);
static int erase_security_register(_CALLBACK_FUNCTION_ARG_);
static int program_security_register(_CALLBACK_FUNCTION_ARG_);
static int read_security_register(_CALLBACK_FUNCTION_ARG_);

static int global_block_lock(_CALLBACK_FUNCTION_ARG_);
static int global_block_unlock(_CALLBACK_FUNCTION_ARG_);
static int read_block_lock(_CALLBACK_FUNCTION_ARG_);
static int individual_block_lock(_CALLBACK_FUNCTION_ARG_);
static int individual_block_unlock(_CALLBACK_FUNCTION_ARG_);

static int erase_or_program_suspend(_CALLBACK_FUNCTION_ARG_);
static int erase_or_program_resume(_CALLBACK_FUNCTION_ARG_);
static int power_down(_CALLBACK_FUNCTION_ARG_);

static int enable_reset(_CALLBACK_FUNCTION_ARG_);
static int reset_device(_CALLBACK_FUNCTION_ARG_);


static int dual_qspi_fast_read_dual_output(_CALLBACK_FUNCTION_ARG_);

static int dual_qspi_fast_read_dual_io(_CALLBACK_FUNCTION_ARG_);
static int dual_qspi_manufacturer_or_device_id_dual_io(_CALLBACK_FUNCTION_ARG_);

static int dual_qspi_quad_input_page_program(_CALLBACK_FUNCTION_ARG_);
static int dual_qspi_fast_read_quad_output(_CALLBACK_FUNCTION_ARG_);

static int dual_qspi_manufacturer_or_device_id_quad_io(_CALLBACK_FUNCTION_ARG_);
static int dual_qspi_fast_read_quad_io(_CALLBACK_FUNCTION_ARG_);
static int dual_qspi_set_burst_with_warp(_CALLBACK_FUNCTION_ARG_);

static int __not_implemented(_CALLBACK_FUNCTION_ARG_);


static uint8_t get_jedec216_data(uint32_t address);





/**
	* **************************************************
	* private variable declaration and definition
	*/
static const char * const __empty_string = "";

static spi_flash_command_callback_t callback_winbond[] = {
	/* 0x00 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x01 */ {.callback = write_status_register_1,                     .function_name = "Write Status Register-1"},
	/* 0x02 */ {.callback = page_program,                                .function_name = "Page Program"},
	/* 0x03 */ {.callback = read_data,                                   .function_name = "Read Data"},
	/* 0x04 */ {.callback = write_disable,                               .function_name = "Write Disable"},
	/* 0x05 */ {.callback = read_status_register_1,                      .function_name = "Read Status Register-1"},
	/* 0x06 */ {.callback = write_enable,                                .function_name = "Write Enable"},
	/* 0x07 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x08 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x09 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x0a */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x0b */ {.callback = fast_read,                                   .function_name = "Fast Read"},
	/* 0x0c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x0d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x0e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x0f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x10 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x11 */ {.callback = write_status_register_3,                     .function_name = "Write Status Register-3"},
	/* 0x12 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x13 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x14 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x15 */ {.callback = read_status_register_3,                      .function_name = "Read Status Register-3"},
	/* 0x16 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x17 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x18 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x19 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x1a */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x1b */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x1c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x1d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x1e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x1f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x20 */ {.callback = sector_erase_4kb,                            .function_name = "Sector Erase (4 kiB)"},
	/* 0x21 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x22 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x23 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x24 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x25 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x26 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x27 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x28 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x29 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x2a */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x2b */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x2c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x2d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x2e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x2f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x30 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x31 */ {.callback = write_status_register_2,                     .function_name = "Write Status Register-2"},
	/* 0x32 */ {.callback = dual_qspi_quad_input_page_program,           .function_name = "Dual/Quad SPI Quad Input Page Program"},
	/* 0x33 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x34 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x35 */ {.callback = read_status_register_2,                      .function_name = "Read Status Register-2"},
	/* 0x36 */ {.callback = individual_block_lock,                       .function_name = "Individual Block Lock"},
	/* 0x37 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x38 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x39 */ {.callback = individual_block_unlock,                     .function_name = "Individual Block Unlock"},
	/* 0x3a */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x3b */ {.callback = dual_qspi_fast_read_dual_output,             .function_name = "Dual/Quad SPI Fast Read Dual Output"},
	/* 0x3c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x3d */ {.callback = read_block_lock,                             .function_name = __empty_string},
	/* 0x3e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x3f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x40 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x41 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x42 */ {.callback = program_security_register,                   .function_name = "Program Security Register"},
	/* 0x43 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x44 */ {.callback = erase_security_register,                     .function_name = "Erase Security Register"},
	/* 0x45 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x46 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x47 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x48 */ {.callback = read_security_register,                      .function_name = "Read Security Register"},
	/* 0x49 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x4a */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x4b */ {.callback = read_unique_id,                              .function_name = "Read Unique ID"},
	/* 0x4c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x4d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x4e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x4f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x50 */ {.callback = volatile_sr_write_enable,                    .function_name = "Volatile SR Write Enable"},
	/* 0x51 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x52 */ {.callback = block_erase_32kb,                            .function_name = "Block Erase (32 kiB)"},
	/* 0x53 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x54 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x55 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x56 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x57 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x58 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x59 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x5a */ {.callback = read_sfdp_register,                          .function_name = "Read SFDP Register"},
	/* 0x5b */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x5c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x5d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x5e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x5f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x60 */ {.callback = chip_erase,                                  .function_name = "Chip Erase"},
	/* 0x61 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x62 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x63 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x64 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x65 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x66 */ {.callback = enable_reset,                                .function_name = "Enable Reset"},
	/* 0x67 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x68 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x69 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x6a */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x6b */ {.callback = dual_qspi_fast_read_quad_output,             .function_name = "Dual/Quad SPI Fast Read Quad Output"},
	/* 0x6c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x6d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x6e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x6f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x70 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x71 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x72 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x73 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x74 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x75 */ {.callback = erase_or_program_suspend,                    .function_name = "Erase/Program Suspend"},
	/* 0x76 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x77 */ {.callback = dual_qspi_set_burst_with_warp,               .function_name = "Dual/Quad SPI Set Burst with Warp"},
	/* 0x78 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x79 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x7a */ {.callback = erase_or_program_resume,                     .function_name = "Erase/Program Resume"},
	/* 0x7b */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x7c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x7d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x7e */ {.callback = global_block_lock,                           .function_name = "Global Block Lock"},
	/* 0x7f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x80 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x81 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x82 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x83 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x84 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x85 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x86 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x87 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x88 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x89 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x8a */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x8b */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x8c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x8d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x8e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x8f */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x90 */ {.callback = manufacturer_or_device_id,                   .function_name = "Manufacturer/Device ID"},
	/* 0x91 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x92 */ {.callback = dual_qspi_manufacturer_or_device_id_dual_io, .function_name = "Dual/Quad SPI Manufacturer/Device ID Dual I/O"},
	/* 0x93 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x94 */ {.callback = dual_qspi_manufacturer_or_device_id_quad_io, .function_name = "Dual/Quad SPI Manufacturer/Device ID Quad I/O"},
	/* 0x95 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x96 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x97 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x98 */ {.callback = global_block_unlock,                         .function_name = "Global Block Unlock"},
	/* 0x99 */ {.callback = reset_device,                                .function_name = "Reset Device"},
	/* 0x9a */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x9b */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x9c */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x9d */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x9e */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0x9f */ {.callback = jedec_id,                                    .function_name = "JEDEC ID"},
	/* 0xa0 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa1 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa2 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa3 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa4 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa5 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa6 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa7 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa8 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xa9 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xaa */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xab */ {.callback = release_power_down_or_id,                    .function_name = "Release Power Down/ID"},
	/* 0xac */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xad */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xae */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xaf */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb0 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb1 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb2 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb3 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb4 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb5 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb6 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb7 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb8 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xb9 */ {.callback = power_down,                                  .function_name = "Power Down"},
	/* 0xba */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xbb */ {.callback = dual_qspi_fast_read_dual_io,                 .function_name = "Dual/Quad SPI Fast Read Dual I/O"},
	/* 0xbc */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xbd */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xbe */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xbf */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc0 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc1 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc2 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc3 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc4 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc5 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc6 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc7 */ {.callback = chip_erase,                                  .function_name = "Chip Erase"},
	/* 0xc8 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xc9 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xca */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xcb */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xcc */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xcd */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xce */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xcf */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd0 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd1 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd2 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd3 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd4 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd5 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd6 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd7 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xd8 */ {.callback = block_erase_64kb,                            .function_name = "Block Erase (64 kiB)"},
	/* 0xd9 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xda */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xdb */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xdc */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xdd */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xde */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xdf */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe0 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe1 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe2 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe3 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe4 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe5 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe6 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe7 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe8 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xe9 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xea */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xeb */ {.callback = dual_qspi_fast_read_quad_io,                 .function_name = "Dual/Quad SPI Fast Read Quad I/O"},
	/* 0xec */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xed */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xee */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xef */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf0 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf1 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf2 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf3 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf4 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf5 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf6 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf7 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf8 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xf9 */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xfa */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xfb */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xfc */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xfd */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xfe */ {.callback = NULL,                                        .function_name = __empty_string},
	/* 0xff */ {.callback = NULL,                                        .function_name = __empty_string}
};


/**
	* flash state
	*/
static bool        this_is_start_trx_session = true;

static uint8_t     spi_flash_command_code;

static bool        has_active_address = false;
static size_t      active_address;

static uint8_t     spi_temp_buffer[SPI_FLASH_SECTOR_SIZE_BYTES];
static size_t      spi_byte_counter;

/**
	* **************************************************
	* function definition
	*/

int spi_flash_simulation_init(void)
{
	/**
		* call fini to clean up
		*/
	spi_flash_simulation_fini();

	/**
		* TODO: initialize state
		*/

	/* set this_is_start_trx_session to true */
	this_is_start_trx_session = true;



	return 0;
}

/* fini, same process as closing file in init */
void spi_flash_simulation_fini(void)
{
}

/**
	* handle discontinue command of spim
	*/
int spi_flash_trx_session(const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_stop)
{
	const spi_flash_command_callback_t *spi_flash_command_callback;

	int retval;

	/**
		* make sure tx and rx is valid
		* spi cannot send NULL to simulated hardware
		*/
	if ((tx == NULL) || (rx == NULL) || (len < 1))
	{
		return (-1);
	}

	if (this_is_start_trx_session)
	{
		/* get the code */
		spi_flash_command_code = tx[0];

		/* reset global byte counter */
		spi_byte_counter = 0;
	}

	/* get callback from global state variable spi_flash_command_code */
	spi_flash_command_callback = &(callback_winbond[spi_flash_command_code]);

	/* check if callback is NULL */
	if ((spi_flash_command_callback->callback) == NULL)
	{
		/**
			* we should leave spi_rx hi-z by not setting any value
			* but valgrind going to throw error when
			* caller try to copy data from rx buffer
			*/
		memset(rx, 0, len * sizeof(uint8_t));

		/* return ok */
		return 0;
	}

	/* callback is not null, call the function without any buffer modification */
	retval = spi_flash_command_callback->callback(spi_flash_command_code, tx, rx, len,
		this_is_start_trx_session, is_stop);

	/* next transaction won't be a new session */
	this_is_start_trx_session = false;

	/* check if this is end of transaction */
	if (is_stop)
	{
		/* next spi transaction will start new session,
		* set this_is_start_trx_session to true */
		this_is_start_trx_session = true;
	}

	return retval;
}

/**
	* spi device callback
	*/
static int write_enable(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int volatile_sr_write_enable(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int write_disable(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int release_power_down_or_id(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int manufacturer_or_device_id(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int jedec_id(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int read_unique_id(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int read_data(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int fast_read(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int page_program(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int sector_erase_4kb(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int block_erase_32kb(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int block_erase_64kb(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int chip_erase(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int read_status_register_1(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int write_status_register_1(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int read_status_register_2(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int write_status_register_2(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int read_status_register_3(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int write_status_register_3(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int read_sfdp_register(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	bool   add_dummy_bytes = false;
	size_t prev_spi_byte_counter;

	int retval = 0;

	/* FIXME: use code to check if this function is called with correct code */
	(void) code;

	/**
		* invalidate active_address and spi_byte_counter when session is started
		*/
	if (is_start)
	{
		spi_byte_counter = 0;
		has_active_address = false;
	}

	/* record spi_byte_counter before it being incremented */
	prev_spi_byte_counter = spi_byte_counter;

	/* accumulate received byte */
	if ((spi_byte_counter + len) > SPI_FLASH_SECTOR_SIZE_BYTES)
	{
		/* temporary storage will be overflow */
		return (-1);
	}

	/* copy data to temporary buffer */
	memcpy(&(spi_temp_buffer[spi_byte_counter]), tx, len * sizeof(uint8_t));

	/* increment counter */
	spi_byte_counter += len;

	/**
		* update active address if
		* 1. has_active_address is false
		* 2. we have enough data (5 byte: code, 3 byte address, 1 dummy)
		*/
	if ((! has_active_address) && (spi_byte_counter >= 5))
	{
		active_address = 0; /* set to 0 */

		/* msb */
		active_address  |= spi_temp_buffer[1];
		active_address <<= 8;

		active_address  |= spi_temp_buffer[2];
		active_address <<= 8;

		active_address  |= spi_temp_buffer[3]; /* lsb */
		/* do not shift lsb */

		/* validate address */
		has_active_address = true;

		/* invalidate buffer */
		spi_byte_counter -= 5;
		if (spi_byte_counter > 0)
		{
			/* move rest of content to start of buffer */
			memmove(spi_temp_buffer, spi_temp_buffer + 5, spi_byte_counter);
		}

		/* add dummy bytes, we don't need to add data to rx at address bytes */
		add_dummy_bytes = true;
	}


	/* do we need to prepend rx with dummy data */
	if (! has_active_address)
	{
		/* we still don't have active address because len is not enough */
		memset(rx, 0, len * sizeof(uint8_t));
	}
	else
	{
		/* has active address */
		size_t i;

		/**
			* we have active address but somehow previous transaction
			* to not send address completely, so in here
			* we receive all address bytes
			*
			* we only need to add dummy when previous counter actually
			* less than code and address bytes
			* but in case we actually already send data, previous counter will
			* also have value less than 5
			* we different those condition with add_dummy_bytes flag
			* which only true when get full address bytes
			*/
		if (add_dummy_bytes && (prev_spi_byte_counter < 5))
		{
			/* add dummy bytes */
			size_t dummy_len = 5 - prev_spi_byte_counter;

			assert(len >= dummy_len); /* make sure dummy length not cause overflow */

			/* add dummy bytes */
			memset(rx, 0, dummy_len * sizeof(uint8_t));

			/* reduce buffer and length */

			rx  += dummy_len;
			len -= dummy_len;
		}

		/* read data only if len is not 0 */
		/* read data */
		for (i = 0; i < len; ++i, ++active_address)
		{
			rx[i] = get_jedec216_data((uint32_t) active_address);
		}

		/* tx is consumed when we have active address */
		spi_byte_counter = 0;
	}


	/* when is_stop is true, or reset happen, reset global state */

	if ((is_stop) || (retval != 0))
	{
		/* invalidate address */
		has_active_address = false;

		/* clean up buffer */
		spi_byte_counter = 0;
	}

	return retval;
}

static int erase_security_register(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int program_security_register(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int read_security_register(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int global_block_lock(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int global_block_unlock(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int read_block_lock(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int individual_block_lock(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int individual_block_unlock(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int erase_or_program_suspend(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int erase_or_program_resume(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int power_down(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int enable_reset(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int reset_device(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int dual_qspi_fast_read_dual_output(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int dual_qspi_fast_read_dual_io(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int dual_qspi_manufacturer_or_device_id_dual_io(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int dual_qspi_quad_input_page_program(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int dual_qspi_fast_read_quad_output(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int dual_qspi_manufacturer_or_device_id_quad_io(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int dual_qspi_fast_read_quad_io(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int dual_qspi_set_burst_with_warp(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int __not_implemented(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	const spi_flash_command_callback_t *spi_flash_command_callback;

	(void) tx;
	(void) rx;
	(void) len;
	(void) is_start;
	(void) is_stop;

	spi_flash_command_callback = &(callback_winbond[code]);

	fprintf(stderr, "handling for code 0x%02x (%s) is not implemented\n",
		code, spi_flash_command_callback->function_name);

	/* return error */
	return (-1);
}

static uint8_t get_jedec216_data(uint32_t address)
{
	const uint32_t parameter1_header1 = 0x09000000u |
		((JESD216_PARAMETER1_VERSION_MAJOR                      & 0xffu) << 16) |
		((JESD216_PARAMETER1_VERSION_MINOR                      & 0xffu) <<  8);

	const uint32_t parameter1_header2 = 0xff000000u |
		((JESD216_PARAMETER1_ADDRESS                        & 0xffffffu) <<  0);


	const uint32_t parameter1_value1 = 0xff8000e0u |
		((JESD216_PARAMETER1_VALUE1_SUPPORT_1_1_4_FAST_READ     &  0x1u) << 22)  |
		((JESD216_PARAMETER1_VALUE1_SUPPORT_1_4_4_FAST_READ     &  0x1u) << 21)  |
		((JESD216_PARAMETER1_VALUE1_SUPPORT_1_2_2_FAST_READ     &  0x1u) << 20)  |
		((JESD216_PARAMETER1_VALUE1_SUPPORT_DOUBLE_TRANSFER_RATE_CLOCKING & 0x1u) << 19) |
		((JESD216_PARAMETER1_VALUE1_ADDRESS_BYTE                &  0x3u) << 17)  |
		((JESD216_PARAMETER1_VALUE1_SUPPORT_1_1_2_FAST_READ     &  0x1u) << 16)  |
		((JESD216_PARAMETER1_VALUE1_4KB_ERASE_OPCODE            & 0xffu) <<  8)  |
		((JESD216_PARAMETER1_VALUE1_WRITE_ENABLE_OPCODE_SELECT_FOR_WRITING_TO_VOLATILE_STATUS_REGISTER & 0x1u) <<  4) |
		((JESD216_PARAMETER1_VALUE1_WRITE_ENABLE_INSTRUCTION_REQUIRED_FOR_WRITING_TO_VOLATILE_STATUS_REGISTER & 0x1u) <<  3) |
		((JESD216_PARAMETER1_VALUE1_WRITE_GRANULARITY           &  0x1u) <<  2)  |
		((JESD216_PARAMETER1_VALUE1_BLOCK_OR_SECTOR_ERASE_SIZES &  0x3u) <<  0);

	const uint32_t parameter1_value2 = JESD216_PARAMETER1_VALUE2_MEMORY_DENSITY_IN_BIT;

	const uint32_t parameter1_value3 =
		((JESD216_PARAMETER1_VALUE3_1_1_4_FAST_READ_OPCODE      & 0xffu) << 24) |
		((JESD216_PARAMETER1_VALUE3_1_1_4_FAST_READ_NUMBER_OF_MODE_BITS &  0x7u) << 21) |
		((JESD216_PARAMETER1_VALUE3_1_1_4_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT & 0x1fu) << 16) |
		((JESD216_PARAMETER1_VALUE3_1_4_4_FAST_READ_OPCODE      & 0xffu) <<  8) |
		((JESD216_PARAMETER1_VALUE3_1_4_4_FAST_READ_NUMBER_OF_MODE_BITS &  0x7u) <<  5) |
		((JESD216_PARAMETER1_VALUE3_1_4_4_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT & 0x1fu) <<  0);

	const uint32_t parameter1_value4 =
		((JESD216_PARAMETER1_VALUE4_1_2_2_FAST_READ_OPCODE      & 0xffu) << 24) |
		((JESD216_PARAMETER1_VALUE4_1_2_2_FAST_READ_NUMBER_OF_MODE_BITS & 0x7u) << 21) |
		((JESD216_PARAMETER1_VALUE4_1_2_2_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT & 0x1fu) << 16) |
		((JESD216_PARAMETER1_VALUE4_1_1_2_FAST_READ_OPCODE      & 0xffu) <<  8) |
		((JESD216_PARAMETER1_VALUE4_1_1_2_FAST_READ_NUMBER_OF_MODE_BITS & 0x1u) <<  5) |
		((JESD216_PARAMETER1_VALUE4_1_1_2_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT & 0x1fu) <<  0);


	const uint32_t parameter1_value5 = 0xffffffeeu |
		((JESD216_PARAMETER1_VALUE5_SUPPORT_4_4_4_FAST_READ     &  0x1u) <<  4) |
		((JESD216_PARAMETER1_VALUE5_SUPPORT_2_2_2_FAST_READ     &  0x1u) <<  0);

	const uint32_t parameter1_value6 = 0x0000ffffu |
		((JESD216_PARAMETER1_VALUE6_2_2_2_FAST_READ_OPCODE      & 0xffu) << 24) |
		((JESD216_PARAMETER1_VALUE6_2_2_2_FAST_READ_NUMBER_OF_MODE_BITS &  0x7u) << 21) |
		((JESD216_PARAMETER1_VALUE6_2_2_2_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT & 0x1fu) << 16);

	const uint32_t parameter1_value7 = 0x0000ffffu |
		((JESD216_PARAMETER1_VALUE7_4_4_4_FAST_READ_OPCODE      & 0xffu) << 24) |
		((JESD216_PARAMETER1_VALUE7_4_4_4_FAST_READ_NUMBER_OF_MODE_BITS &  0x7u) << 21) |
		((JESD216_PARAMETER1_VALUE7_4_4_4_FAST_READ_NUMBER_OF_DUMMY_CLOCKS_NEEDED_BEFORE_VALID_OUTPUT & 0x1fu) << 16);

	const uint32_t parameter1_value8 =
		((JESD216_PARAMETER1_VALUE8_ERASE_SECTOR_TYPE_2_OPCODE  & 0xffu) << 24) |
		((JESD216_PARAMETER1_VALUE8_ERASE_SECTOR_TYPE_2_SIZE    & 0xffu) << 16) |
		((JESD216_PARAMETER1_VALUE8_ERASE_SECTOR_TYPE_1_OPCODE  & 0xffu) <<  8) |
		((JESD216_PARAMETER1_VALUE8_ERASE_SECTOR_TYPE_1_SIZE    & 0xffu) <<  0);

	const uint32_t parameter1_value9 =
		((JESD216_PARAMETER1_VALUE9_ERASE_SECTOR_TYPE_4_OPCODE  & 0xffu) << 24) |
		((JESD216_PARAMETER1_VALUE9_ERASE_SECTOR_TYPE_4_SIZE    & 0xffu) << 16) |
		((JESD216_PARAMETER1_VALUE9_ERASE_SECTOR_TYPE_3_OPCODE  & 0xffu) <<  8) |
		((JESD216_PARAMETER1_VALUE9_ERASE_SECTOR_TYPE_3_SIZE    & 0xffu) <<  0);


	switch (address)
	{
		/* 1st header (JEDEC STANDARDIZED): 0x50444653 ("SFDP") */
		case ( 0): return (uint8_t) 'S';
		case ( 1): return (uint8_t) 'F';
		case ( 2): return (uint8_t) 'D';
		case ( 3): return (uint8_t) 'P';

		/* 2nd header (PURPOSE IS JEDEC STANDARDIZED): 0xff <nph> <major rev> <minor rev> ("SFDP") */
		case ( 4): return (uint8_t) JESD216_PARAMETER1_VERSION_MINOR;
		case ( 5): return (uint8_t) JESD216_PARAMETER1_VERSION_MAJOR;
		case ( 6): return 0x00; /* only 1 header */
		case ( 7): return 0xff;

		/* 1st parameter header (PURPOSE IS JEDEC STANDARDIZED): <ptl> <major rev> <minor rev> <id number> */
		case ( 8): return (uint8_t) ( parameter1_header1 & 0x000000ff       );
		case ( 9): return (uint8_t) ((parameter1_header1 & 0x0000ff00) >>  8);
		case (10): return (uint8_t) ((parameter1_header1 & 0x00ff0000) >> 16);
		case (11): return (uint8_t) ((parameter1_header1 & 0xff000000) >> 24);

		/* 2st parameter header (PURPOSE IS JEDEC STANDARDIZED): 0xff <ptp> */
		case (12): return (uint8_t) ( parameter1_header2 & 0x000000ff       );
		case (13): return (uint8_t) ((parameter1_header2 & 0x0000ff00) >>  8);
		case (14): return (uint8_t) ((parameter1_header2 & 0x00ff0000) >> 16);
		case (15): return (uint8_t) ((parameter1_header2 & 0xff000000) >> 24);


		/* parameter jedec standardized 1-9 parameter */

		/* parameter: 1 */
		case (JESD216_PARAMETER1_ADDRESS +  0): return (uint8_t) ( parameter1_value1 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS +  1): return (uint8_t) ((parameter1_value1 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS +  2): return (uint8_t) ((parameter1_value1 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS +  3): return (uint8_t) ((parameter1_value1 & 0xff000000) >> 24);

		/* parameter: 2 */
		case (JESD216_PARAMETER1_ADDRESS +  4): return (uint8_t) ( parameter1_value2 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS +  5): return (uint8_t) ((parameter1_value2 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS +  6): return (uint8_t) ((parameter1_value2 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS +  7): return (uint8_t) ((parameter1_value2 & 0xff000000) >> 24);

		/* parameter: 3 */
		case (JESD216_PARAMETER1_ADDRESS +  8): return (uint8_t) ( parameter1_value3 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS +  9): return (uint8_t) ((parameter1_value3 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS + 10): return (uint8_t) ((parameter1_value3 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS + 11): return (uint8_t) ((parameter1_value3 & 0xff000000) >> 24);

		/* parameter: 4 */
		case (JESD216_PARAMETER1_ADDRESS + 12): return (uint8_t) ( parameter1_value4 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS + 13): return (uint8_t) ((parameter1_value4 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS + 14): return (uint8_t) ((parameter1_value4 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS + 15): return (uint8_t) ((parameter1_value4 & 0xff000000) >> 24);

		/* parameter: 5 */
		case (JESD216_PARAMETER1_ADDRESS + 16): return (uint8_t) ( parameter1_value5 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS + 17): return (uint8_t) ((parameter1_value5 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS + 18): return (uint8_t) ((parameter1_value5 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS + 19): return (uint8_t) ((parameter1_value5 & 0xff000000) >> 24);

		/* parameter: 6 */
		case (JESD216_PARAMETER1_ADDRESS + 20): return (uint8_t) ( parameter1_value6 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS + 21): return (uint8_t) ((parameter1_value6 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS + 22): return (uint8_t) ((parameter1_value6 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS + 23): return (uint8_t) ((parameter1_value6 & 0xff000000) >> 24);

		/* parameter: 7 */
		case (JESD216_PARAMETER1_ADDRESS + 24): return (uint8_t) ( parameter1_value7 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS + 25): return (uint8_t) ((parameter1_value7 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS + 26): return (uint8_t) ((parameter1_value7 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS + 27): return (uint8_t) ((parameter1_value7 & 0xff000000) >> 24);

		/* parameter: 8 */
		case (JESD216_PARAMETER1_ADDRESS + 28): return (uint8_t) ( parameter1_value8 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS + 29): return (uint8_t) ((parameter1_value8 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS + 30): return (uint8_t) ((parameter1_value8 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS + 31): return (uint8_t) ((parameter1_value8 & 0xff000000) >> 24);

		/* parameter: 9 */
		case (JESD216_PARAMETER1_ADDRESS + 32): return (uint8_t) ( parameter1_value9 & 0x000000ff       );
		case (JESD216_PARAMETER1_ADDRESS + 33): return (uint8_t) ((parameter1_value9 & 0x0000ff00) >>  8);
		case (JESD216_PARAMETER1_ADDRESS + 34): return (uint8_t) ((parameter1_value9 & 0x00ff0000) >> 16);
		case (JESD216_PARAMETER1_ADDRESS + 35): return (uint8_t) ((parameter1_value9 & 0xff000000) >> 24);

		default: return 0xff;
	}
}
