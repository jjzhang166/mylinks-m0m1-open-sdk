/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file 
*   \brief 
*   \author Montage
*/

#ifndef TRAP_H
#define TRAP_H

#define EXP_MAX 16

typedef void (*ifunc)(void *);
typedef struct irq_handler
{
    ifunc hnd;
    unsigned int id;
} irq_handler;

extern irq_handler ex_tables[EXP_MAX];

void set_ex_handler(unsigned int exp, void (*handler) (), void *id);
void trap_init();

#endif
