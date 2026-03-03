/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RTIO_STRING_H__
#define __RTIO_STRING_H__

#include <stddef.h>
#include "compatibility.h"

/*#define NULL ((void *)0)*/

#ifdef __cplusplus
extern "C"
{
#endif

void *memset(void *s, int c, size_t n);

int memcmp(const void *m1, const void *m2, size_t n);
void *memcpy(void *dst0, const void *src0, size_t len0);

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);

char *strchr(const char *s, int c);

void *memmove(void *d, const void *s, size_t n);

char *strcpy(char *d, const char *s);
char *strcat(char *dest, const char *src);

size_t strlen(const char *str);
size_t strnlen(const char *s, size_t maxlen); /* provide declaration only,
	implementation is depend whether arduino is user or not; when not, link with -lc */

/**
	* provide declaration-only, if arduino used, then pgmspace implementation is used
	* when not, -lc is required
	*/
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

/* provide declaration only */
char * strtok_r(char * restrict str, const char * restrict delim,
	char ** restrict saveptr);

/* C++11 memchr */
void *memchr(const void *s, int c, size_t n);

#ifdef __cplusplus
}
#endif

#endif
