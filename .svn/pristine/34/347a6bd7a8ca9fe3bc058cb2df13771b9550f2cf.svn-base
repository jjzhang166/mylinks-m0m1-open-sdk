/*=============================================================================+
|                                                                              |
| Copyright 2015                                                             |
| Montage Inc. All right reserved.                                           |
|                                                                              |
+=============================================================================*/
/*! 
*   \file event.h
*   \brief  Define all event structures
*   \author Montage
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <pt.h>

/*=============================================================================+
| Define                                                                       |
+=============================================================================*/
#define EVENT_MAX_NUM		16


enum {
	/* 0 ~ 7 Reserved for wlan system usage */
	EVT_TBTT_NUM = 0,
	EVT_UART_RX_NUM = 1,
	EVT_TIMER_NUM = 2,
	EVT_WRX_NUM = 3,
	EVT_WTX_NUM = 4,
	EVT_HOST_NUM = 5,
	EVT_UART_CH0_TX_NUM = 6,
	EVT_INIT_NUM = 7,
	EVT_UART_CH1_TX_NUM = 8,
	EVT_UART_CH2_TX_NUM = 9,
	EVT_USB_CDC_TX_NUM = 10,
	/* User can define 8 ~ 15 */
	EVT_DEV_INIT_NUM = 8,
	EVT_UART1_RX_NUM = 11,

	EVT_WTEST_NUM = 14,
	EVT_SDIO_NUM = 15,
};


#define TBTT_EVENT			(1 << EVT_TBTT_NUM)
#define UART_RX_EVENT		(1 << EVT_UART_RX_NUM)
#define UART1_RX_EVENT		(1 << EVT_UART1_RX_NUM)
#define TIMER_EVENT 		(1 << EVT_TIMER_NUM)
#define WRX_EVENT			(1 << EVT_WRX_NUM)
#define WTX_EVENT			(1 << EVT_WTX_NUM)
#define HOST_EVENT			(1 << EVT_HOST_NUM)
#define UART_CH0_TX_EVENT	(1 << EVT_UART_CH0_TX_NUM)
#define UART_CH1_TX_EVENT	(1 << EVT_UART_CH1_TX_NUM)
#define UART_CH2_TX_EVENT	(1 << EVT_UART_CH2_TX_NUM)
#define USB_CDC_TX_EVENT	(1 << EVT_USB_CDC_TX_NUM)

typedef int (*evt_func)(int event_num);

struct event_entry {
	evt_func func;
	struct pt pt;
};

extern struct event_entry evt_table_g[EVENT_MAX_NUM];


#define EVENT_REGISTER(cfunc, num)		\
	do{ \
		((struct event_entry *)(ldev->event_table))[num].func = cfunc; \
	}while(0)\

#define EVENT_UNREGISTER(num)			\
	do{ \
		((struct event_entry *)(ldev->event_table))[num].func = 0; \
	}while(0)\

#define CURRENT_EVENT()		(ldev->current_event)

void raise_event(int event_bit);

