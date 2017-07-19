#ifndef __PING_H__
#define __PING_H__

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif

extern unsigned int ping_dip;
extern unsigned int ping_data_size;
extern unsigned int ping_rcv_timeo;
extern unsigned int ping_iteration;
extern unsigned int ping_interval;
extern unsigned int ping_tx;
extern unsigned int ping_rx;

void ping_init(void);

#if !PING_USE_SOCKETS
void ping_send_now(void);
#endif /* !PING_USE_SOCKETS */

#endif /* __PING_H__ */
