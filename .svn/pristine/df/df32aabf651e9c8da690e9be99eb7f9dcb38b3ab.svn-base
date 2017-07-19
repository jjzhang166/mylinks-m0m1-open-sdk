#include "arch/sys_arch.h"
#include "lwip/opt.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/stats.h"

#if NO_SYS
#else

#define error(x)    LWIP_DEBUGF(LWIP_DBG_ON, (x))
/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new(sys_mutex_t *mutex) 
{
#if defined(CONFIG_FREERTOS)

#else
    os_mutex_init(mutex);
    return ERR_OK;
#endif
}

/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock(sys_mutex_t *mutex) 
{
    //os_mutex_lock(mutex);
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *mutex) 
{
    //if(os_mutex_unlock(mutex) == -1)
        //error("sys_mutex_unlock error\n");
}

/** Delete a mutex
 * @param mutex the mutex to delete */
void sys_mutex_free(sys_mutex_t *mutex) 
{
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. The "ucCount" argument specifies
 *      the initial state of the semaphore.
 *      NOTE: Currently this routine only creates counts of 1 or 0
 * Inputs:
 *      sys_sem_t sem         -- Handle of semaphore
 *      u8_t count            -- Initial count of semaphore
 * Outputs:
 *      err_t                 -- ERR_OK if semaphore created
 *---------------------------------------------------------------------------*/
err_t sys_sem_new(sys_sem_t *sem, u8_t count) 
{
#if defined(CONFIG_FREERTOS)
	err_t xReturn = ERR_MEM;

	vSemaphoreCreateBinary( ( *sem ) );

	if( *sem != NULL )
	{
		if( count == 0U )
		{
			xSemaphoreTake( *sem, 1UL );
		}

		xReturn = ERR_OK;
		SYS_STATS_INC_USED( sem );
	}
	else
	{
		SYS_STATS_INC( sem.err );
	}

	return xReturn;
#else
    os_semaphore_init(sem, count);
    return ERR_OK;
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *  
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout. 
 *                                 If timeout is zero, semaphore wait foever.
 * Outputs:
 *      u32_t                   -- Time elapsed or SYS_ARCH_TIMEOUT.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
#if defined(CONFIG_FREERTOS)
	TickType_t xStartTime, xEndTime, xElapsed;
	unsigned long ulReturn;

	xStartTime = xTaskGetTickCount();

	if( timeout != 0UL )
	{
		if( xSemaphoreTake( *sem, timeout / portTICK_PERIOD_MS ) == pdTRUE )
		{
			xEndTime = xTaskGetTickCount();
			xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;
			ulReturn = xElapsed;
		}
		else
		{
			ulReturn = SYS_ARCH_TIMEOUT;
		}
	}
	else
	{
		while( xSemaphoreTake( *sem, portMAX_DELAY ) != pdTRUE );
		xEndTime = xTaskGetTickCount();
		xElapsed = ( xEndTime - xStartTime ) * portTICK_PERIOD_MS;

		if( xElapsed == 0UL )
		{
			xElapsed = 1UL;
		}

		ulReturn = xElapsed;
	}

	return ulReturn;
#else
    u32_t start = millis();

    if(os_semaphore_wait(sem, timeout) < 1)
        return SYS_ARCH_TIMEOUT;

    return millis() - start;
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t *sem) 
{
#if defined(CONFIG_FREERTOS)
	xSemaphoreGive(*sem);
#else
    os_semaphore_post(sem);
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/
void sys_sem_free(sys_sem_t *sem)
{
#if defined(CONFIG_FREERTOS)
	SYS_STATS_DEC(sem.used);
	vQueueDelete(*sem);
#else

#endif
}
/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      int queue_sz            -- Size of elements in the mailbox. size will
 *                                 be 2^bits
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *---------------------------------------------------------------------------*/
err_t sys_mbox_new(sys_mbox_t *mbox, int queue_sz) 
{
#if defined(CONFIG_FREERTOS)
	err_t xReturn = ERR_MEM;

	*mbox = xQueueCreate( queue_sz, sizeof( void * ) );

	if( *mbox != NULL )
	{
		xReturn = ERR_OK;
		SYS_STATS_INC_USED( mbox );
	}

	return xReturn;
#else
    int bits;

    for(bits = 0; bits < 7; bits++)
    {
        if((1 << bits) >= queue_sz)
            break;
    }

    if(bits > 0 && bits < 7)
        ;
    else
        error("sys_mbox_new size error\n");
    
    *mbox = os_msgq_create(bits);
    return ERR_OK;
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a mailbox. If there are messages still present in the
 *      mailbox when the mailbox is deallocated, it is an indication of a
 *      programming error in lwIP and the developer should be notified.
 * Inputs:
 *      sys_mbox_t *mbox         -- Handle of mailbox
 *---------------------------------------------------------------------------*/
void sys_mbox_free(sys_mbox_t *mbox) 
{
#if defined(CONFIG_FREERTOS)
	unsigned long ulMessagesWaiting;

	ulMessagesWaiting = uxQueueMessagesWaiting( *mbox );
	configASSERT( ( ulMessagesWaiting == 0 ) );

	#if SYS_STATS
	{
		if( ulMessagesWaiting != 0UL )
		{
			SYS_STATS_INC( mbox.err );
		}

		SYS_STATS_DEC( mbox.used );
	}
	#endif /* SYS_STATS */

	vQueueDelete( *mbox );
#else
    if(os_msgq_free(*mbox) == -1)
        error("sys_mbox_free error\n");
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox        -- Handle of mailbox
 *      void *msg              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
#if defined(CONFIG_FREERTOS)
	while( xQueueSendToBack( *mbox, &msg, portMAX_DELAY ) != pdTRUE );
#else
    os_msgq_post(*mbox, MSG_NULL, 0, msg, EVENT_WAIT_INFINITE);
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg) 
{
#if defined(CONFIG_FREERTOS)
	err_t xReturn;

	if( xQueueSend( *mbox, &msg, 0UL ) == pdPASS )
	{
		xReturn = ERR_OK;
	}
	else
	{
		/* The queue was already full. */
		xReturn = ERR_MEM;
		SYS_STATS_INC( mbox.err );
	}

	return xReturn;
#else
    int ret;
    ret = os_msgq_post(*mbox, MSG_NULL, 0, msg, EVENT_NO_WAIT);
    return (ret == 0) ? (ERR_OK) : (ERR_MEM);
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_TIMEOUT if timeout, else number
 *                                  of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
#if defined(CONFIG_FREERTOS)
	void *pvDummy;
	TickType_t xStartTime, xEndTime, xElapsed;
	unsigned long ulReturn;

	xStartTime = xTaskGetTickCount();

	if( NULL == msg )
	{
		msg = &pvDummy;
	}

	if( timeout != 0UL )
	{
		if( pdTRUE == xQueueReceive( *mbox, &( *msg ), timeout/ portTICK_PERIOD_MS ) )
		{
			xEndTime = xTaskGetTickCount();
			xElapsed = ( xEndTime - xStartTime ) * portTICK_PERIOD_MS;

			ulReturn = xElapsed;
		}
		else
		{
			/* Timed out. */
			*msg = NULL;
			ulReturn = SYS_ARCH_TIMEOUT;
		}
	}
	else
	{
		while( pdTRUE != xQueueReceive( *mbox, &( *msg ), portMAX_DELAY ) );
		xEndTime = xTaskGetTickCount();
		xElapsed = ( xEndTime - xStartTime ) * portTICK_PERIOD_MS;

		if( xElapsed == 0UL )
		{
			xElapsed = 1UL;
		}

		ulReturn = xElapsed;
	}

	return ulReturn;
#else
    os_msg lmsg1, *lmsg = &lmsg1;
    u32_t start = millis();
    
    if(os_msgq_get(*mbox, lmsg, (timeout != 0)?(timeout):(EVENT_WAIT_INFINITE)) == -1)
        return SYS_ARCH_TIMEOUT;

    *msg = lmsg->data;

    return millis() - start;
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_tryfetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Similar to sys_arch_mbox_fetch, but if message is not ready
 *      immediately, we'll return with SYS_MBOX_EMPTY.  On success, 0 is
 *      returned.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 * Outputs:
 *      u32_t                   -- SYS_MBOX_EMPTY if no messages.  Otherwise,
 *                                  return ERR_OK.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg) 
{
#if defined(CONFIG_FREERTOS)
	void *pvDummy;
	unsigned long ulReturn;

	if( msg== NULL )
	{
		msg = &pvDummy;
	}

	if( pdTRUE == xQueueReceive( *mbox, &( *msg ), 0UL ) )
	{
		ulReturn = ERR_OK;
	}
	else
	{
		ulReturn = SYS_MBOX_EMPTY;
	}

	return ulReturn;
#else
    os_msg lmsg1, *lmsg=&lmsg1;
    
    if(os_msgq_get(*mbox, lmsg, MSG_NO_WAIT) == -1)
        return SYS_MBOX_EMPTY;
    
    *msg = lmsg->data;
    
    return ERR_OK;
#endif
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_thread_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Starts a new thread with priority "prio" that will begin its
 *      execution in the function "thread()". The "arg" argument will be
 *      passed as an argument to the thread() function. The id of the new
 *      thread is returned. Both the id and the priority are system
 *      dependent.
 * Inputs:
 *      char *name                -- Name of thread
 *      void (*thread)(void *arg) -- Pointer to function to run.
 *      void *arg                 -- Argument passed into function
 *      int stacksize             -- Required stack amount in bytes
 *      int priority              -- Thread priority
 * Outputs:
 *      sys_thread_t              -- Pointer to thread handle.
 *---------------------------------------------------------------------------*/
sys_thread_t sys_thread_new(const char *name,
                            void (*thread)(void *arg),
                            void *arg, int stacksize, int prio) 
{
#if defined(CONFIG_FREERTOS)
	TaskHandle_t xCreatedTask;
	portBASE_TYPE xResult;
	sys_thread_t xReturn;

	xResult = xTaskCreate( thread, name, stacksize, arg, prio, &xCreatedTask );

	if( xResult == pdPASS )
	{
		xReturn = xCreatedTask;
	}
	else
	{
		xReturn = NULL;
	}

	return xReturn;
#else
    os_thread_info *td;
    LWIP_DEBUGF(SYS_DEBUG | LWIP_DBG_TRACE, ("New Thread: %s\n", name));

    if((td = os_thread_create(name, thread, arg, stacksize, prio)) == NULL)
        error("sys_thread_new number error\n");

    return td;
#endif
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_init
 *---------------------------------------------------------------------------*
 * Description:
 *      Initialize sys arch
 *---------------------------------------------------------------------------*/
//os_mutex lwip_sys_mutex;

void sys_init(void) 
{
    LWIP_DEBUGF(SYS_DEBUG | LWIP_DBG_TRACE, ("sys_init\n"));
    //os_mutex_init(&lwip_sys_mutex);
}       

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      operating system.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t sys_arch_protect(void) 
{
    os_enter_critical();
    return (sys_prot_t) 1;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an operating system.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void sys_arch_unprotect(sys_prot_t p) 
{
    os_exit_critical();
}

u32_t sys_now(void) 
{
#if defined(CONFIG_FREERTOS)
	return (xTaskGetTickCount()*portTICK_PERIOD_MS);
#else
    return millis();
#endif
}

void sys_msleep(u32_t ms) 
{
#if defined(CONFIG_FREERTOS)
	vTaskDelay(ms/portTICK_PERIOD_MS);
#else
    os_sleep(ms);
#endif
}

#endif


