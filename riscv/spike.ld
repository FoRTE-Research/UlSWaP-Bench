/* memmap */

MEMORY
{
    rom : ORIGIN = 0x80000000, LENGTH = 0x00004F00
    ram : ORIGIN = 0x80005000, LENGTH = 0x0007A000
    h   : ORIGIN = 0x80005000, LENGTH = 0x0007A000
}
IO_BASE = 0x90000000;


SECTIONS
{
    .text : ALIGN(0x0100) { *(.text*) } > rom
    .bss  : ALIGN(0x1000) { *(.bss*)  } > ram
    .heap : ALIGN(0x1000) { *(.heap*) } > h
}

ENTRY(_start)

end = ORIGIN(h) + LENGTH(h);

_stacktop = ORIGIN(ram) + LENGTH(ram);
__heap_current = ORIGIN(h);
__heap_start = ORIGIN(h);
__heap_end = ORIGIN(h) + LENGTH(h);
