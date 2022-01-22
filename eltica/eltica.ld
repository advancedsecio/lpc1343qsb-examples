OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
    romall(rx) :   ORIGIN = 0x00000000, LENGTH = 0x8000
    romvector(r) : ORIGIN = 0x00000000, LENGTH = 0x400
    rom(rx) :      ORIGIN = 0x00000400, LENGTH = LENGTH(romall) - LENGTH(romvector)

    ramall(rwx) :  ORIGIN = 0x10000000, LENGTH = 0x2000
    data(rw) :     ORIGIN = 0x10000000, LENGTH = 0x1E00
    stack(rw) :    ORIGIN = 0x10001E00, LENGTH = 0x200
}

SECTIONS
{
    .romvector : {
        KEEP(*(.vector))

        /* 余った領域を一応フラッシュ ROM の初期値 0xFF で埋める。要らないかも */
        FILL(0xFF)
        . = LENGTH(romvector);
    } > romvector

    /* reset_handler() を .text セクションの先頭に配置する
       (なぜだかよくわからないが、そうしないと reset_handler() にジャンプしない) */
    .text : {
        *(.reset)
        . = ALIGN(4);
    } > rom

    .text : {
        *(.text)
    } > rom

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > rom

    _data_org = .;
    .data : {
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    } > data AT> rom

    .bss : {
        _sbss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > data AT> rom

    . = ALIGN(4);
    _end = .;

    .stack : {
        _main_sp = ORIGIN(stack) + LENGTH(stack);
    } > stack
}
