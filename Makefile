RISCV_GCC?=~/RISC-V-GCC-TOOLCHAIN/riscv/bin/riscv-none-elf-gcc
RISCV_OBJDUMP?=~/RISC-V-GCC-TOOLCHAIN/riscv/bin/riscv-none-elf-objdump

SRC_DIR=src
BUILD_DIR=build
OUTPUT?=program

SRC_NAME?=
SRC_FILE=$(SRC_NAME).c
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OUTPUT_OBJ=$(BUILD_DIR)/$(OUTPUT).o
OUTPUT_ASM=$(BUILD_DIR)/$(OUTPUT).asm
OUTPUT_HEX=$(BUILD_DIR)/$(OUTPUT).hex

all: $(OUTPUT_OBJ)

$(OUTPUT_OBJ): $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(RISCV_GCC) -mcmodel=medany -Wall -mexplicit-relocs -march=rv64im_zicsr -mabi=lp64 -nostdlib -static -Tlinker.ld -ggdb -Wl,--no-gc-sections $(SRC_FILES) -o $(OUTPUT_OBJ)
	$(RISCV_OBJDUMP) -D -s $(OUTPUT_OBJ) > $(OUTPUT_ASM)
	python3 asm2hex.py $(OUTPUT_ASM) $(OUTPUT_HEX)

single:
	@if [ -z "$(SRC_NAME)" ]; then \
		echo "Error: Please specify a file to compile, e.g.: make single SRC_NAME=example"; \
		exit 1; \
	fi
	@mkdir -p $(BUILD_DIR)
	$(RISCV_GCC) -mcmodel=medany -Wall -mexplicit-relocs -march=rv64im_zicsr -mabi=lp64 -nostdlib -static -Tlinker.ld -ggdb -Wl,--no-gc-sections $(SRC_DIR)/$(SRC_FILE) -o $(BUILD_DIR)/$(SRC_FILE:.c=.o)
	$(RISCV_OBJDUMP) -D -s $(BUILD_DIR)/$(SRC_FILE:.c=.o) > $(BUILD_DIR)/$(SRC_FILE:.c=.asm)
	python3 asm2hex.py $(BUILD_DIR)/$(SRC_FILE:.c=.asm) $(BUILD_DIR)/$(SRC_FILE:.c=.hex)

clean:
	rm -rf $(BUILD_DIR)
