# Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT.

PULP_APP = spi_flash_writer
PULP_APP_C_SRCS = spi_flash_writer.c debugger_writer.c

PULP_CFLAGS = -O3 -g -Wall -Wextra -Wconversion -pedantic # -std=c11

# check if we in simulation
ifeq ($(SIMULATION),1)

# simulation omit PULP_ prefix from environment variable

MAKEFILE_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# application maybe already defined in parent makefile
#APP ?= $(patsubst %, $(MAKEFILE_DIR)/%, $(PULP_APP))
APP ?= $(patsubst %, $(MAKEFILE_DIR)%, $(PULP_APP))

# THIS DEFINITION IS NEEDED BY SIMULATION MAKEFILE TO GET FLASH_FILE PATH
APP_DIR = $(MAKEFILE_DIR)

# source code
#C_SRCS += $(patsubst %, $(MAKEFILE_DIR)/%, $(PULP_APP_C_SRCS))
C_SRCS += $(patsubst %, $(MAKEFILE_DIR)%, $(PULP_APP_C_SRCS))

# add definition for LINE_END to "\n" because we run in terminal
# and terminal use '\n' as line end
#CFLAGS += $(PULP_CFLAGS) -DSIMULATION=1 -DLINE_END=\"\\n\"
CFLAGS += -DSIMULATION=1 -DLINE_END=\"\\n\"

else

# add more stack in pulpissimo implementation
PULP_APP_C_SRCS += more_stack.c

# if we don't run simulation, link and use actual hardware
include $(PULPRT_HOME)/rules/pulp.mk

endif
