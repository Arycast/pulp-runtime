# Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT.

RM ?= rm --verbose -f

.PHONY: all clean

all: elf_to_flash_image

# build the tester
elf_to_flash_image: elf_to_flash_image.c
	$(CC) $(CFLAGS) -g -Wall -Wextra -pedantic -Wconversion -std=c11 $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) elf_to_flash_image
