#ifndef OS_API_H
#define OS_API_H
#include <stdint.h>
#include <stddef.h>

typedef void * OSHANDLE;

typedef struct _os_thread_info os_thread_info;

struct _os_thread_info {
    os_thread_info *next;               /* Linked list of all threads */
    os_thread_info *qnxt;               /* Linked list of all queued thread */
    volatile unsigned int qpec;         /* Pending event counter */
    char name[9];                    
    uint8_t state;                    
    uintptr_t sp;                       /* Stack pointer */
    uint8_t priority;                
    uint8_t *memory;                    /* Pointer to heap memory used for stack */
    OSHANDLE timer;                   
    volatile OSHANDLE queue;            /* Root entry of the waiting queue. */
};

typedef OSHANDLE os_event_qhld;

typedef struct _os_mutex os_mutex;

struct _os_mutex
{
    OSHANDLE qhld;
    os_thread_info *thread;
    uint16_t count;
};

typedef struct _os_sem os_sem;

struct _os_sem
{
    OSHANDLE qhld;
    short value;
    short valid;
};

typedef struct _os_msg os_msg;

struct _os_msg
{
    uint8_t id;
    int param;
    void *data;
};

typedef struct _os_msgq os_msgq;
typedef struct _os_msgtmr os_msgtmr;

struct _os_msgq
{
    OSHANDLE waitq;
    os_event_qhld postlock;
    os_msgtmr *msgqtmrs;
    os_msgq *next;
    uint8_t read;
    uint8_t write;
    uint8_t mask;
    os_msg msg[1];
};

struct _os_msgtmr
{
    os_msgq *mq;
    int param;
    void *data;
    OSHANDLE qhld;
    os_msgtmr *next;
    uint8_t flags;
};

/*----------------------------------------------------------------*/
/**
 *  \brief Signaled state definition
 *	
 *  The header of empty event queue is set to this value if an event
 *  is posted to this queue .
 */
/*----------------------------------------------------------------*/
#define SIGNALED    ((void *)-1)

#define THREAD_DEFAULT_PRIO     64

#define TDS_TERM        0
#define TDS_RUNNING     1
#define TDS_READY       2
#define TDS_SLEEP       3

#define TIMER_ONESHOT  0x01

#define MSG_TIMER   0xfe
#define MSG_NULL    0xff
#define MSG_NO_WAIT           -1

#define EVENT_WAIT_INFINITE     0
#define EVENT_NO_WAIT           -1

extern os_thread_info *current_thread;
extern os_thread_info *os_thread_list;
extern os_thread_info *run_queue;

extern os_event_qhld dsr_mutex;

/* os assert API */

#define OS_USE_ASSERT

#ifdef OS_USE_ASSERT
#define OS_ASSERT(c)                \
{                                   \
    if(!(c))                              \
    {                                   \
        os_enter_critical();            \
        serial_printf("%s:%d: Fatal: Expected %s in %s\n", __FILE__, __LINE__, #c, __func__); \
        for(;;);                        \
    }                                   \
}                                   
#else
#define OS_ASSERT(c)
#endif

/* os thread API */

#define THREAD(threadfn, arg) \
void threadfn(void *arg) __attribute__ ((noreturn)); \
void threadfn(void *arg)
 
extern OSHANDLE os_thread_create(const char *name, void (*fn) (void *), void *arg, size_t stackSize, int prio);
extern uint8_t os_thread_set_priority(uint8_t level);
extern void os_thread_kill(void);
extern void os_thread_destroy(void);
extern void os_thread_exit(void);
extern void os_thread_resume(void);
extern void os_thread_wake(OSHANDLE td);
extern void os_thread_yield(void);
extern void os_thread_add_queue(os_thread_info * td, os_thread_info * volatile *tqpp);
extern void os_thread_remove_queue(os_thread_info * td, os_thread_info * volatile *tqpp);
#if ((CONFIG_ROM_VER > 1)||defined(CONFIG_FLASH_BOOT))
#define os_thread_switch()	\
		__asm__ __volatile__ ( 	"b.sys       \n\t");
#else
extern void os_thread_switch(void);
#endif
extern void os_enter_critical(void);
extern void os_exit_critical(void);

/* os mutex API */

extern void os_mutex_init(os_mutex *mutex);
extern void os_mutex_lock(os_mutex *mutex);
extern int os_mutex_try_lock(os_mutex *mutex);
extern int os_mutex_unlock(os_mutex *mutex);
extern int os_mutex_destroy(os_mutex *mutex);

/* os semaphore API */

extern void os_semaphore_init(os_sem *sem, short value); 
extern uint32_t os_semaphore_wait(os_sem *sem, uint32_t timeout);

/* os event API */

#define os_event_post_irq(qp)       \
{                                   \
    if (*qp == 0) {                 \
        *qp = SIGNALED;             \
    }                               \
    else if (*qp != SIGNALED) {     \
        os_thread_info *tp = (os_thread_info *)(*qp);    \
        tp->qpec++;              \
    }                               \
}

extern void os_event_timeout(void *arg);
extern int os_event_wait(volatile OSHANDLE *qhp, uint32_t ms);
extern int os_event_wait_next(volatile OSHANDLE *qhp, uint32_t ms);
extern int os_event_post_async(volatile OSHANDLE *qhp);
extern int os_event_post(volatile OSHANDLE *qhp);
//extern int NutEventBroadcastAsync(volatile HANDLE *qhp);
//extern int NutEventBroadcast(volatile HANDLE *qhp);

/* os timer API */

extern OSHANDLE os_timer_start(unsigned int ms, void (*callback)(OSHANDLE, void *), void *arg, unsigned short flags);
extern void os_timer_init(void);
extern void os_timer_stop(OSHANDLE handle);
extern void os_timer_process_elapsed(void);
extern void os_sleep(unsigned int ms);

/* os msgq API */

extern os_msgq *os_msgq_create(uint8_t bits);
extern int os_msgq_broadcast(uint8_t id, int param, void *data);
extern int os_msgq_post(os_msgq *mq, uint8_t id, int param, void *data, int flag);
extern int os_msgq_send(os_msgq *mq, uint8_t id, int param, void *data);
extern int os_msgq_full(os_msgq *mq);
extern int os_msgq_get(os_msgq *mq, os_msg *msg, uint32_t timeout);
extern void os_msgq_flush(os_msgq *mq);
extern int os_msgq_free(os_msgq *mq);

extern void os_msgq_dummy(void);
extern void os_semaphore_dummy(void);
#endif
