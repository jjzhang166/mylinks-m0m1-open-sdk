/*=============================================================================+
|                                                                              |
| Copyright 2016                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file omniconfig.h 
*   \brief define omniconfig result and interface functions 
*   \author Montage
*/
#ifndef OMNICONFIG_FUNC_H
#define OMNICONFIG_FUNC_H

#ifdef CONFIG_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#else
#include <os_api.h>
#endif

/*=============================================================================+
| Define                                                                       |
+=============================================================================*/

/*-----------------------------------------------------------------------------+
| System Function (Let users choose CONFIG_FREERTOS or CONFIG_OS)                                |
+-----------------------------------------------------------------------------*/

void* omnicfg_thread_new(	const char *name,
						void (*thread)(void *arg),
						void *arg, int stacksize, int prio) 
{
#if defined(CONFIG_FREERTOS)
	TaskHandle_t xCreatedTask;
	portBASE_TYPE xResult;
	void* xReturn;
	
	xResult = xTaskCreate( thread, name, stacksize, arg, prio, &xCreatedTask );
	
	if( xResult == pdPASS )
	{
		xReturn = xCreatedTask;
	}
	else
	{
		xReturn = NULL;
	}
	
	return (void*)xReturn;
#else
	os_thread_info *td;
	if((td = os_thread_create(name, thread, arg, stacksize, prio)) == NULL)
		serial_printf("sys_thread_new number error\n");

	return (void*)td;
#endif
}

void omnicfg_thread_end()
{
#if defined(CONFIG_FREERTOS)
	vTaskDelete(NULL);
#else
	os_thread_exit();
	for (;;)
		os_thread_yield();
#endif
}

void omnicfg_msleep(unsigned int ms) 
{
#if defined(CONFIG_FREERTOS)
	if(0 == ms)
		vTaskDelay(0);
	else
		vTaskDelay(ms/portTICK_PERIOD_MS);
#else
	os_sleep(ms);
#endif
}

#endif /*OMNICONFIG_FUNC_H*/
