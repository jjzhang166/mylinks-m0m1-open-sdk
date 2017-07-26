/*
 * we define extra memory area for stack to insure
 * content of ram never exceeds avaliable space
 */


MEMORY
   {
        ram   (rwx)  : ORIGIN = 0x00001790, LENGTH = 0x50000-0x4000-0x00001790
		dma   (rwx)  : ORIGIN = 0x50000-0x4000, LENGTH = 0x4000
   }


SECTIONS
{
    .text :
    {
    	*(.text)
	} > ram


	.rodata ALIGN(8):
	{
		*(.rodata .rodata.*)
		_rodata_end = .;
	} > ram

	.data ALIGN(8):
	{
        *(.data .data.*)
		. = ALIGN(4);
		__start_cmdt = .;
		*(cmdt)
		__stop_cmdt = .;
	} > ram

	.bss ALIGN(8):
	{
		_bss_begin = .;
		*(.scommon)
		*(.bss)
		_bss_end = .;
	} > ram
	
	. = ALIGN(8);
	_heap = .;
	.eh_frame : { . = ALIGN(8); *(.eh_frame .eh_frame.*) } > ram		
	PROVIDE(_heap_end = ORIGIN(ram) + LENGTH(ram));

	.dma : 
	{
		*(.dma .dma.*)
	} > dma

}

INCLUDE "../../include/rlib_sym_v2.lds"
