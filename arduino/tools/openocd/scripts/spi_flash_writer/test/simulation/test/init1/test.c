/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <assert.h>

#include "../../spi_flash_simulation_posix.h"

int main(void)
{
	int retval;

	/* init */
	retval = spi_flash_simulation_init();

	/**
		* always success if disk space is available
		* if not success, change file path to another file system
		*/
	assert(retval == 0);

	/* fini */
	spi_flash_simulation_fini();

	return 0;
}
