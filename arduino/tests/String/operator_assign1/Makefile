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
APP = main.elf

# compilation flags
ASMFLAGS = -ggdb -Wall -Wextra -pedantic -flto -I$(ARDUINO_CORES) -I. -DSIMULATION_TEST
CFLAGS = -ggdb -Wall -Wextra -pedantic -Wconversion -std=c11 -flto -I$(ARDUINO_CORES) -I. -DSIMULATION_TEST
CXXFLAGS = -ggdb -Wall -Wextra -pedantic -Wconversion -Wnoexcept -std=c++11 -flto -I$(ARDUINO_CORES) -I. -DSIMULATION_TEST

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

# additional tools (instrumentation)
GDB = gdb
GDB_OPT =

VALGRIND = valgrind
VALGRIND_OPT = --verbose --read-inline-info=yes --read-var-info=yes --leak-check=full --show-leak-kinds=all --leak-check-heuristics=all --track-origins=yes

# massif
MASSIF_OUTPUT = $(patsubst %.elf,%.elf.massif.out,$(APP))
MASSIF_OPT =

# RM value default to rm -f is not part of POSIX standard,
# (it is actually specific behavior to GNU Make)
# define RM to maximize portability (add --verbose while do it)
RM ?= rm --verbose -f

# rules that don't need output
.PHONY: all clean distclean

# first parsed rule is the default rule (called by make without argument)
# put rule 'all' here as default rule
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

# massif output
%.elf.massif.out : %.elf
	sh -c 'cd "$$(dirname "$$1")" && $(VALGRIND) --tool=massif --massif-out-file="$$1" "./$$(basename "$$2")"' -- $@ $<

# clean and distclean
clean:
	$(RM) --verbose $(ASM_OBJS) $(C_OBJS) $(CXX_OBJS) $(MASSIF_OUTPUT)

distclean: clean
	$(RM) --verbose $(APP)

# instrumentation
.PHONY: run run-valgrind run-massif debug debug-valgrind-server debug-valgrind-client

run: $(APP)
	sh -c 'cd "$$(dirname "$$1")" && "./$$(basename "$$1")"' -- $<

run-valgrind: $(APP)
	sh -c 'cd "$$(dirname "$$1")" && $(VALGRIND) $(VALGRIND_OPT) "./$$(basename "$$1")"' -- $<

run-massif: $(MASSIF_OUTPUT)

debug: $(APP)
	$(GDB) $(GDB_OPT) $<

debug-valgrind: $(APP)
	sh -c 'cd "$$(dirname "$$1")" && $(VALGRIND) $(VALGRIND_OPT) --vgdb-error=1 --vgdb-stop-at=startup "./$$(basename "$$1")" & VALGRIND_PID="$$!" && sleep 2 && VGDB="$$(which "vgdb")" && $(GDB) $(GDB_OPT) -ex "target remote | $$VGDB --pid=$$VALGRIND_PID" "$$(basename "$$1")" && wait "$$VALGRIND_PID"' -- $<
