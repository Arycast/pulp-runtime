/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include "spi_flash_simulation_posix.h"
#include "pulp_spim.h"

/**
	* private function declaration
	*/



/**
	* variable (private and public) definition
	*/



/**
	* function (private and public) definition
	*/

/**
	* dummy initialization
	*/
void spim_conf_init(spim_conf_t *spim_conf)
{
	memset(spim_conf, 0, sizeof(spim_conf_t));
}

/**
	* dummy open spi device
	*/
spim_t *spim_open(const spim_conf_t *spim_conf)
{
	spim_t *p;

	int retval;

	(void) spim_conf;

	/* alloc */
	p = (spim_t *) malloc(sizeof(spim_t));
	if (p == NULL)
	{
		return NULL;
	}

	memset(p, 0, sizeof(spim_t));

	/**
		* buffer_tx and buffer_rx set to NULL,
		* buffer will be allocated in spim_transfer
		*/

	/* before return p, initialize flash simulation */
	retval = spi_flash_simulation_init();
	if (retval != 0)
	{
		/* error */
		fprintf(stderr, "failed to initialize spi NOR flash simulation in spi init: %d\n", retval);

		/* free allocated pointer p */
		free(p);
		p = NULL; /* prevent usage of deallocated pointer */
	}

	return p;
}

/* close spi device */
void spim_close(spim_t *spim)
{
	/**
		* check if spim pointer is NULL,
		* by standard, free can handle NULL pointer
		* but to prevent valgrind get mad at us, check
		* before free is called
		*/
	if (spim != NULL)
	{
		/* free buffer first */
		if (spim->buffer_tx != NULL)
		{
			/* buffer tx and rx should be allocated simultaneusly */
			assert(spim->buffer_rx != NULL);

			free(spim->buffer_tx);
			free(spim->buffer_rx);

			/* prevent usage of deallocating pointer */
			spim->buffer_tx = NULL;
			spim->buffer_rx = NULL;
		}

		free(spim);

		/**
			* do not set spim to NULL here,
			* it is user responsibility to make sure
			* deallocated pointer is not used without reallocation
			*/
	}

	/* deinitialize spi flash simulation */
	spi_flash_simulation_fini(); /* fini can be called multiple times */
}

/* simulation */
void spim_transfer(spim_t * restrict spim,
	const void * restrict tx_data,
	void * restrict rx_data, size_t bit_len, enum spim_cs_e mode)
{
	/*bool first_transfer = false;*/
	bool free_buffer = false;

	size_t byte_len = bit_len / 8;

	int retval;

	/* check if input is sane */
	if ((spim == NULL) || ((tx_data == NULL) && (rx_data == NULL)) || (byte_len < 1))
	{
		return;
	}

	/* check if this is first transfer */
	if (spim->buffer_tx == NULL)
	{
		assert(spim->buffer_rx == NULL);

		/* set flag first transfer */
		/*first_transfer = true;*/

		/* alloc buffer in first transfer */
		spim->buffer_tx = (uint8_t *) malloc(byte_len * sizeof(uint8_t));
		if (spim->buffer_tx == NULL)
		{
			/* fail to allocate buffer */

			/**
				* panic if cs is SPIM_CS_KEEP, because user expect
				* continuity of previous session
				*/
			if (mode == SPIM_CS_KEEP)
			{
				fprintf(stderr, "PANIC: allocate spim->buffer_tx to %lu bytes " \
					"for continous session (this is first session)\n",
					(unsigned long) byte_len);
				abort();

				/* we should never reach here */
			}

			return;
		}

		spim->buffer_rx = (uint8_t *) malloc(byte_len * sizeof(uint8_t));
		if (spim->buffer_rx == NULL)
		{
			free(spim->buffer_tx);
			spim->buffer_tx = 0;

			/**
				* panic if cs is SPIM_CS_KEEP, because user expect
				* continuity of previous session
				*/
			if (mode == SPIM_CS_KEEP)
			{
				fprintf(stderr, "PANIC: allocate spim->buffer_rx to %lu bytes " \
					"for continous session (this is first session)\n",
					(unsigned long) byte_len);
				abort();

				/* we should never reach here */
			}

			return;
		}

		/* set buffer_len */
		spim->buffer_len = byte_len;
	}
	else
	{
		/* this is not a first fransfer */
		/*first_transfer = false;*/

		/* check if we need realloc */
		if (spim->buffer_len < byte_len)
		{
			/* realloc */
			uint8_t *p;

			/**
				* realloc TX
				*/
			p = (uint8_t *) realloc(spim->buffer_tx, byte_len);
			if (p == NULL)
			{
				/**
					* failed to realloc
					* this failure is fatal
					*/
				fprintf(stderr, "PANIC: cannot realloc spim->buffer_tx to %lu bytes " \
					"(from %lu bytes)\n", (unsigned long) byte_len, (unsigned long) spim->buffer_len);
				abort();

				/* we should not reach here */
			}

			/* set new buffer */
			spim->buffer_tx = p;

			/**
				* RX
				*/
			p = (uint8_t *) realloc(spim->buffer_rx, byte_len);
			if (p == NULL)
			{
				/* failed to realloc */
				free(spim->buffer_tx);

				fprintf(stderr, "PANIC: cannot realloc spim->buffer_tx to %lu bytes " \
					"(from %lu bytes)\n", (unsigned long) byte_len, (unsigned long) spim->buffer_len);
				abort();

				/* we should not reach here */
			}

			/* set new buffer */
			spim->buffer_rx = p;

			/* update buffer length */
			spim->buffer_len = byte_len;
		}
	}

	/* copy tx buffer */
	if (tx_data == NULL)
	{
		/* zero fill buffer */
		memset(spim->buffer_tx, 0, byte_len * sizeof(uint8_t));
	}
	else
	{
		/* copy tx data if tx_data is not NULL */
		memcpy(spim->buffer_tx, tx_data, byte_len * sizeof(uint8_t));
	}


	/* call spim_command */
	retval = spi_flash_trx_session(spim->buffer_tx, spim->buffer_rx, byte_len, mode == SPIM_CS_AUTO);

	/* cannot handle error session */
	if (retval != 0)
	{
		fprintf(stderr, "PANIC: spi_flash_trx_session error: %d\n", retval);
		abort();
	}



	switch (mode)
	{
	case (SPIM_CS_AUTO):
		free_buffer = true;
		break;

	case (SPIM_CS_KEEP):
		free_buffer = false;
		break;

	case (SPIM_CS_NONE):
		/* cannot handle this case, panic, call abort */
		fputs("PANIC: can\'t handle spim_transfer cs value SPIM_CS_NONE\n", stderr);
	default: /* compiler will complain "warning: this statement may fall through", but it is intended behavior; just ignore this warning */
		/* we should not reach here */
		abort();
	}

	/* should we handle buffer free */
	if (free_buffer)
	{
		if (spim->buffer_tx != NULL)
		{
			assert(spim->buffer_rx != NULL);

			/* free buffer */
			free(spim->buffer_tx);
			free(spim->buffer_rx);
			spim->buffer_tx = NULL;
			spim->buffer_rx = NULL;
		}
	}
}
