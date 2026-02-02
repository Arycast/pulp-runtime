ifeq '$(CONFIG_CRT0)' '1'
PULP_ASM_SRCS += kernel/crt0.S
endif

ifeq '$(CONFIG_LIBC_MINIMAL)' '1'
PULP_C_SRCS += lib/libc/minimal/io.c lib/libc/minimal/fprintf.c lib/libc/minimal/prf.c lib/libc/minimal/sprintf.c lib/libc/minimal/dynamic_memory.c lib/libc/minimal/newlib_reentrant.c
endif

PULP_C_SRCS += kernel/init.c kernel/kernel.c kernel/alloc.c kernel/alloc_pool.c kernel/irq.c kernel/soc_event.c kernel/tick.c kernel/delay.c kernel/bench.c drivers/uart.c drivers/i2c.c drivers/spi.c drivers/timer.c drivers/adv_timer.c

PULP_ASM_SRCS += kernel/irq_asm.S kernel/tick32.S


ifneq '$(cluster/version)' ''
PULP_C_SRCS += kernel/cluster.c
endif
