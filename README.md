# C Compile Template

This repository contains a template for compiling C code using RISC-V GCC and generating hex files from assembly output.
The directory structure and Makefile are designed to handle multiple C source files and allow for flexible compilation options.

## Directory Structrue

```
.
├── Makefile
├── asm2hex.py
├── build
│   ├── test_code.asm
│   ├── test_code.hex
│   └── test_code.o
├── linker.ld
└── src
    └── test_code.c
```

- `Makefile`: The Makefile used to compile the C source files and generate the necessary output files.
- `asm2hex.py`: A Python script to convert assembly files to hex files.
- `linker.ld`: The linker script used during the compilation process.
- `src/`: Directory containing the C source files.
- `build/`: Directory where the compiled object files, assembly files, and hex files will be placed.

## Usage

### Compiling All C Files

To compile all C files in the `src` directory, simply run:

```sh
make
```

This will compile all `.c` files in the `src` directory and place the output files in the `build` directory.

### Compiling a Single C File

To compile a specific C file, pass the `SRC_FILE` variable when running make:

```sh
make single SRC_FILE=test_code.c
```

This will compile the specified `.c` file and place the output files in the `build` directory.

### Cleaning Up

To clean up the `build` directory and remove all generated files, run:

```sh
make clean
```

## `asm2hex.py`

The `asm2hex.py` script converts assembly files to hex files. It takes two command-line arguments: the input assembly file and the output hex file.

### Usage

```sh
python asm2hex.py <input_asm_file> <output_hex_file>
```