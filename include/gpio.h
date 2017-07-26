/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
#ifndef __GPIO_H__
#define __GPIO_H__
#include <arch/trap.h>
/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/
#define GPIO_OUT 1
#define GPIO_IN  0
#define HIGH 1
#define LOW  0

typedef void (*gpio_func_cb)( void* arg );

typedef struct gpio_irq_handler
{
	gpio_func_cb hnd;
	void* arg;	
} gpio_irq_handler;

/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
void gpio_enable(int pin, int mode);
void pin_mode(int pin, int mode);
int digital_read(int pin);
void digital_write(int pin, int val);
void digital_write_two(int pin, int val, int pin2, int val2);
void pin_en_intr(int pin, int mode, gpio_func_cb handler,void* arg);
void pin_dis_intr(int pin);
void pin_trigger(int pin);
void gpio_handler(void);

#endif
