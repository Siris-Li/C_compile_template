//////////////////////////////////////////////////////////////////////////////////
// Author:          Zhantong Zhu
// Acknowledgement: GitHub Copilot
// Description:     Startup code for RISC-V
//////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

extern int main(void);

void _start(void) __attribute__((section(".text.init")));

// Entry point - calls main and halts
void _start(void)
{
	main();

	// Halt after main returns (infinite loop)
	while (1)
	{
	}
}
