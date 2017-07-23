#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "lwip/opt.h"

#if defined(CONFIG_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef SemaphoreHandle_t sys_sem_t;
typedef SemaphoreHandle_t sys_mutex_t;
typedef QueueHandle_t sys_mbox_t;
typedef TaskHandle_t sys_thread_t;
typedef unsigned long sys_prot_t;

#define sys_mbox_valid( x ) ( ( ( *x ) == NULL) ? pdFALSE : pdTRUE )
#define sys_mbox_set_invalid( x ) ( ( *x ) = NULL )
#define sys_sem_valid( x ) ( ( ( *x ) == NULL) ? pdFALSE : pdTRUE )
#define sys_sem_set_invalid( x ) ( ( *x ) = NULL )

#else
#include "os_api.h"

typedef os_sem          sys_sem_t;
typedef os_thread_info* sys_thread_t;
typedef os_msgq*        sys_mbox_t;
typedef os_mutex        sys_mutex_t;
typedef unsigned long   sys_prot_t;

//#define SYS_SEM_NULL                ((uint32_t) NULL)
#define sys_sem_valid(x)            ((((sys_sem_t*)x)->valid == 0) ? 0 : 1)
#define sys_sem_set_invalid(x)      (((sys_sem_t*)x)->valid = 0)

//#define SYS_MBOX_NULL               NULL
#define sys_mbox_valid(x)           ((*x == NULL) ? 0 : 1 )
#define sys_mbox_set_invalid(x)     (*x = NULL )
#endif

#endif
