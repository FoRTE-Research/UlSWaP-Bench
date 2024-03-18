/* memmap */
OUTPUT_ARCH("riscv")

__bram_size = 0x7F000;
__rom_size = 0x4F000;

MEMORY
{
    HOST : ORIGIN = 0x80000000,                 LENGTH = 0x00001000
    ROM  : ORIGIN = 0x80002000,                 LENGTH = __rom_size
    RAM  : ORIGIN = (ORIGIN(ROM) + __rom_size), LENGTH = (__bram_size - __rom_size)
    HEAP : ORIGIN = (ORIGIN(ROM) + __rom_size), LENGTH = (__bram_size - __rom_size)
}
IO_BASE = 0x90000000;


SECTIONS
{
    .tohost : ALIGN(0x1000) { *(.tohost) } > HOST
    .text   : ALIGN(0x1000) { *(.text*)  } > ROM
    .bss    : ALIGN(0x1000) { *(.bss*)   } > RAM
    .data   : ALIGN(0x1000) { *(.data*)  } > RAM
    .heap   : ALIGN(0x1000) { *(.heap*)  } > HEAP
}

ENTRY(_start)

/* end = ORIGIN(h) + LENGTH(h); */

_stacktop = ORIGIN(RAM) + LENGTH(RAM);
__heap_current = ORIGIN(HEAP);
__heap_start = ORIGIN(HEAP);
__heap_end = ORIGIN(HEAP) + LENGTH(HEAP);
