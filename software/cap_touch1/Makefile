# Copyright 2019 SiFive, Inc #
# SPDX-License-Identifier: Apache-2.0 #

PROGRAM ?= cap_touch
RISCV_PATH ?=~/Documents/programming/riscv/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14
RISCV_OPENOCD_PATH ?=~/Documents/programming/riscv/riscv-openocd-0.10.0-2019.08.2-x86_64-linux-ubuntu14
FREEDOM_SDK_ROOT=../..
METAL_DIR=$(FREEDOM_SDK_ROOT)/freedom-metal
CFLAGS=-I$(METAL_DIR)
CC=$(RISCV_PATH)/bin/riscv64-unknown-elf-gcc
BINDIR=./bin

#$(BINDIR)/%.o: $(SRC_FILES)/%.c
#	$(CC) -c -o $@ $<

main:
	@mkdir $(BINDIR)	
	$(CC) $(PROGRAM).c $(CFLAGS) -c -o $(BINDIR)/$(PROGRAM).o

clean:
	rm -f $(BINDIR)
