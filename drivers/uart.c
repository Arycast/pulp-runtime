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

#include "pulp.h"



#define UART_BAUDRATE 115200

#define RX_BUFFER_SIZE 128
#define User_Area_Limit 64
#define uDMA_Area_Size 64
#define uDMA_Start_idx 64 

static L2_DATA volatile uint8_t RX_BUFFER[RX_BUFFER_SIZE] __attribute__((aligned(4)));

static volatile uint32_t user_head = -1;
static volatile uint32_t user_tail = 0;
static uint32_t last_hw_count = 0;
static uint32_t current_hw_count = 0;


static void uart_wait_tx_done(int periph)
{
  while (plp_udma_busy(UDMA_UART_TX_ADDR(periph - ARCHI_UDMA_UART_ID(0))))
  {
  }

  // while(plp_uart_tx_busy(periph - ARCHI_UDMA_UART_ID(0)));
}


static void uart_wait_rx_done(int periph)
{
  while (plp_udma_busy(UDMA_UART_RX_ADDR(periph - ARCHI_UDMA_UART_ID(0))))
  {
  }
  // while(plp_uart_rx_busy(periph - ARCHI_UDMA_UART_ID(0)));
}



static void uart_setup(int channel, int baudrate)
{
  int div =  (pos_freq_domains[PI_FREQ_DOMAIN_PERIPH] + baudrate/2) / baudrate;

  plp_uart_setup(channel - ARCHI_UDMA_UART_ID(0), 0, div-1);
}



int uart_open(int uart_id, int baudrate)
{
  int periph_id = ARCHI_UDMA_UART_ID(uart_id);
  int channel = UDMA_EVENT_ID(periph_id);

  unsigned int rx_base = hal_udma_channel_base(channel);
  
  pulp_write32(rx_base + UDMA_CHANNEL_CFG_OFFSET, (1 << 6));

  user_head = -1;
  user_tail = 0;
  last_hw_count = 0;

  for (int i = 0; i<RX_BUFFER_SIZE; i++) RX_BUFFER[i] = 0;

  plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));

  soc_eu_fcEventMask_setEvent(channel);
  soc_eu_fcEventMask_setEvent(channel+1);

  uart_setup(periph_id, baudrate);

  unsigned int base = hal_udma_channel_base(channel);

  plp_udma_enqueue(base, (int)&RX_BUFFER[uDMA_Start_idx], uDMA_Area_Size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_CONT);

  return 0;
}



int uart_available(int uart_id)
{
  int periph_id = ARCHI_UDMA_UART_ID(uart_id);
  int channel = UDMA_EVENT_ID(periph_id);

  unsigned int base = hal_udma_channel_base(channel);

  uint32_t remaining = pulp_read32(base + UDMA_CHANNEL_SIZE_OFFSET);
  current_hw_count = uDMA_Area_Size - remaining;

  if (current_hw_count != last_hw_count) {
    int count = (current_hw_count - last_hw_count + uDMA_Area_Size) % uDMA_Area_Size;
    int remaining_user_area = (User_Area_Limit - 1) - user_head;

    if (count > remaining_user_area) {
      int shift = count - remaining_user_area;

      if (user_head >= 0) {
        int bytes_to_move = (user_head - shift + 1);

        if(bytes_to_move > 0) {
          for (int i = 0; i < bytes_to_move; i++) {
            RX_BUFFER[i] = RX_BUFFER[i + shift];
          }
          user_head -= shift;
        } else {
          user_head = -1;
        }

        if (user_tail >= shift) {
          user_tail -= shift;
        } else {
          user_tail = 0;
        }
      }
    }

    int write_position = user_head + 1;
    for (int i = 0; i < count; i++) {
      uint32_t udma_idx = uDMA_Start_idx + ((last_hw_count + i) % uDMA_Area_Size);
      if (write_position + i < User_Area_Limit) {
        RX_BUFFER[write_position + i] = RX_BUFFER[udma_idx];
        // RX_BUFFER[udma_idx] = 0;
      }
    }

    user_head += count;
    if (user_head >= User_Area_Limit) user_head = User_Area_Limit - 1;

    last_hw_count = current_hw_count;
  }

  if (user_head < 0) return 0;
  return (user_head - user_tail + 1);
}



int uart_peek(int uart_id)
{
  if (uart_available(uart_id) == 0) return -1;
  return RX_BUFFER[user_tail];
}

int uart_read(int uart_id)
{
  if (uart_available(uart_id) <= 0) return -1;

  uint8_t data = RX_BUFFER[user_tail];
  RX_BUFFER[user_tail] = 0;

  if (user_tail < user_head) {
    user_tail++;
  } else {
    user_head = -1;
    user_tail = 0;
  }
  return (int)data;
}



void uart_close(int uart_id)
{
  int periph_id = ARCHI_UDMA_UART_ID(uart_id);
  int channel = UDMA_EVENT_ID(periph_id);
  unsigned int rx_base = hal_udma_channel_base(channel);

  pulp_write32(rx_base + UDMA_CHANNEL_CFG_OFFSET, UDMA_CHANNEL_CFG_CLEAR);

  uart_wait_tx_done(periph_id);

  plp_uart_disable(periph_id);      

  plp_udma_cg_set(plp_udma_cg_get() & ~(1<<periph_id));
}



int uart_write(int uart_id, const void *buffer, uint32_t size)
{
  int periph_id = ARCHI_UDMA_UART_ID(uart_id);
  int channel = UDMA_EVENT_ID(periph_id) + 1;

  unsigned int base = hal_udma_channel_base(channel);

  plp_udma_enqueue(base, (int)buffer, size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
  
  uart_wait_tx_done(periph_id);

  return 0;
}
