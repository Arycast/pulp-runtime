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
static int get_absolute_path_from_relative_path_to_executable(
	char ** restrict absolute_path, const char * restrict path);

static void *__non_standard_memrchr(const void *s, int c, size_t n);

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






/* simulated flash manipulation */
static int simulate_erase_sector(size_t start_address, size_t length);
static int simulate_read_data(size_t start_address, void *buffer, size_t length);
static int simulate_write_data(size_t start_address, const void *buffer, size_t length);

/**
	* **************************************************
	* private variable declaration and definition
	*/
static FILE *spi_flash_fd = NULL;

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
static bool        is_write_enable = false;

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
	const char                   *flash_file = FLASH_FILE;
	char        *absolute_path_to_flash_file;

	struct stat   statbuf;

	int fd;

	int _errno;
	int retval;

	/**
		* call fini to help close lingering opened file
		* on fini return, spi_flash_fd will be set to NULL
		*/
	spi_flash_simulation_fini();

	assert(spi_flash_fd == NULL); /* should be set to NULL by fini */

	/* try to find out where the spi flash file is */
	switch (flash_file[0])
	{
	case ('\0'):
		/* empty string */
		return (-1); /* error, file should be used */
		break; /* control won't reach here */

	case ('/'):
		/**
			* absolute path
			* remove const qualifier, but DON'T
			*/
		absolute_path_to_flash_file = (char *) flash_file;
		break;

	default:
		/**
			* flash_file is relative path, get absolute path
			* set absolute_path_to_flash_file to NULL first
			*/
		absolute_path_to_flash_file = NULL; /* make sure to set this to NULL */
		retval = get_absolute_path_from_relative_path_to_executable(
			&absolute_path_to_flash_file, flash_file);
		if (retval != 0)
		{
			/**
				* cannot resolve path, maybe this system is not posix compliant
				* or memory allocation is failed
				*/
			fprintf(stderr, "cannot resolve absolute path of flash_file: %s\n",
				flash_file);
			if (absolute_path_to_flash_file != NULL)
			{
				/* free allocation if necessary */
				free(absolute_path_to_flash_file);
			}

			/* return error */
			return (-2);
		}
	}

	/* absolute path should not NULL */
	assert(absolute_path_to_flash_file != NULL);

	/**
		* open file, use open instead of fopen, because
		* fopen don't offer mode O_RDWR | O_CREAT
		* "r+" don't include O_CREAT
		* "w+" include O_TRUNC which will empty out file
		*      which is undesired
		*
		* the downside is open (2) is posix function while
		* fopen is c11 compliant (usually compliance to c11 is more desirable)
		*
		* file will be created if not exist
		*/
	errno = 0;
	retval = open(absolute_path_to_flash_file, O_RDWR | O_CREAT,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	_errno = errno;
	if (retval == (-1))
	{
		const char *s;

		/* if error, e.g. permission denied, errno should not zero */
		assert(_errno != 0);

		s = strerror(_errno);
		if (s == NULL)
		{
			fprintf(stderr, "open(\"%s\") error: %d, " \
				"but strerror cannot stringify the error\n",
				absolute_path_to_flash_file, _errno);
		}
		else
		{
			fputs(s, stderr);
			fputc('\n', stderr);
		}

		/* free absolute path */
		if (((const char *) absolute_path_to_flash_file) != flash_file)
		{
			free(absolute_path_to_flash_file);
		}

		/* return error, don't need to abort */
		return (-3);
	}

	/* open should not return negative number */
	assert((retval >= 0) && (_errno == 0));

	/* retval contain file descriptor fd */
	fd = retval;

	/* after opened, free absolute path */
	if (((const char *) absolute_path_to_flash_file) != flash_file)
	{
		free(absolute_path_to_flash_file);
	}

	/* check if file size is correct */
	errno = 0;
	retval = fstat(fd, &statbuf);
	_errno = errno;

	/* should success */
	assert((retval == 0) && (_errno == 0));

	/* check and make sure we open regular file */
	/*if ((statbuf.st_mode & S_IFREG) == 0)*/
	if (! (S_ISREG(statbuf.st_mode)))
	{
		/* this file is not regular file */
		fputs("file \"" FLASH_FILE "\" is not regular file\n", stderr);

		/* close file */
		errno = 0;
		retval = close(fd);
		_errno = errno;

		/* should succeess */
		assert((retval == 0) && (_errno == 0));

		return (-4);
	}

	/* check file size */
	if (statbuf.st_size < SPI_FLASH_SIZE_BYTES)
	{
		/* truncate */
		off_t current_size = statbuf.st_size;
		off_t seek_position;

		uint8_t  default_block_value[SPI_FLASH_SECTOR_SIZE_BYTES];

		ssize_t   retval2;

		errno = 0;
		retval = ftruncate(fd, SPI_FLASH_SIZE_BYTES);
		_errno = errno;
		if (retval == (-1))
		{
			const char *s;
			/* truncate error */

			assert(_errno != 0);

			s = strerror(_errno);
			if (s == NULL)
			{
				fprintf(stderr, "ftruncate(\"%s\", %lu) error: %d, " \
					"but strerror cannot stringify the error\n",
					absolute_path_to_flash_file,
					(long unsigned int) SPI_FLASH_SIZE_BYTES, _errno);
			}
			else
			{
				fputs(s, stderr);
				fputc('\n', stderr);
			}

			/* close file */
			errno = 0;
			retval = close(fd);
			_errno = errno;

			/* should succeess */
			assert((retval == 0) && (_errno == 0));

			return (-5);
		}

		assert((retval == 0) && (_errno == 0));

		/* seek */
		errno = 0;
		seek_position = lseek(fd, current_size, SEEK_SET);
		_errno = errno;

		/* lseek should success */
		assert((seek_position == current_size) && (_errno == 0));

		/* set to default value: 0xff */
		memset(default_block_value, -1, SPI_FLASH_SECTOR_SIZE_BYTES * sizeof(uint8_t));

		/**
			* write data to truncated block
			*/
		do
		{
			off_t new_seek_position = seek_position + SPI_FLASH_SECTOR_SIZE_BYTES;

			size_t write_length;

			/* check if new_seek_position will overflow */
			if (new_seek_position > SPI_FLASH_SIZE_BYTES)
			{
				new_seek_position = SPI_FLASH_SIZE_BYTES;
			}

			/* calculate how much byte we need to write */
			write_length = (size_t) (new_seek_position - seek_position);

			/* write_length will never longer than SPI_FLASH_SECTOR_SIZE_BYTES */
			assert(write_length <= SPI_FLASH_SECTOR_SIZE_BYTES);

			errno = 0;
			retval2 = write(fd, default_block_value, write_length);
			_errno = errno;

			/* write should not error */
			assert((retval2 == ((ssize_t) write_length)) && (_errno == 0));

			/* update seek position */
			seek_position = new_seek_position;
		}
		while (seek_position < ((off_t) SPI_FLASH_SIZE_BYTES));

		/* return seek pointer to start of file */
		errno = 0;
		seek_position = lseek(fd, 0, SEEK_SET);
		_errno = errno;

		/* should success */
		assert((seek_position == 0) && (_errno == 0));
	}

	/* change fd to FILE object */
	errno = 0;
	spi_flash_fd = fdopen(fd, "r+"); /* use r+ because file already created */
	_errno = errno;

	/* should success */
	assert((spi_flash_fd != NULL) && (_errno == 0));


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
	int _errno;
	int retval;

	if (spi_flash_fd != NULL)
	{
		/* close fd */
		errno = 0;
		retval = fclose(spi_flash_fd);
		_errno = errno;
		if (retval == EOF)
		{
			const char *s;

			/* errno must be non-zero (EBADF) */
			/*assert(_errno == EBADF);*/
			assert(_errno != 0);

			s = strerror(_errno);
			if (s == NULL)
			{
				fprintf(stderr, "fclose error (EOF): %d, " \
					"but strerror cannot stringify the error\n", _errno);
			}
			else
			{
				fputs(s, stderr);
				fputc('\n', stderr);
			}

			/* fatal error */
			abort();
		}

		/* if we reach here, file must be already closed */
		assert((retval == 0) && (_errno == 0));

		/* set spi_flash_fd to NULL */
		spi_flash_fd = NULL;
	}

	/* if spi_flash_fd is not NULL, we don't have to do anything */
}

static int get_absolute_path_from_relative_path_to_executable(
	char ** restrict absolute_path, const char * restrict path)
{
	char __absolute_path[PATH_MAX]; /* PATH_MAX is provided by limits.h */
	char * p;

	const char *s;

	ssize_t len;

	size_t  relative_path_len;

	/*int  retval;*/
	int _errno;

	/* check if input are sane */
	if ((absolute_path == NULL) || (path == NULL) ||
		(relative_path_len = strlen(path), relative_path_len <= 0))
	{
		/* input is not sane */
		return (-1);
	}

	/**
		* Read the symbolic link of current running executable
		* use /proc/self instead of /proc/<pid> so we can skip getpid call
		* and /proc/self is always (soft-) linked to current calling process
		*
		* purposely set buffer length to PATH_MAX so when readlink actually
		* write as much as PATH_MAX, we assume readlink output is truncated
		*/
	errno = 0;
	len = readlink("/proc/self/exe", __absolute_path, PATH_MAX);
	_errno = errno;

	switch (len)
	{
	case (-1):
		/* error */
		assert(_errno != 0);
		s = strerror(_errno);
		if (s == NULL)
		{
			fprintf(stderr, "readlink error: %d, " \
				"but strerror cannot stringify the error\n", _errno);
		}
		else
		{
			fputs(s, stderr);
			fputc('\n', stderr);
		}
		return (-2);
		break; /* control won't reach here */

	case (PATH_MAX):
		/* readlink is truncated */
		fputs("readlink(\"/proc/self/exec\") output truncated to PATH_MAX\n",
			stderr);
		return (-3); /* return error */
		break; /* control won't reach here */

	default:
		/* make sure we get correct path length */
		assert((len > 0) && (len < PATH_MAX));

		/* since readlink don't add '\0' to end of string, add it ourself */
		__absolute_path[len] = '\0';
	}

	/**
		* get position of last '/' and discard all content after it
		* do not need to search until len,
		* because __absolute_path won't end with '/'
		*
		* use memrchr, but since memrchr is gnu standard, implement it outself
		* we prefer to use memrchr since we already have string length (len)
		*/
	p = __non_standard_memrchr(__absolute_path, '/', (size_t) (len - 1) * sizeof(char));

	/**
		* since executable path returned from readlink is an absolute path,
		* it should have at least '/' which is root, except if this binary
		* run in non-POSIX-compliant system
		*/
	assert((p != NULL) && (((uintptr_t) p) >= ((uintptr_t) __absolute_path)));

	/* make sure p is '/' */
	assert((*p) == '/');

	/* make sure len */

	/* increment p then set that position character to zero termination */
	*++p = '\0';

	/* get new len */
	len = p - __absolute_path;

	/* allocate new buffer for absolute_path */
	*absolute_path = (char *) malloc((((size_t) len) + relative_path_len + 1) * sizeof(char)); /* +1 for '\0' */
	if ((*absolute_path) == NULL)
	{
		/* allocation failure */
		return (-4);
	}

	/* copy path */
	memcpy( *absolute_path,        __absolute_path, ((size_t) len) * sizeof(char));
	memcpy((*absolute_path) + len,            path, (relative_path_len + 1) * sizeof(char)); /* +1 for '\0' */

	return 0;
}

static void *__non_standard_memrchr(const void *s, int c, size_t n)
{
	size_t i;
	const uint8_t *_s;

	/* cannot found character if array is invalid */
	if ((s == NULL) || (n <= 0))
	{
		return NULL;
	}

	_s = (const uint8_t *) s; /* set _s to start of array first */

	/**
		* && operation is sequence point, so *s only called when i < n is true
		* when character c found in s, increment of i and decrement of _s
		* is not done
		*/
	for (i = 0, _s += (n - 1); (i < n) && ((*_s) != ((uint8_t) c)); ++i, --_s)
	{
	}

	/* exit condition is either index is out of bound or character c found */
	if (i >= n)
	{
		return NULL;
	}
	else
	{
		/* i < n, loop exit condition is *s == c */
		return (void *) _s;
	}
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
	/* FIXME: use code to check if this function is called with correct code */
	(void) code;


	/**
		* invalidate active_address and spi_byte_counter when session is started
		*/
	if (is_start)
	{
		spi_byte_counter = 0;
	}

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


	/* return 0 to rx */
	memset(rx, 0, len * sizeof(uint8_t));


	/**
		* process set is_write_enable true if
		* 1. is_stop is true
		*/

	/* when is_stop is true, reset global state and process actual erase */

	if (is_stop)
	{
		/* clean up buffer */
		spi_byte_counter = 0;

		/* automatically disable write */
		is_write_enable = true;
	}

	return 0;
}

static int volatile_sr_write_enable(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}

static int write_disable(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	/* FIXME: use code to check if this function is called with correct code */
	(void) code;


	/**
		* invalidate active_address and spi_byte_counter when session is started
		*/
	if (is_start)
	{
		spi_byte_counter = 0;
	}

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


	/* return 0 to rx */
	memset(rx, 0, len * sizeof(uint8_t));


	/**
		* process set is_write_enable false if
		* 1. is_stop is true
		*/

	/* when is_stop is true, reset global state and process actual erase */

	if (is_stop)
	{
		/* clean up buffer */
		spi_byte_counter = 0;

		/* automatically disable write */
		is_write_enable = false;
	}

	return 0;
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
		* 2. we have enough data (4 byte: code, 3 byte address)
		*/
	if ((! has_active_address) && (spi_byte_counter >= 4))
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
		spi_byte_counter -= 4;
		if (spi_byte_counter > 0)
		{
			/* move rest of content to start of buffer */
			memmove(spi_temp_buffer, spi_temp_buffer + 4, spi_byte_counter);
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

		/**
			* we have active address but somehow previous transaction
			* to not send address completely, so in here
			* we receive all address bytes
			*
			* we only need to add dummy when previous counter actually
			* less than code and address bytes
			* but in case we actually already send data, previous counter will
			* also have value less than 4
			* we different those condition with add_dummy_bytes flag
			* which only true when get full address bytes
			*/
		if (add_dummy_bytes && (prev_spi_byte_counter < 4))
		{
			/* add dummy bytes */
			size_t dummy_len = 4 - prev_spi_byte_counter;

			assert(len >= dummy_len); /* make sure dummy length not cause overflow */

			/* add dummy bytes */
			memset(rx, 0, dummy_len * sizeof(uint8_t));

			/* reduce buffer and length */

			rx  += dummy_len;
			len -= dummy_len;
		}

		/* read data only if len is not 0 */
		if (len > 0)
		{
			/* read data */
			retval = simulate_read_data(active_address, rx, len);
			if (retval == 0)
			{
				/* read ok, move active address */
				active_address += len;
			}
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

static int fast_read(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
}


static int page_program(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
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
		* 2. we have enough data (4 byte: code, 3 byte address)
		*/
	if ((! has_active_address) && (spi_byte_counter >= 4))
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
		spi_byte_counter -= 4;
		if (spi_byte_counter > 0)
		{
			/* move rest of content to start of buffer */
			memmove(spi_temp_buffer, spi_temp_buffer + 4, spi_byte_counter);
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

		/**
			* we have active address but somehow previous transaction
			* to not send address completely, so in here
			* we receive all address bytes
			*
			* move tx to proper position
			*/
		if (add_dummy_bytes && (prev_spi_byte_counter < 4))
		{
			/* add dummy bytes */
			size_t dummy_len = 4 - prev_spi_byte_counter;

			/* fill rx buffer */
			memset(rx, 0, dummy_len * sizeof(uint8_t));

			assert(len >= dummy_len); /* make sure dummy length not cause overflow */

			/* add dummy bytes and reduce buffer and length */
			tx  += dummy_len;
			rx  += dummy_len;
			len -= dummy_len;
		}

		/* write data only if len is not 0 */
		if (len > 0)
		{
			/* read data */
			retval = simulate_write_data(active_address, tx, len);
			if (retval == 0)
			{
				/* read ok, move active address */
				active_address += len;
			}

			/* fill rx */
			memset(rx, 0, len * sizeof(uint8_t));
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


static int sector_erase_4kb(uint8_t code, const uint8_t * restrict tx, uint8_t * restrict rx, size_t len, bool is_start, bool is_stop)
{
	int retval;

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
		* 2. we have enough data (4 byte: code, 3 byte address)
		*/
	if ((! has_active_address) && (spi_byte_counter >= 4))
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
		spi_byte_counter -= 4;
		if (spi_byte_counter > 0)
		{
			/* move rest of content */
			memmove(spi_temp_buffer, spi_temp_buffer + 4, spi_byte_counter);
		}
	}


	/* return 0 to rx */
	memset(rx, 0, len * sizeof(uint8_t));


	/**
		* process erase only if
		* 1. is_write_enable true
		* 2. address is valid
		* 3. is_stop is true
		*/

	/* when is_stop is true, reset global state and process actual erase */

	if (is_stop)
	{
		if (is_write_enable && has_active_address)
		{
			/* to erase sector, write_enable and has_active_address need to be true */
			retval = simulate_erase_sector(active_address, SPI_FLASH_SECTOR_SIZE_BYTES);
		}

		/* invalidate address */
		has_active_address = false;

		/* clean up buffer */
		spi_byte_counter = 0;

		/* automatically disable write */
		is_write_enable = false;
	}

	return retval;
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
	/* FIXME: use code to check if this function is called with correct code */
	(void) code;


	/**
		* invalidate active_address and spi_byte_counter when session is started
		*/
	if (is_start)
	{
		spi_byte_counter = 0;
	}

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

	/* we don't really care about TX value */

	/**
		* return 0 to rx
		* (we need to make sure this BUSY bit is 0)
		*/
	memset(rx, 0, len * sizeof(uint8_t));


	/* when is_stop is true, reset global state and process actual erase */

	if (is_stop)
	{
		/* clean up buffer */
		spi_byte_counter = 0;
	}

	return 0;
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
	return __not_implemented(code, tx, rx, len, is_start, is_stop);
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



static int simulate_erase_sector(size_t start_address, size_t length)
{
	/* prepare sector */
	uint8_t default_sector_fill[SPI_FLASH_SECTOR_SIZE_BYTES];

	int retval = 0;

	/* prepare sector filling */
	memset(default_sector_fill, -1, SPI_FLASH_SECTOR_SIZE_BYTES * sizeof(uint8_t));

	/* write fill */
	while ((retval == 0) && (length > 0))
	{
		/* take smallest value between length and SPI_FLASH_SECTOR_SIZE_BYTES */
		size_t write_length = (length > SPI_FLASH_SECTOR_SIZE_BYTES) ?
			SPI_FLASH_SECTOR_SIZE_BYTES : length;

		/* reuse write data */
		retval = simulate_write_data(start_address, default_sector_fill, write_length);
		if (retval == 0)
		{
			/* increment write_length */
			start_address += write_length;

			/* make sure we don't reduce length more than necessary */
			assert(length >= write_length);

			/* reduce length */
			length -= write_length;
		}
	}

	return retval;
}

static int simulate_read_data(size_t start_address, void *buffer, size_t length)
{
	int _errno;
	int retval;

	size_t retval2;

	/* check if length is non-zero and buffer is not null */
	if ((length <= 0) || (buffer == NULL))
	{
		return 0; /* just return (ok) if nothing to erase */
	}

	/* check if fd is open */
	if (spi_flash_fd == NULL)
	{
		/* nothing to read */
		return (-1);
	}

	/* seek and write file */
	errno = 0;
	retval = fseek(spi_flash_fd, (long) start_address, SEEK_SET); /* seek to start_address */
	_errno = errno;
	if (retval == (-1))
	{
		const char *s;

		assert(_errno != 0); /* errno should not 0 */

		/* get error message */
		s = strerror(_errno);

		/* print error message */
		if (s == NULL)
		{
			fprintf(stderr, "(read data) fseek error at address %lu\n", \
				(unsigned long) start_address);
		}
		else
		{
			fputs(s, stderr);
			fputc('\n', stderr);
		}

		/* return error */
		return (-2);
	}

	/* retval and errno should 0 when fseek is success */
	assert((retval == 0) && (_errno == 0));


	/* read data */

	/* fread should return number of byte read */
	retval2 = fread(buffer, sizeof(uint8_t), length, spi_flash_fd);
	if (retval2 != (length * sizeof(uint8_t)))
	{
		/* read less than required, fill rest of buffer with 0xff */
		fprintf(stderr, "fread return different number of byte: " \
			"%lu (expect: %lu)\n", retval2, length * sizeof(uint8_t));

		assert(retval2 < length);

		memset(((uint8_t *) buffer) + retval2, -1, length - retval2);
	}

	return 0;
}

static int simulate_write_data(size_t start_address, const void *buffer, size_t length)
{
	int _errno;
	int retval;

	size_t retval2;


	/* check if length is non-zero that we need to actually erase something */
	if ((buffer == NULL) || (length <= 0))
	{
		return 0; /* just return (ok) if nothing to erase */
	}

	/* check if fd is open */
	if (spi_flash_fd == NULL)
	{
		/* nothing to write */
		return (-1);
	}

	/* seek and write file */
	errno = 0;
	retval = fseek(spi_flash_fd, (long) start_address, SEEK_SET); /* seek to start_address */
	_errno = errno;
	if (retval == (-1))
	{
		const char *s;

		assert(_errno != 0); /* errno should not 0 */

		/* get error message */
		s = strerror(_errno);

		/* print error message */
		if (s == NULL)
		{
			fprintf(stderr, "(sector write) fseek error at address %lu\n", \
				(unsigned long) start_address);
		}
		else
		{
			fputs(s, stderr);
			fputc('\n', stderr);
		}

		/* return error */
		return (-2);
	}

	/* retval and errno should 0 when fseek is success */
	assert((retval == 0) && (_errno == 0));

	/* write */


	/* fwrite should return number of byte written */
	retval2 = fwrite(buffer, sizeof(uint8_t), length, spi_flash_fd);
	if (retval2 != (length * sizeof(uint8_t)))
	{
		/* write failed, we cannot handle this, abort */
		fprintf(stderr, "fwrite return different number of byte: " \
			"%lu (expect: %lu)\n", retval2, length * sizeof(uint8_t));
		abort();
	}

	return 0;
}
