/*
 * we define extra memory area for stack to insure
 * content of ram never exceeds avaliable space
 */


MEMORY
   {
        ram   (rwx)  : ORIGIN = 0x00002dc0, LENGTH = 0x20000-0x00002dc0
		dma   (rwx)  : ORIGIN = CONFIG_DATABUF_START-CONFIG_DMA_SIZE, LENGTH = CONFIG_DMA_SIZE
   }


SECTIONS
{
    .text :
    {
    	*(.text)
	} > ram


	.rodata :
	{
		*(.rodata .rodata.*)
		. = ALIGN(0x4);
	} > ram

	.data : AT(ADDR(.rodata) + SIZEOF(.rodata))
	{
        *(.data .data.*)
		__start_cmdt = .;
		*(cmdt)
		__stop_cmdt = .;
	} > ram

	. = ALIGN(0x4);
        _bss_begin = .;

	.bss :
	{
		*(.scommon)
		*(.bss)
	} > ram

	. = ALIGN(0x4);
	_heap = .;
	.eh_frame : { . = ALIGN(8); *(.eh_frame .eh_frame.*) } > ram		
	PROVIDE(_heap_end = ORIGIN(ram) + LENGTH(ram) - 128 - 4096);

	.dma : 
	{
		*(.dma .dma.*)
	} > dma

}

INCLUDE "../include/rlib_sym.lds"

INCLUDE "../../include/rlib_sym.lds"
