OUTPUT_ARCH( "riscv" )
ENTRY( _start )

SECTIONS
{
    . = 0x80000000;

    .text : {
        *(.text.init)
        *(.text)
        *(.text.*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    .data : {
        . = ALIGN(16);
        *(.data)
        *(.data.*)
    }

    .bss : {
        . = ALIGN(16);
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }
}
