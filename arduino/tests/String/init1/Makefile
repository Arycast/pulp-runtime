# Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT.

# arduino dir
ARDUINO_BASEDIR = ../../..

# cores
ARDUINO_CORES = $(ARDUINO_BASEDIR)/cores

# source
ASM_SRCS =
C_SRCS =
CXX_SRCS = main.cpp $(ARDUINO_CORES)/String.cpp

# object
ASM_OBJS = $(filter %.o,$(patsubst %.S,%.S.o,$(ASM_SRCS))) $(filter %.o,$(patsubst %.s,%.s.o,$(ASM_SRCS)))
C_OBJS = $(patsubst %.c,%.c.o,$(C_SRCS))
#CXX_OBJS = $(filter %.o,$(patsubst %.C,%.C.o,$(CXX_SRCS))) $(filter %.o,$(patsubst %.cc,%.cc.o,$(CXX_SRCS))) $(filter %.o,$(patsubst %.cpp,%.cpp.o,$(CXX_SRCS))) $(filter %.o,$(patsubst %.cxx,%.cxx.o,$(CXX_SRCS)))
CXX_OBJS = $(filter %.o,$(patsubst %.cc,%.cc.o,$(CXX_SRCS))) $(filter %.o,$(patsubst %.cpp,%.cpp.o,$(CXX_SRCS))) $(filter %.o,$(patsubst %.cxx,%.cxx.o,$(CXX_SRCS)))

# application to build
APP = main

# compilation flags
ASMFLAGS = -ggdb -Wall -Wextra -pedantic -flto -I$(ARDUINO_CORES) -I.
CFLAGS = -ggdb -Wall -Wextra -pedantic -Wconversion -std=c11 -flto -I$(ARDUINO_CORES) -I.
CXXFLAGS = -ggdb -Wall -Wextra -pedantic -Wconversion -Wnoexcept -std=c++11 -flto -I$(ARDUINO_CORES) -I.

HAS_C = $(C_SRCS)
HAS_CXX = $(CXX_SRCS)

# need to explicitly choose which program used for linking
LD_AS = $(CC)
LD_C = $(CC)
LD_CXX = $(CXX)

ASM_LDFLAGS =
C_LDFLAGS =
CXX_LDFLAGS =

ASM_LIBS = -Wl,-flto
C_LIBS = -Wl,-flto
CXX_LIBS = -Wl,-flto

# rules that don't need output
.PHONY: all clean distclean

all: $(APP)

# linking
$(APP): $(ASM_OBJS) $(C_OBJS) $(CXX_OBJS)
ifeq ($(HAS_CXX),)
ifeq ($(HAS_C),)
		$(LD_AS) $(ASM_LDFLAGS) -o $@ $^ $(ASM_LIBS)
else
		$(LD_C) $(C_LDFLAGS) -o $@ $^ $(C_LIBS)
endif
else
		$(LD_CXX) $(CXX_LDFLAGS) -o $@ $^ $(CXX_LIBS)
endif

# compile

# ASM
%.S.o : %.S
	$(CC) $(ASMFLAGS) -c -o $@ $<

%.s.o : %.s
	$(CC) $(ASMFLAGS) -c -o $@ $<

# C
%.c.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# C++
%.cc.o : %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.cpp.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.cxx.o : %.cxx
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# clean and distclean
clean:
	$(RM) --verbose $(ASM_OBJS) $(C_OBJS) $(CXX_OBJS)

distclean: clean
	$(RM) --verbose $(APP)
