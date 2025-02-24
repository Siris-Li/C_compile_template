#include <stdint.h>

// DCOBase = 64'h2000_0000
// DCOLength = 64'h1000

// CC_SEL[5:0]:   0x2000_0000
// FC_SEL[5:0]:   0x2000_0004
// DIV_SEL[2:0]:  0x2000_0008
// FREQ_SEL[1:0]: 0x2000_000C

void write_DCO() {
  // memory base address for DCO registers
  uint32_t* DCO_base = (uint32_t*)0x20000000;
  // write some value to DCO registers

  // write to CC_SEL
  *(DCO_base + 0) = (uint32_t)0b1010;
  // write to FC_SEL
  *(DCO_base + 1) = (uint32_t)0b1111;
  // write to DIV_SEL
  *(DCO_base + 2) = (uint32_t)0b101;
  // write to FREQ_SEL
  *(DCO_base + 3) = (uint32_t)0b10;
}