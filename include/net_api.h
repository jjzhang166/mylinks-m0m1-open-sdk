/*=============================================================================+
|                                                                              |
| Copyright 2014                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file   net_api.h
*   \brief  define net API functions
*   \author Montage
*/

#ifndef NET_API_H
#define NET_API_H

void net_ping(unsigned int dip, unsigned int *size,
				unsigned int *iter, unsigned int *to,
				unsigned int *interval);
void net_set_dns(int idx, unsigned int *ipaddr);
char *net_get_dns(int idx, unsigned int *ipaddr);
char *net_get_name(int idx);
int net_get_hostbyname(const char *name, char *ipaddr);
int net_get_client_info(const char *mac);
void net_if_ip_sts(void *data, int type);
void net_drv_init(void);
int net_init_notification(void);
int net_add_notification(int type, void *functionAddress);
int net_del_notification(int type);
int net_del_notification_all(int type);
void net_if_up(int idx, unsigned char dhcp, char *mac,
				unsigned char *_ip, unsigned char *_mask,
				unsigned char *_gw, unsigned char *_dns);
void net_if_down(int idx);
#endif
