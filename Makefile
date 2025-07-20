##################################################################################
# Author:          Mingxuan Li, Zhantong Zhu
# Acknowledgement: Cursor + Claude
# Description:     C Code Compile Script for RISCV
##################################################################################

RISCV_GCC?=~/RISC-V-GCC-TOOLCHAIN/riscv/bin/riscv-none-elf-gcc
RISCV_OBJDUMP?=~/RISC-V-GCC-TOOLCHAIN/riscv/bin/riscv-none-elf-objdump
RISCV_ELF_LD?=~/RISC-V-GCC-TOOLCHAIN/riscv/bin/riscv-none-elf-ld

SRC_DIR=src
UTILS_DIR=utils
BUILD_DIR=build
BINARY_DIR=bin

MAIN?=main
OUTPUT_ELF=$(BINARY_DIR)/$(MAIN).elf
OUTPUT_ASM=$(BUILD_DIR)/$(MAIN).asm
OUTPUT_HEX=$(BUILD_DIR)/$(MAIN).hex

ALL_DEPENDENCIES = $(shell $(RISCV_GCC) -M $(SRC_DIR)/$(MAIN).c 2>/dev/null | sed -e 's/^[^:]*: *//' -e 's/\\$$//' | tr -d '\n')
SRC_FILES = $(sort $(patsubst %.h,%.c, $(filter $(SRC_DIR)/%, $(ALL_DEPENDENCIES))))


all: $(OUTPUT_ELF)

$(OUTPUT_ELF): $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BINARY_DIR)
	$(RISCV_GCC) -mcmodel=medany -Wall -mexplicit-relocs -march=rv64im_zicsr -mabi=lp64 -nostdlib -static -Tlinker.ld -ggdb -Wl,--no-gc-sections -fno-builtin -fno-tree-loop-distribute-patterns -O1 $(SRC_DIR)/startup.S $(SRC_FILES) -o $(OUTPUT_ELF)
	$(RISCV_OBJDUMP) -D -s $(OUTPUT_ELF) > $(OUTPUT_ASM)
	python3 $(UTILS_DIR)/asm2hex.py $(OUTPUT_ASM) $(OUTPUT_HEX)
	python3 $(UTILS_DIR)/gdb_scripts.py $(MAIN)

clean:
	rm -rf $(BUILD_DIR)

.PHONY:
	all clean
