ENTRY(main)

SECTIONS
{
    . = 0x1000;
    __KERNEL_START = .;
    
    .text : {
        __TEXT_START = .;
        *(.text.boot)     /* Boot code first */
        *(.text)          /* All other code */
        *(.text.*)
        __TEXT_END = .;
    }
    
    /* Read-only data section */
    .rodata : {
        __RODATA_START = .;
        *(.rodata)
        *(.rodata.*)
        __RODATA_END = .;
    }
    
    /* Initialized data section */
    .data : {
        __DATA_START = .;
        *(.data)
        *(.data.*)
        __DATA_END = .;
    }
    
    /* Uninitialized data section */
    .bss : {
        __BSS_START = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __BSS_END = .;
    }
    
    /* Mark end of kernel */
    . = ALIGN(4K);
    __KERNEL_END = .;
}