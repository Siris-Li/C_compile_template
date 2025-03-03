RISCV_GCC?=riscv-none-elf-gcc
RISCV_OBJDUMP?=riscv-none-elf-objdump

SRC_DIR=src
BUILD_DIR=build

SRC_FILE?=
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: $(OBJ_FILES)

single: $(BUILD_DIR)/$(SRC_FILE:.c=.o)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(RISCV_GCC) -mcmodel=medany -Wall -mexplicit-relocs -march=rv64im -mabi=lp64 -nostdlib -static -Tlinker.ld -ggdb -Wl,--no-gc-sections $< -o $@
	$(RISCV_OBJDUMP) -D -s $@ > $(BUILD_DIR)/$*.asm
	python asm2hex.py  $(BUILD_DIR)/$*.asm $(BUILD_DIR)/$*.hex

clean:
	rm -rf $(BUILD_DIR)
