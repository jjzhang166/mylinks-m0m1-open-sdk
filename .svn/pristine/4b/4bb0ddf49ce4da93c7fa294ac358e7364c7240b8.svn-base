/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file 
*   \brief CAMELOT 
*   \author Montage
*/

#ifndef COMMON_H
#define COMMON_H

enum
{
    ERR_OK          = 0,
    ERR_HELP        = 1,
    ERR_AT          = 2,
    ERR_PARM        = -1,
    ERR_ALIGN       = -2,
    ERR_ADDR        = -3,
    ERR_FILE        = -4,
    ERR_TIMEOUT     = -5,
    ERR_ETHER       = -6,
    ERR_MEM         = -7,
    ERR_LAST        = -8,
};


#define ALIGN_TO(a,b) (((unsigned int)a + (b-1)) & ~(b-1))

#define MS_PER_SEC   	(1000)
#define US_PER_SEC		(1000000)
#if defined(CONFIG_FPGA)
#define OSCT_PER_MS		(20)
#else
#define OSCT_PER_MS		(32)
#endif
/*
 * TIME_AFTER(a,b) returns true if the time a is after time b.
 *
 * Do this with "<0" and ">=0" to only test the sign of the result. A
 * good compiler would generate better code (and a really good compiler
 * wouldn't care). Gcc is currently neither.
 */
 #define TIME_AFTER(a,b)     ((long)(b) - (long)(a) < 0)
 #define TIME_BEFORE(a,b)    TIME_AFTER(b,a)
  
 #define TIME_AFTER_EQ(a,b)  ((long)(a) - (long)(b) >= 0)
 #define TIME_BEFORE_EQ(a,b) TIME_AFTER_EQ(b,a)

#ifdef CONFIG_WLA_DEBUG_MSG
#define DBG(...)						serial_printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

void clock_init ();
void udelay (unsigned int time);
unsigned int how_long (unsigned int from);
extern int get_args(const char *string, char *argv[]);
extern char _ftext[], _fbss[], _end[];
extern unsigned long byte_count;
extern unsigned long buf_address;
extern int serial_printf(char *fmt, ...);

typedef void (*FUNCPTR)(void *);
typedef void (*HW_TIMER_FUNCPTR)(void);

extern int apps_printf;
extern char apps_buf[4096];
extern int apps_buf_len;

// Timer
extern unsigned int micros(void);
extern unsigned int millis(void);
extern void udelay(unsigned int us);
extern void hw_timer_start(unsigned int us, void (*func)(void), int autoload);
extern void hw_timer_stop(void);
extern void *add_timeout(FUNCPTR, void *sta, unsigned int expire_time);
extern void del_timeout(FUNCPTR, void *sta);
extern int arc4random(void);
extern void get_random_bytes(void *buf, unsigned int len);

#include    <cmd.h>
//#include    <arch/trap.h>

#endif
