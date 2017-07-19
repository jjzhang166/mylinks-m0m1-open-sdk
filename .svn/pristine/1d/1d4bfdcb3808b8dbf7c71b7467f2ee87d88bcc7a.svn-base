/*=============================================================================+
|                                                                              |
| Copyright 2014                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file   socket_api.h
*   \brief  define socket API functions
*   \author Montage
*/

#ifndef SOCKET_API_H
#define SOCKET_API_H
#include <lwip/netif.h>
#include <lwip/sockets.h>

#define NET_MAX_NETIF		3
#define SOCKET_LEN 2048
struct net_dev {
	struct netif netif[NET_MAX_NETIF];
#ifdef CONFIG_SOCKET_API
	int s[MEMP_NUM_NETCONN];
	int type[MEMP_NUM_NETCONN];
	char trans_txb[2048];
	char trans_rxb[2048];
	u16 trans_txlen;
	u16 trans_rxlen;
	struct sockaddr_in localaddr[MEMP_NUM_NETCONN];
	struct sockaddr_in fromaddr[MEMP_NUM_NETCONN];
#endif
	char buf[256];
};


void net_ur_tx(void);
int net_ur_rx(int ev_num);
void net_socket_info(void);
//int net_socket_add(int argc, char *argv[]);
int net_socket_add(int idx, int sockType);
int net_socket_del(int idx);
int net_socket_tx(int idx, char *data,int len);
int net_socket_rx(void);
#endif
