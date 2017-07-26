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

#ifndef IRQ_H
#define IRQ_H

#include <arch/trap.h>

void irq_init(void);
void request_irq(unsigned int irq, void (*handler) (), void *id);
// disable and save the orginal status
unsigned int irq_save(void);
// restore cpu's status
void irq_restore(unsigned int org);
void enable_irq(int irq);
void disable_irq(int irq);
void irq_enable(void);
void irq_disable(void);

#endif                          //IRQ_H
