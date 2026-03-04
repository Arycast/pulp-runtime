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

#ifndef __RTIO_STDLIB_H__
#define __RTIO_STDLIB_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__ ((noreturn)) void exit(int status);

__attribute__ ((noreturn)) void abort(void);

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
/*void * reallocarray(void *ptr, size_t nmemb, size_t size);*/

void free(void *ptr);

/**
	* declare stdlib's random
	* for now, implementation rely on -lc in linking
	*/
long random(void);
void srandom(unsigned int seed);

long strtol(const char *nptr, char **endptr, int base);
double strtod(const char *nptr, char **endptr);

#ifdef __cplusplus
}
#endif

#endif
