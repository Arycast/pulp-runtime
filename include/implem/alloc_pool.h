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

#ifndef __POS_IMPLEM_ALLOC_POOL_H__
#define __POS_IMPLEM_ALLOC_POOL_H__

#ifdef __cplusplus
extern "C"
{
#endif

void pos_allocs_init();

#if defined(ARCHI_HAS_L1)
void  alloc_init_l1(int cid);
void *pi_l1_malloc(int cid, int size);
void  pi_l1_free(int cid, void *chunk, int size);
#endif

void *pi_l2_malloc(int size);
void  pi_l2_free(void *_chunk, int size);

#if defined(ARCHI_HAS_FC_TCDM)
void *pi_fc_tcdm_malloc(int size);
void  pi_fc_tcdm_free(void *_chunk, int size);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
