# C Compile Template

This repository contains a template for compiling C code using RISC-V GCC and generating hex files from assembly output.
The directory structure and Makefile are designed to handle multiple C source files and allow for flexible compilation options.

## Directory Structrue

```
.
├── Makefile
├── utils
│   ├── gdb_scripts.py
│   ├── 64b_2_128b.py
│   └── asm2hex.py
├── build
│   ├── main.asm
│   └── main.hex
├── bin
│   └── main.elf
├── scripts
│   └── main.gdb
├── linker.ld
└── src
    ├── main.c
    ├── func_call.c
    ├── inline_assembly.c
    ├── startup.S
    ├── uart.c
    └── uart.h
```

- `Makefile`: The Makefile used to compile the C source files and generate the necessary output files.
- `utils/gdb_scripts.py`: A Python script to generate GDB scripts for debugging.
- `utils/asm2hex.py`: A Python script to convert assembly files to hex files.
- `utils/64b_2_128b.py`: A Python script to convert the data width of the hex file.
- `linker.ld`: The linker script used during the compilation process.
- `src/`: Directory containing the C source files.
- `build/`: Directory where the compiled disassembly files and hex files will be placed.
- `bin/`: Directory where the compiled object files will be placed.
- `scripts/`: Directory where the generated GDB scripts will be placed.

## Usage

### Prerequisites

- RISCV Toolchain
- Python 3.x

Before running the Makefile, you need to set the RISCV_GCC and RISCV_OBJDUMP environment variables to the path of your RISCV toolchain.

```sh
export RISCV_GCC=<path_to_riscv_gcc>
export RISCV_OBJDUMP=<path_to_riscv_objdump>
```

or you can set the environment variables in the Makefile:

```sh
RISCV_GCC?=<path_to_riscv_gcc>
RISCV_OBJDUMP?=<path_to_riscv_objdump>
```

### Compiling C Files

To compile C files in the `src` directory, simply run:

```sh
make MAIN=<main_file_name>
```
If you don't specify the `MAIN` variable, the default main file name will be `main`.

This will compile  `${MAIN}.c` files and all dependencies (found automatically by script) in the `src` directory and generate the output files (`bin/${MAIN}.elf, build/${MAIN}.asm, build/${MAIN}.hex, scripts/`).

### Cleaning Up

To clean up the `build` directory and remove all generated files, run:

```sh
make clean
```

## `gdb_scripts.py`

This script generates GDB scripts for debugging the compiled C files, which will be called automatically by the Makefile.

### Usage

```sh
python gdb_scripts.py <main_file_name>
```

## `asm2hex.py`

This script will be called automatically by the Makefile when compiling C files.

The `asm2hex.py` script converts assembly files to hex files. It takes two command-line arguments: the input assembly file and the output hex file.

### Usage

```sh
python asm2hex.py <input_asm_file> <output_hex_file>
```

## `64b_2_128b.py`

This script converts the generated `program.hex` into a 128-bit wide hex file `program_128b.hex`.

This is needed if the data width main memory of your system doesn't match 64-bit. Modifications are needed if your main memory width is not 128-bit.

### Usage

```sh
python 64b_2_128b.py
```

The script will find the `build/program.hex` and convert it to `build/program_128b.hex`.

## RISCV Toolchain

If you want to install a RISCV toolchain, please refer to [RISCV Toolchain](https://github.com/Siris-Li/RISC-V-GCC-TOOLCHAIN) for more information.
