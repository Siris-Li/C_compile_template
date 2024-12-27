RISCV_GCC?=riscv-none-elf-gcc
RISCV_OBJDUMP?=riscv-none-elf-objdump

all:
	$(RISCV_GCC) -mcmodel=medany -Wall -mexplicit-relocs -march=rv64im -mabi=lp64 -nostdlib -static -Tlinker.ld -ggdb -Wl,--no-gc-sections test_code.c -o test_code.o
	$(RISCV_OBJDUMP) -D -s test_code.o > test_code.asm
	python asm2hex.py

clean:
	rm test_code.o test_code.asm init_mem.hex
