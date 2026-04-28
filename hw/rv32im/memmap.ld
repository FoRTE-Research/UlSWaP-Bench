/* riscv-virt.ld */

OUTPUT_ARCH(riscv)
ENTRY(_start)

/* QEMU virt machine RAM layout */
MEMORY
{
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

/* Sections */
SECTIONS
{
    /* Start at RAM base */
    . = ORIGIN(RAM);

    /* Text section (code) */
    .text :
    {
        KEEP(*(.init))
        KEEP(*(.text.start))
        *(.text .text.*)
        . = ALIGN(4096);
        __heap_end = .;
    } > RAM

    /* Read-only data */
    .rodata :
    {
        *(.rodata .rodata.*)
    } > RAM

    /* Initialized data */
    .data :
    {
        *(.data .data.*)
    } > RAM

    /* Uninitialized data (zeroed at runtime) */
    .bss :
    {
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    } > RAM

    /* Stack (grows downward) */
    .stack (NOLOAD) :
    {
        . = ALIGN(16);
        __stack_top = ORIGIN(RAM) + LENGTH(RAM);
        __stack_bottom = __stack_top - 0x4000; /* 16 KB stack */
    } > RAM

    /* Heap start */
    __heap_start = __bss_end;
    
    /* End of the entire image */
    _end = .;
}

