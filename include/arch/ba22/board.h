/*
 * Defines properties of hardware.
 */
#ifndef _BOARD_H_
#define _BOARD_H_

/* clock frequencies */
/* Default Raptor demo board: 62.5 MHz */
//#define IN_CLK  		62500000
#define IN_CLK  		100000000
//#define IN_CLK  		50000000
#define TICKS_PER_SEC		100
#define UART_BAUD_RATE		115200

// fake peripheral ports for simple_bench
#define CAST_CHAR  		0x90000000
#define CAST_NUM  		0x90000004

#define CAST_INTRMASK  		0x9000000C
#define CAST_INTRSR      	0x90000010
#define CAST_INTRCLR 	        0x90000020
#define CAST_VECINTR 	        0x90000024

#define CAST_SR          	0x90000014

#define CAST_TT_CNT             0x90000018
#define CAST_TT_INT             0x9000001C

#define CAST_ENDSIM             0x900000F0


#endif
