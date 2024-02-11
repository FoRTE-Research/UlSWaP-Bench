/* memmap */

MEMORY
{
    rom : ORIGIN = 0x00000000, LENGTH = 0x0001FF00
    ram : ORIGIN = 0x00020000, LENGTH = 0x0007A000
    h   : ORIGIN = 0x00020000, LENGTH = 0x0007A000
}
IO_BASE = 0x40000000;

SECTIONS
{
    .text : ALIGN(4) { *(.text*) } > rom
    .bss  : ALIGN(4) { *(.bss*)  } > ram
    .heap : ALIGN(4) { *(.heap*) } > h
}


ENTRY(_start)

end = ORIGIN(h) + LENGTH(h);

_stacktop = ORIGIN(ram) + LENGTH(ram);
/* __heap_current = ORIGIN(h); */
__heap_start = ORIGIN(h);
__heap_end = ORIGIN(h) + LENGTH(h);
