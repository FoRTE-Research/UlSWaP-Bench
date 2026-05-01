__bram_size = 0x7F000;
__rom_size = 0x4F000;

MEMORY
{
    ROM  : ORIGIN = 0x00000000,                 LENGTH = __rom_size
    RAM  : ORIGIN = (ORIGIN(ROM) + __rom_size), LENGTH = (__bram_size - __rom_size)
    HEAP : ORIGIN = (ORIGIN(ROM) + __rom_size), LENGTH = (__bram_size - __rom_size)
}

SECTIONS
{
    .vectors : ALIGN(4)
    {
        *(.vectors)
    } > ROM
    
    .text : ALIGN(4)
    {
        *(.text)
    } > ROM
    
    .rodata : ALIGN(4)
    {
        *(.rodata)
    } > ROM
    
    .data : ALIGN(4)
    {
        *(.data)
    } > ROM
    
    .bss  : ALIGN(4)
    {
        *(.bss*)
    } > ROM
    
    .heap : ALIGN(4)
    {
        *(.heap*)
    } > HEAP
}


ENTRY(_start)

_stacktop = ORIGIN(RAM) + LENGTH(RAM);
__heap_start = ORIGIN(HEAP);
__heap_end = ORIGIN(HEAP) + LENGTH(HEAP);

