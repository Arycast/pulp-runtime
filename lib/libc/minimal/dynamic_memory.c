/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/**
	* wrapper for malloc-related function
	*/
/*#include <stdint.h>
#include <stddef.h>*/
#include <string.h>

#include "dynamic_memory.h"

#include "pulp.h"


static void * aligned_malloc(size_t size);
static void   aligned_free(void *ptr, size_t size);


void * malloc(size_t size)
{
	/* call aligned alloc */
	void      *ptr          = aligned_malloc(sizeof(size_t) + size);
	size_t    *ptr_size_t   = (size_t *) ptr;
	if (ptr == NULL) /* check if malloc success */
	{
		return NULL; /* just return NULL if malloc not success */
	}

	/* record chuck size */
	*ptr_size_t++ = size;

	return (void *) ptr_size_t;
}

void   free(void *ptr)
{
	size_t *ptr_size_t = ptr; /* convert ptr to size_t (maybe unalign) */

	/* check if ptr is NULL */
	if (ptr == NULL)
	{
		/* just return if ptr is NULL */
		return;
	}

	/* move pointer backward */
	ptr_size_t--;

	/* free */
	aligned_free(ptr_size_t, *ptr_size_t);
}

void * calloc(size_t nmemb, size_t size)
{
	/* call aligned alloc */
	size_t     allocated_memory = nmemb * size;

	void      *ptr          = malloc(allocated_memory);

	/* only do memset if malloc success and size is not 0 */
	if ((ptr != NULL) && (allocated_memory != 0))
	{
		memset(ptr, 0, allocated_memory);
	}

	return (void *) ptr;
}

void * realloc(void *ptr, size_t size)
{
	size_t   *ptr_size_t;
	void     *ptr2;

	if (size == 0)
	{
		free(ptr); /* free is NULL-tolerable */
		return NULL;
	}

	/* size is not 0 */

	/* allocate new memory even if requested size is smaller than current size */
	ptr2 = malloc(size);
	if (ptr2 == NULL)
	{
		return NULL; /* return NULL without touching old memory */
	}
	else if (ptr == NULL)
	{
		return ptr2; /* in case ptr is NULL, just return ptr2 */
	}

	/* in case allocation success, before freeing old memory, copy data first */
	ptr_size_t = (size_t *) ptr;

	/* move backward to get buffer size */
	ptr_size_t--;

	/* copy data */
	memcpy(ptr2, ptr, ((*ptr_size_t) > size) ? size : (*ptr_size_t));

	/* return new ptr2 */
	return ptr2;
}

/*void * reallocarray(void *ptr, size_t nmemb, size_t size)
{
}*/

static void * aligned_malloc(size_t size)
{
	return pi_l2_malloc((int) size);
}

static void   aligned_free(void *ptr, size_t size)
{
	pi_l2_free(ptr, (int) size);
}
