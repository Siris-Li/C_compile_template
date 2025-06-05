#################################################################################
# Author:          Zhantong Zhu, Mingxuan Li
# Acknowledgement: GitHub Copilot
# Description:     Startup code for RISC-V (Assembly version)
#################################################################################

.section .text.init
.global _start
.extern main

# Entry point - calls main and halts
_start:
    # Setup stack frame & return address
    li   sp, 0x80020000       # Initialize stack pointer
    li   ra, 0x80000000       # Initialize return address (bootrom)

    # Call main function
    call main

loop:
    # Infinite loop to halt execution
    j loop
