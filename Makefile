##################################################################################
# Author:          Mingxuan Li, Zhantong Zhu
# Acknowledgement: Cursor + Claude
# Description:     C Code Compile Script for RISCV
##################################################################################

RISCV_GCC?=~/RISC-V-GCC-TOOLCHAIN/riscv/bin/riscv-none-elf-gcc
RISCV_OBJDUMP?=~/RISC-V-GCC-TOOLCHAIN/riscv/bin/riscv-none-elf-objdump

SRC_DIR=src
UTILS_DIR=utils
BUILD_DIR=build
BINARY_DIR=bin

MAIN?=main
OUTPUT_OBJ=$(BINARY_DIR)/$(MAIN).o
OUTPUT_ASM=$(BUILD_DIR)/$(MAIN).asm
OUTPUT_HEX=$(BUILD_DIR)/$(MAIN).hex

ALL_DEPENDENCIES = $(shell $(RISCV_GCC) -M $(SRC_DIR)/$(MAIN).c 2>/dev/null | sed -e 's/^[^:]*: *//' -e 's/\\$$//' | tr -d '\n')
SRC_FILES = $(sort $(patsubst %.h,%.c, $(filter $(SRC_DIR)/%, $(ALL_DEPENDENCIES))))


all: $(OUTPUT_OBJ)

$(OUTPUT_OBJ): $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BINARY_DIR)
	$(RISCV_GCC) -mcmodel=medany -Wall -mexplicit-relocs -march=rv64im_zicsr -mabi=lp64 -nostdlib -static -Tlinker.ld -ggdb -Wl,--no-gc-sections $(SRC_DIR)/startup.c $(SRC_FILES) -o $(OUTPUT_OBJ)
	$(RISCV_OBJDUMP) -D -s $(OUTPUT_OBJ) > $(OUTPUT_ASM)
	python3 $(UTILS_DIR)/asm2hex.py $(OUTPUT_ASM) $(OUTPUT_HEX)

clean:
	rm -rf $(BUILD_DIR)
