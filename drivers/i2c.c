#include <stdint.h>
#include <string.h>
#include "pulp.h"

#if ((ARCHI_UDMA_NB_I2C) > 0)
static L2_DATA i2c_t __i2c[ARCHI_UDMA_NB_I2C];
#else
/*
  * at-least allocate 1 element
  *   FIXME: we should not allocated anything if hardware is not exist
  *          to reserve memory
  */
static L2_DATA i2c_t __i2c[1];
#endif

static volatile uint32_t i2c_timeout = 0ul;
static volatile bool i2c_timed_out_flag = false;
static volatile bool i2c_reset_on_timeout = false;

// Get local array index from peripheral ID
static inline int __i2c_id(int periph_id)
{
    return periph_id - ARCHI_UDMA_I2C_ID(0);
}

// Compute divider from desired I2C frequency
int i2c_get_div(int i2c_freq)
{
    int div = (pos_freq_domains[PI_FREQ_DOMAIN_PERIPH] + i2c_freq - 1) / i2c_freq;

    if (div & 1) div += 1;
    div >>= 1;

    return div;
}

i2c_t *i2c_open(i2c_dev_t *dev)
{

    if (dev == NULL) {
        return NULL;
    }

    if (dev->id < 0 || dev->id >= ARCHI_UDMA_NB_I2C) {
        return NULL;
    }

    int periph_id = ARCHI_UDMA_I2C_ID(dev->id); // 2

    i2c_t *i2c =  &__i2c[__i2c_id(periph_id)];

    i2c->periph_id = periph_id * 2;
    i2c->id = dev->id;
    i2c->cs = dev->cs;
    i2c->max_baudrate = dev->max_baudrate;
    i2c->div = i2c_get_div(dev->max_baudrate);

    plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));
    soc_eu_fcEventMask_setEvent(periph_id);
    soc_eu_fcEventMask_setEvent(periph_id + 1);

    return i2c;
}

void i2c_close(i2c_t *i2c)
{
    if (i2c != NULL) {
        plp_udma_cg_set(plp_udma_cg_get() & ~(1<<i2c->periph_id));
    }
}

void i2c_settimeout(uint32_t timeout, bool reset_on_timeout){
    i2c_timed_out_flag = false;
    i2c_timeout = timeout;
    i2c_reset_on_timeout = reset_on_timeout;
}

int i2c_write(i2c_t *dev, unsigned char *data, int length, int send_stop)
{
    unsigned char udma_cmd[I2C_CMD_BUFFER_SIZE];
    int seq_index = 0;
    int i;

    udma_cmd[seq_index++] = I2C_CMD_CFG;
    udma_cmd[seq_index++] = (dev->div >> 8) & 0xFF;
    udma_cmd[seq_index++] = (dev->div & 0xFF);

    udma_cmd[seq_index++] = I2C_CMD_START;
    udma_cmd[seq_index++] = I2C_CMD_WR;
    udma_cmd[seq_index++] = dev->cs;

    if (length > 1) {
        udma_cmd[seq_index++] = I2C_CMD_RPT;
        udma_cmd[seq_index++] = length;
    }

    udma_cmd[seq_index++] = I2C_CMD_WR;
    for (i = 0; i<length; i++) {
        udma_cmd[seq_index++] = data[i];
    }

    if (send_stop) {
        udma_cmd[seq_index++] = I2C_CMD_STOP;
    }    

    plp_udma_enqueue(UDMA_I2C_TX_ADDR(dev->id), (unsigned int)udma_cmd, seq_index, UDMA_CHANNEL_CFG_EN); 

    uint32_t startMicros = micros();
    while (plp_udma_busy(UDMA_I2C_TX_ADDR(dev->id))){
        if ((i2c_timeout > 0) && ((micros() - startMicros) > i2c_timeout)) {
            
            // Stop and clear the on-going transfer
            plp_i2c_tx_clear(dev->id);

            if (i2c_reset_on_timeout) {
                int actual_periph_id = ARCHI_UDMA_I2C_ID(dev->id);

                // Turn off UDMA peripherals clock gate
                plp_udma_cg_set(plp_udma_cg_get() & ~(1 << actual_periph_id));
                
                for(volatile int j=0; j<10; j++); 
                
                // Turn on UDMA peripheral clock gate
                plp_udma_cg_set(plp_udma_cg_get() | (1 << actual_periph_id));
            }
            i2c_timed_out_flag = true;
            return 5; // 5 is the standar error Arduino code for timeout in I2C write
        }
    }

    return 0; // Success
}

int i2c_read(i2c_t *dev, unsigned char *rx_buff, int length, int pending)
{
    unsigned char udma_cmd[I2C_CMD_BUFFER_SIZE];
    int seq_index = 0;

    udma_cmd[seq_index++] = I2C_CMD_CFG;
    udma_cmd[seq_index++] = (dev->div >> 8) & 0xFF;
    udma_cmd[seq_index++] = (dev->div & 0xFF);

    udma_cmd[seq_index++] = I2C_CMD_START;
    udma_cmd[seq_index++] = I2C_CMD_WR;
    udma_cmd[seq_index++] = dev->cs | 0x1; // READ

    if (length > 1)
    {
        udma_cmd[seq_index++] = I2C_CMD_RPT;
        udma_cmd[seq_index++] = length - 1;
        udma_cmd[seq_index++] = I2C_CMD_RD_ACK;
    }

    udma_cmd[seq_index++] = I2C_CMD_RD_NACK;

    if (!pending)
        udma_cmd[seq_index++] = I2C_CMD_STOP;

    plp_udma_enqueue(UDMA_I2C_DATA_ADDR(dev->id), (unsigned int)rx_buff, length, UDMA_CHANNEL_CFG_EN);
    plp_udma_enqueue(UDMA_I2C_TX_ADDR(dev->id), (unsigned int)udma_cmd, seq_index, UDMA_CHANNEL_CFG_EN);

    uint32_t startMicros = micros();
    while (plp_udma_busy(UDMA_I2C_DATA_ADDR(dev->id))){
        if ((i2c_timeout > 0) && ((micros() - startMicros) > i2c_timeout)) {
            // Stop and clear the on-going transfer
            plp_i2c_rx_clear(dev->id);
            plp_i2c_tx_clear(dev->id);

            if (i2c_reset_on_timeout) {
                int actual_periph_id = ARCHI_UDMA_I2C_ID(dev->id);

                // Turn off UDMA peripherals clock gate
                plp_udma_cg_set(plp_udma_cg_get() & ~(1 << actual_periph_id));
                
                for(volatile int j=0; j<10; j++); 

                // Turn on UDMA peripheral clock gate
                plp_udma_cg_set(plp_udma_cg_get() | (1 << actual_periph_id));
            }
            i2c_timed_out_flag = true;
            return 0; 
        }
    }
    while (plp_udma_busy(UDMA_I2C_TX_ADDR(dev->id)));
    return length;
}

void i2c_dev_init(i2c_dev_t *dev)
{
  dev->cs = -1;
  dev->id = -1;
  dev->max_baudrate = 200000;
}

bool i2c_managetimeoutflag(bool clearflag)
{
    bool flag = i2c_timed_out_flag;
    if (clearflag){
        flag = false;
    }
    return(flag);
}