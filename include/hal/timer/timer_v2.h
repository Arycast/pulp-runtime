/*
 * Copyright (C) 2018 ETH Zurich, University of Bologna
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

#ifndef __HAL_TIMER_TIMER_V2_H__
#define __HAL_TIMER_TIMER_V2_H__

#include <stdbool.h>
#include <stdint.h>

#include "hal/pulp.h"
#include "archi/timer/timer_v2.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * hardware provide 2 (apb_)timer
  * timer lo and timer hi
  * both timer can be cascaded to count to 64-bit value
  */
enum timer_id_e
{
  TIMER_LO,
  TIMER_HI
};

typedef    enum timer_id_e    timer_id_t;


#ifdef ARCHI_HAS_FC

static inline unsigned int timer_base_fc(unsigned int id, unsigned int sub_id)
{
  return ARCHI_FC_TIMER_ADDR + id * ARCHI_FC_TIMER_SIZE + sub_id * 4;
}

#else

#define timer_base_fc(id,sub_id) timer_base_cl(0,id,sub_id)

#endif


#ifdef ARCHI_HAS_CLUSTER

static inline unsigned int timer_base_cl(unsigned int cid, unsigned int id, unsigned int sub_id)
{
  return ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(cid) + ARCHI_TIMER_OFFSET + id * ARCHI_TIMER_SIZE + sub_id * 4;
}

#else

static inline unsigned int timer_base_cl(unsigned int cid, unsigned int id, unsigned int sub_id)
{
  (void)     cid;
  (void)      id;
  (void)  sub_id;
  return 0;
}


#endif


static inline unsigned int timer_conf_get(unsigned int addr, timer_id_t id)
{
  switch (id)
  {
  case (TIMER_LO):
    /**
      * use cfg get from archi
      * (multiple return is forbidden by MISRA-C?)
      */
    return timer_cfg_lo_get(addr);
    break;
  case (TIMER_HI):
    return timer_cfg_hi_get(addr);
    break;
  }

  /**
    * if @arg id is set properly, this line should never be reached
    * (this break MISRA-C?)
    */
  return 0;
}


static inline void timer_conf_set(unsigned int addr, timer_id_t id, unsigned int value)
{
  switch (id)
  {
  case (TIMER_LO):
    timer_cfg_lo_set(addr, value);
    break;
  case (TIMER_HI):
    timer_cfg_hi_set(addr, value);
    break;

  /* do nothing for invalid id */
  }
}


static inline unsigned int timer_count_get(unsigned int addr, timer_id_t id)
{
  switch (id)
  {
  case (TIMER_LO):
    return timer_cnt_lo_get(addr);
    break;
  case (TIMER_HI):
    return timer_cnt_hi_get(addr);
    break;
  }

  /* invalid case */
  return 0;
}


static inline void timer_count_set(unsigned int addr, timer_id_t id, unsigned int value)
{
  switch (id)
  {
  case (TIMER_LO):
    timer_cnt_lo_set(addr, value);
    break;
  case (TIMER_HI):
    timer_cnt_hi_set(addr, value);
    break;
  }
}


static inline unsigned int timer_cmp_get(unsigned int addr, timer_id_t id)
{
  switch (id)
  {
  case (TIMER_LO):
    return timer_cmp_lo_get(addr);
    break;
  case (TIMER_HI):
    return timer_cmp_hi_get(addr);
    break;
  }

  return 0;
}


static inline void timer_cmp_set(unsigned int addr, timer_id_t id, unsigned int cmp)
{
  switch (id)
  {
  case (TIMER_LO):
    timer_cmp_lo_set(addr, cmp);
    break;
  case (TIMER_HI):
    timer_cmp_hi_set(addr, cmp);
    break;
  }
}


static inline void timer_reset(unsigned int addr, timer_id_t id)
{
  /**
    * reset can be done in 2 ways:
    * 1. by setting bit 1 in register CFG (cleared by hardware)
    * 2. by setting bit 0 in register RESET
    *
    * both will trigger reset signal for prescaler counter and timer counter
    * and set both counter to 0 again
    *
    * it doesn't change any configuration, just counter value (set back to 0)
    */

  /* choose second implementation */
  switch (id)
  {
  case (TIMER_LO):
    timer_reset_lo_set(addr, 1);
    break;
  case (TIMER_HI):
    timer_reset_hi_set(addr, 1);
    break;
  }
}


static inline void timer_start(unsigned int addr, timer_id_t id)
{
  /**
    * same with reset, there are 2 ways to start timer:
    * 1. with setting bit 0 in CFG register (EN)
    * 2. with setting bit 0 in START register
    */
  /* choose second implementation */
  switch (id)
  {
  case (TIMER_LO):
    timer_start_lo_set(addr, 1);
    break;
  case (TIMER_HI):
    timer_start_hi_set(addr, 1);
    break;
  }
}



/**
  * add more fine-grained control of timer
  */

/**
  * stop timer (without resetting current configuration)
  */
static inline void timer_stop(unsigned int addr, timer_id_t id)
{
  unsigned int value = timer_conf_get(addr, id);

  /* both timer lo and hi have same bit position for enable bit (bit 0) */
  timer_conf_set(addr, id, value & ~((unsigned int) 0x1));
}

void timer_set_clock_source(unsigned int addr, timer_id_t id, bool is_use_32khz);

void timer_set_prescaler(unsigned int addr, timer_id_t id, bool is_enable, uint8_t prescaler_value);

void timer_set_continuity(unsigned int addr, timer_id_t id, bool is_oneshot, bool is_reset_after_compare_match);

void timer_enable_irq(unsigned int addr, timer_id_t id);

void timer_disable_irq(unsigned int addr, timer_id_t id);


static inline    uint32_t   timer_pause(unsigned int addr, timer_id_t id)
{
  uint32_t state = 0;
  uint32_t irq_state;
  switch (id)
  {
  case (TIMER_LO):
    /* disable interrupt */
    __asm__ __volatile__ (   "csrrci     %0, mstatus, 0x8\n" : "=r" (irq_state) : : "memory");

    state = timer_cfg_lo_get(addr);
    timer_cfg_lo_set(addr, state & ~((uint32_t) 0x1));

    /* re-enable interrupt */
    if (irq_state & ((uint32_t) 0x8))
    {
      __asm__ __volatile__ ( "csrs           mstatus, 0x8\n" :                  : : "memory");
    }
    break;

  case (TIMER_HI):
    /* disable interrupt */
    __asm__ __volatile__ (   "csrrci     %0, mstatus, 0x8\n" : "=r" (irq_state) : : "memory");

    state = timer_cfg_hi_get(addr);
    timer_cfg_hi_set(addr, state & ~((uint32_t) 0x1));

    /* re-enable interrupt */
    if (irq_state & ((uint32_t) 0x8))
    {
      __asm__ __volatile__ ( "csrs           mstatus, 0x8\n" :                  : : "memory");
    }
    break;

  default:
    /* error if timer_id is not 0 or 1 */
    while (1)
    {
    }
  }

  return state & 0x1;
}

static inline    void       timer_continue(unsigned int addr, timer_id_t id, uint32_t state)
{
  if (state & ((uint32_t) 0x1)) /* only continue if EN bit in state is set */
  {
    uint32_t irq_state;
    switch (id)
    {
    case (TIMER_LO):
      /* disable interrupt */
      __asm__ __volatile__ (   "csrrci     %0, mstatus, 0x8\n" : "=r" (irq_state) : : "memory");

      state = timer_cfg_lo_get(addr);
      timer_cfg_lo_set(addr, state | ((uint32_t) 0x1));

      /* re-enable interrupt */
      if (irq_state & ((uint32_t) 0x8))
      {
        __asm__ __volatile__ ( "csrs           mstatus, 0x8\n" :                  : : "memory");
      }
      break;

    case (TIMER_HI):
      /* disable interrupt */
      __asm__ __volatile__ (   "csrrci     %0, mstatus, 0x8\n" : "=r" (irq_state) : : "memory");

      state = timer_cfg_hi_get(addr);
      timer_cfg_hi_set(addr, state | ((uint32_t) 0x1));

      /* re-enable interrupt */
      if (irq_state & ((uint32_t) 0x8))
      {
        __asm__ __volatile__ ( "csrs           mstatus, 0x8\n" :                  : : "memory");
      }
      break;

    default:
      /* error if timer_id is not 0 or 1 */
      while (1)
      {
      }
    }
  }
}

#ifdef __cplusplus
}
#endif

#endif
