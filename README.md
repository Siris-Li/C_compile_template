# C Compile Template

This repository contains a template for compiling C code using RISC-V GCC and generating hex files from assembly output.
The directory structure and Makefile are designed to handle multiple C source files and allow for flexible compilation options.

## Directory Structrue

```
.
├── Makefile
├── asm2hex.py
├── 64b_2_128b.py
├── build
│   ├── program.asm
│   ├── program.hex
│   └── program.o
├── linker.ld
└── src
    ├── main.c
    ├── start.c
    ├── uart.c
    └── uart.h
```

- `Makefile`: The Makefile used to compile the C source files and generate the necessary output files.
- `asm2hex.py`: A Python script to convert assembly files to hex files.
- `64b_2_128b.py`: A Python script to convert the data width of the hex file.
- `linker.ld`: The linker script used during the compilation process.
- `src/`: Directory containing the C source files.
- `build/`: Directory where the compiled object files, assembly files, and hex files will be placed.

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

### Compiling All C Files

To compile all C files in the `src` directory, simply run:

```sh
make OUTPUT=<output_name>
```
If you don't specify the `OUTPUT` variable, the default output name will be `program`.

This will compile all `.c` files in the `src` directory and place the output files (<output_name>.o, <output_name>.asm, <output_name>.hex) in the `build` directory.


### Compiling a Single C File

To compile a specific C file, pass the `SRC_NAME` variable when running make:

```sh
make single SRC_NAME=<source_name>
```

If you don't specify the `SRC_NAME` variable, an error will be thrown.

This will compile the specified `.c` file and place the output files (<source_name>.o, <source_name>.asm, <source_name>.hex) in the `build` directory.

### Cleaning Up

To clean up the `build` directory and remove all generated files, run:

```sh
make clean
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
