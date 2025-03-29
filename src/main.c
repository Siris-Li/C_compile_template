#include <stdint.h>
#include "uart.h"

int main() {
  // inline assembly code
  __asm__ volatile(
      "li t0, 1;"           // load "1" to register t0
      "li t1, 2;"           // load "2" to register t1
      "li t2, 0x80001000;"  // load "0x80001000" to register t2

      "add t3, t0, t1;"  // add t0 and t1, store the result in t3
      "sub t4, t0, t1;"  // sub t0 and t1, store the result in t4

      "sw t3, 0(t2);"  // store the value in t3 to the address in t2
      "sw t4, 4(t2);"  // store the value in t4 to the address in t2 + 4

      "lw t5, 0(t2);"  // load the value in the address in t2 to t5
      "lw t6, 4(t2);"  // load the value in the address in t2 + 4 to t6
  );

  // normal C code
  uint64_t* mem_base = (uint64_t*)0x80001000;
  *(mem_base + 0) = (uint64_t)0xb6acad2abb260109;
  *(mem_base + 1) = (uint64_t)0x11752c63ab69c863;

  // function call
  init_uart();
  print_uart("Hello, World!\n");

  return 0;
}