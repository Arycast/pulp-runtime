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


#include <sys/reent.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>

#include "pulp.h"

#include "io.h"
#include "dynamic_memory.h"


typedef void (*fptr)(void);

static fptr ctor_list[1] __attribute__((section(".ctors.start"))) = { (fptr) -1 };
static fptr dtor_list[1] __attribute__((section(".dtors.start"))) = { (fptr) -1 };

static void hal_cpp_linking_workaround(void);

static void pos_init_do_ctors(void)
{
    fptr *fpp;

    for(fpp = ctor_list+1;  *fpp != 0;  ++fpp)
    {
        (**fpp)();
    }
}



static void pos_init_do_dtors(void)
{
    fptr *fpp;
    for(fpp = dtor_list + 1;  *fpp != 0;  ++fpp)
    {
        (**fpp)();
    }
}


/*extern int main(int argc, const char * const argv[]);*/
extern int main(void);


void pos_init_start()
{
#if PULP_CHIP_FAMILY == CHIP_GAP
  // Always allow JTAG accesses for now as security is not implemented
  hal_pmu_bypass_set (ARCHI_REG_FIELD_SET (hal_pmu_bypass_get (), 1, 11, 1) );
#endif

  pos_soc_init();

  pos_irq_init();

  #ifdef ARCHI_NO_FC
  pos_soc_event_init();
  #endif

  // Initialize first the memory allocators and the utils so that they are
  // available for constructors, especially to let them declare
  // callbacks
  //__rt_utils_init();
  pos_allocs_init();

  // Call global and static constructors
  // Each module may do private initializations there
  pos_init_do_ctors();

  // Now that the system is ready, activate IO
  pos_io_start();

  // Now now the minimal init are done, we can activate interruptions
  hal_irq_enable();

  /**
    * somehow we will get linking error if instead of linking all object
    * we go with arduino flow which is create static library (.a) first
    * then linking, because in linking process, when main program don't use
    * alloc-related functions, those alloc function will be thrown away
    * from static library. but then c++ implementation (libstd++) will try
    * use it, end up with undefined reference
    */
  hal_cpp_linking_workaround();

  if (!hal_is_fc())
  {
      cluster_start(hal_cluster_id(), main);
  }

  // start tick
  pos_tick_init();
}


void pos_init_stop()
{
    // Close IO to flush them
    pos_io_stop();

    /* Call global and static destructors */
    pos_init_do_dtors();

    // stop tick
    pos_tick_stop();
}


/**
    * we need to call and use some symbol that is needed
    * by c++ standard library (libstdc++) to force linker to
    * not throw away those symbol, where are should be more
    * smart way to do this effectively (like by lw'ing with assembly)
    * but for now, just do following meaningless thing
    */
#if 0
static void hal_cpp_linking_workaround(void)
{
    /* allocate a pointer with realloc (which internally call malloc) */
    struct _reent **p1 = realloc(NULL, sizeof(struct _reent *));
    char           *p2;

    /**
        * check if alloc success, nothing to worry even if
        * allocation fail, just return and continue
        */
    if (p1 == NULL)
    {
        /* return if allocation fail */
        return;
    }

    /* set with _impure_ptr */
    *p1 = _impure_ptr;

    /* free it again */
    free(p1);


    p2 = realloc(NULL, 4 * sizeof(char));
    if (p2 == NULL)
    {
        return;
    }

    /* use sprintf */
    sprintf(p2, "%c", 'A');

    /**
        * use write (POSIX.1-2008), because we write dummy thing
        * set length/size to 0
        */
    write((int) stdout, p2, 0);

    /* free it again */
    free(p2);
}
#else
int a, b = 42;
static void hal_cpp_linking_workaround(void)
{
    void *p = NULL;

    const void * const p___impure_ptr = (const void *) _impure_ptr;
    const void * const p__malloc      = (const void *) malloc;
    const void * const p__realloc     = (const void *) realloc;
    const void * const p__free        = (const void *) free;
    const void * const p__sprintf     = (const void *) sprintf;
    const void * const p__write       = (const void *) write;

    __asm__ volatile (
        "mv %0, %1\n" \
        "mv %0, %2\n" \
        "mv %0, %3\n" \
        "mv %0, %4\n" \
        "mv %0, %5\n" \
        "mv %0, %6\n"
        : "=r" (p)           /* Output: %0 */
        : "r" (p___impure_ptr), /* Input:  %1, %2, %3, and so on */
            "r" (p__malloc),
            "r" (p__realloc),
            "r" (p__free),
            "r" (p__sprintf),
            "r" (p__write)
    );
}
#endif
