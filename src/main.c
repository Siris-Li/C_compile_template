//////////////////////////////////////////////////////////////////////////////////
// Author:          Mingxuan Li, Zhantong Zhu
// Acknowledgement: Cursor + Claude
// Description:     C Code Template for RISC-V
//////////////////////////////////////////////////////////////////////////////////

/////////// RISC-V ABI Naming Convention ///////////
// x0  => zero   (always 0)
// x1  => ra     (return address)
// x2  => sp     (stack pointer)
// x3  => gp     (global pointer)
// x4  => tp     (thread pointer)
// x5  => t0     (temporary register 0)
// x6  => t1     (temporary register 1)
// x7  => t2     (temporary register 2)
// x8  => s0/fp  (saved register 0/frame pointer)
// x9  => s1     (saved register 1)
// x10 => a0     (function argument 0)
// x11 => a1     (function argument 1)
// x12 => a2     (function argument 2)
// x13 => a3     (function argument 3)
// x14 => a4     (function argument 4)
// x15 => a5     (function argument 5)
// x16 => a6     (function argument 6)
// x17 => a7     (function argument 7)
// x18 => s2     (saved register 2)
// x19 => s3     (saved register 3)
// x20 => s4     (saved register 4)
// x21 => s5     (saved register 5)
// x22 => s6     (saved register 6)
// x23 => s7     (saved register 7)
// x24 => s8     (saved register 8)
// x25 => s9     (saved register 9)
// x26 => s10    (saved register 10)
// x27 => s11    (saved register 11)
// x28 => t3     (temporary register 3)
// x29 => t4     (temporary register 4)
// x30 => t5     (temporary register 5)
// x31 => t6     (temporary register 6)
/////////////////////////////////////////////////////

#include <stdint.h>
#include "uart.h"

__attribute__((section(".custom_data")))
static const uint64_t custom_patterns[] = {
    0x1234567890abcdef,
    0xfedcba0987654321,
    0xaaaaaaaaaaaaaaaa,
    0x5555555555555555,
    0x0011223344556677,
    0x8899aabbccddeeff,
    0xdeadbeefdeadbeef,
    0xfeedfacefeedface
};

int main() {
  // inline assembly code
  __asm__ volatile(
    "li t0, 1;"           // load "1" to register t0
    "li t1, 2;"           // load "2" to register t1
    "li t2, 0x80001000;"  // load "0x80001000" to register t2

    "add t3, t0, t1;"     // add t0 and t1, store the result in t3
    "sub t4, t0, t1;"     // sub t0 and t1, store the result in t4

    "sw t3, 0(t2);"       // store the value in t3 to the address in t2
    "sw t4, 4(t2);"       // store the value in t4 to the address in t2 + 4

    "lw t5, 0(t2);"       // load the value in the address in t2 to t5
    "lw t6, 4(t2);"       // load the value in the address in t2 + 4 to t6
    );

  // normal C code
  uint64_t* mem_base = (uint64_t*)0x80001000;
  *(mem_base + 0) = (uint64_t)0xb6acad2abb260109;
  *(mem_base + 1) = (uint64_t)0x11752c63ab69c863;

  // function call
  init_uart(10000000, 101000);
  print_uart("Hello, World!\n");

  return 0;
}