/*
 * we define extra memory area for stack to insure
 * content of ram never exceeds avaliable space
 */


MEMORY
   {
        ram   (rwx)  : ORIGIN = 0x00001770, LENGTH = 0x20
   }


SECTIONS
{
	.rodata :
	{
		*(.rodata .rodata.*)
		. = ALIGN(0x4);
	} > ram

	.data : AT(ADDR(.rodata) + SIZEOF(.rodata))
	{
        *(.data .data.*)
	} > ram

}


