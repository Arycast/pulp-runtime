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

# check if we compile with coverage?
# to add coverage intrumentation, call make with environment variable: COVERAGE_TEST=1
# e.g.:
#
# $ COVERAGE_TEST=1 make all
#
# or
#
# $ export COVERAGE_TEST=1
# $ make all
#
# coverage test instrumentation won't be added if COVERAGE_TEST set to 0 or set to empty string
ifeq ($(COVERAGE_TEST),)
	# no coverage testing
	COVERAGE_ASM_FLAGS =
	COVERAGE_C_FLAGS =
	COVERAGE_CXX_FLAGS =

	COVERAGE_ASM_LDFLAGS =
	COVERAGE_C_LDFLAGS =
	COVERAGE_CXX_LDFLAGS =
else
ifeq ($(COVERAGE_TEST),0)
	# no coverage testing
	COVERAGE_ASM_FLAGS =
	COVERAGE_C_FLAGS =
	COVERAGE_CXX_FLAGS =

	COVERAGE_ASM_LDFLAGS =
	COVERAGE_C_LDFLAGS =
	COVERAGE_CXX_LDFLAGS =
else
	# compilation flags
	COVERAGE_ASM_FLAGS =
	COVERAGE_C_FLAGS = -coverage
	COVERAGE_CXX_FLAGS = -coverage

	# linking flags
	COVERAGE_ASM_LDFLAGS = -coverage
	COVERAGE_C_LDFLAGS = -coverage
	COVERAGE_CXX_LDFLAGS = -coverage
endif
endif

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
# respect compilation flags from environment variable (ASMFLAGS, CFLAGS, CXXFLAGS)
ASMFLAGS += -g -Wall -Wextra -pedantic -flto $(COVERAGE_ASM_FLAGS) -I$(ARDUINO_CORES) -I. -DSIMULATION_TEST
CFLAGS += -g -Wall -Wextra -pedantic -Wconversion -std=c11 -flto $(COVERAGE_C_FLAGS) -I$(ARDUINO_CORES) -I. -DSIMULATION_TEST
CXXFLAGS += -g -Wall -Wextra -pedantic -Wconversion -Wnoexcept -std=c++11 -flto $(COVERAGE_CXX_FLAGS) -I$(ARDUINO_CORES) -I. -DSIMULATION_TEST

HAS_C = $(C_SRCS)
HAS_CXX = $(CXX_SRCS)

# need to explicitly choose which program used for linking
LD_AS = $(CC)
LD_C = $(CC)
LD_CXX = $(CXX)

# respect general linking flags (LDFLAGS and LIBS) and
# linker-specific flags from environment variable
ASM_LDFLAGS += $(LDFLAGS) $(COVERAGE_ASM_LDFLAGS)
C_LDFLAGS += $(LDFLAGS) $(COVERAGE_C_LDFLAGS)
CXX_LDFLAGS += $(LDFLAGS) $(COVERAGE_CXX_LDFLAGS)

ASM_LIBS += $(LIBS) -Wl,-flto
C_LIBS += $(LIBS) -Wl,-flto
CXX_LIBS += $(LIBS) -Wl,-flto

# additional tools (instrumentation)
GDB = gdb
GDB_OPT =

# valgrind (memcheck)
VALGRIND = valgrind
VALGRIND_OPT = --verbose --read-inline-info=yes --read-var-info=yes --leak-check=full --show-leak-kinds=all --leak-check-heuristics=all --track-origins=yes

# valgrind's massif
MASSIF_OUTPUT = $(patsubst %.elf,%.elf.massif.out,$(APP))
MASSIF_OPT =

# coverage object
ifeq ($(COVERAGE_TEST),)
	# no coverage testing

	ASM_COVERAGE_GCNO =
	C_COVERAGE_GCNO =
	CXX_COVERAGE_GCNO =

	ASM_COVERAGE_GCDA =
	C_COVERAGE_GCDA =
	CXX_COVERAGE_GCDA =

	APP_COVERAGE_GCNO =
	APP_COVERAGE_GCDA =
else
ifeq ($(COVERAGE_TEST),0)
	# no coverage testing

	# compilation
	ASM_COVERAGE_GCNO =
	C_COVERAGE_GCNO =
	CXX_COVERAGE_GCNO =

	# running
	ASM_COVERAGE_GCDA =
	C_COVERAGE_GCDA =
	CXX_COVERAGE_GCDA =

	# no filtering and just add suffix to app coverage
	# add ltrans (main.elf.ltrans0.ltrans.gcno)
	APP_COVERAGE_GCNO =
	APP_COVERAGE_GCDA =
else
	# result of compilation
	ASM_COVERAGE_GCNO = $(filter %.gcno,$(patsubst %.S,%.S.gcno,$(ASM_SRCS))) $(filter %.gcno,$(patsubst %.s,%.s.gcno,$(ASM_SRCS)))
	C_COVERAGE_GCNO = $(patsubst %.c,%.c.gcno,$(C_SRCS))
	#CXX_COVERAGE_GCNO = $(filter %.gcno,$(patsubst %.C,%.C.gcno,$(CXX_SRCS))) $(filter %.gcno,$(patsubst %.cc,%.cc.gcno,$(CXX_SRCS))) $(filter %.gcno,$(patsubst %.cpp,%.cpp.gcno,$(CXX_SRCS))) $(filter %.gcno,$(patsubst %.cxx,%.cxx.gcno,$(CXX_SRCS)))
	CXX_COVERAGE_GCNO = $(filter %.gcno,$(patsubst %.cc,%.cc.gcno,$(CXX_SRCS))) $(filter %.gcno,$(patsubst %.cpp,%.cpp.gcno,$(CXX_SRCS))) $(filter %.gcno,$(patsubst %.cxx,%.cxx.gcno,$(CXX_SRCS)))

	# result of running
	ASM_COVERAGE_GCDA = $(filter %.gcda,$(patsubst %.S,%.S.gcda,$(ASM_SRCS))) $(filter %.gcda,$(patsubst %.s,%.s.gcda,$(ASM_SRCS)))
	C_COVERAGE_GCDA = $(patsubst %.c,%.c.gcda,$(C_SRCS))
	#CXX_COVERAGE_GCDA = $(filter %.gcda,$(patsubst %.C,%.C.gcda,$(CXX_SRCS))) $(filter %.gcda,$(patsubst %.cc,%.cc.gcda,$(CXX_SRCS))) $(filter %.gcda,$(patsubst %.cpp,%.cpp.gcda,$(CXX_SRCS))) $(filter %.gcda,$(patsubst %.cxx,%.cxx.gcda,$(CXX_SRCS)))
	CXX_COVERAGE_GCDA = $(filter %.gcda,$(patsubst %.cc,%.cc.gcda,$(CXX_SRCS))) $(filter %.gcda,$(patsubst %.cpp,%.cpp.gcda,$(CXX_SRCS))) $(filter %.gcda,$(patsubst %.cxx,%.cxx.gcda,$(CXX_SRCS)))

	# no filtering and just add suffix to app coverage
	# add ltrans (main.elf.ltrans0.ltrans.gcno)
	APP_COVERAGE_GCNO = $(APP:=.*.ltrans.gcno) $(APP:=.wpa.gcno)
	APP_COVERAGE_GCDA = $(APP:=.*.ltrans.gcda) $(APP:=.wpa.gcda)
endif
endif

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
	$(RM) --verbose $(ASM_OBJS) $(C_OBJS) $(CXX_OBJS) $(MASSIF_OUTPUT) $(ASM_COVERAGE_GCNO) $(C_COVERAGE_GCNO) $(CXX_COVERAGE_GCNO) $(ASM_COVERAGE_GCDA) $(C_COVERAGE_GCDA) $(CXX_COVERAGE_GCDA) $(APP_COVERAGE_GCNO) $(APP_COVERAGE_GCDA)

distclean: clean
	$(RM) --verbose $(APP)

# instrumentation
.PHONY: run run-valgrind run-massif debug debug-valgrind

run: $(APP)
	sh -c 'cd "$$(dirname "$$1")" && "./$$(basename "$$1")"' -- $<

run-valgrind: $(APP)
	sh -c 'cd "$$(dirname "$$1")" && $(VALGRIND) $(VALGRIND_OPT) "./$$(basename "$$1")"' -- $<

run-massif: $(MASSIF_OUTPUT)

debug: $(APP)
	$(GDB) $(GDB_OPT) $<

debug-valgrind: $(APP)
	sh -c 'cd "$$(dirname "$$1")" && $(VALGRIND) $(VALGRIND_OPT) --vgdb-error=1 --vgdb-stop-at=startup "./$$(basename "$$1")" & VALGRIND_PID="$$!" && sleep 2 && VGDB="$$(which "vgdb")" && $(GDB) $(GDB_OPT) -ex "target remote | $$VGDB --pid=$$VALGRIND_PID" "$$(basename "$$1")" && wait "$$VALGRIND_PID"' -- $<
