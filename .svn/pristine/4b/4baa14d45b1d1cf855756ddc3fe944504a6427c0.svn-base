#ifndef LWIPOPTS_H
#define LWIPOPTS_H

extern void free(void *mem);

// Operating System 
#define NO_SYS                      0

#if NO_SYS == 0

#define SYS_LIGHTWEIGHT_PROT        1

#define LWIP_RAW                    1

#define TCPIP_MBOX_SIZE             8
#define DEFAULT_TCP_RECVMBOX_SIZE   8
#define DEFAULT_UDP_RECVMBOX_SIZE   8
#define DEFAULT_RAW_RECVMBOX_SIZE   8
#define DEFAULT_ACCEPTMBOX_SIZE     8

#define TCPIP_THREAD_STACKSIZE      2048
#define TCPIP_THREAD_PRIO           3

#define DEFAULT_THREAD_STACKSIZE    2048

#endif

#define MEM_LIBC_MALLOC				1
#define MEMP_MEM_MALLOC             1

// 32-bit alignment
#define MEM_ALIGNMENT               4

#define PBUF_POOL_SIZE              5
#define MEMP_NUM_TCP_PCB_LISTEN     4
#define MEMP_NUM_TCP_PCB            4
#define MEMP_NUM_PBUF               8

#define TCP_QUEUE_OOSEQ             1
#define TCP_OVERSIZE                0

#ifdef CONFIG_DHCPC
#define LWIP_DHCP                   1
#endif
#ifdef CONFIG_DNS
#define LWIP_DNS                    1
#endif
#ifndef CONFIG_FRAG
#define IP_REASSEMBLY               0
#define IP_FRAG                     0
#endif

// Support Multicast
#define LWIP_IGMP                   1
#define LWIP_RAND()                 arc4random()

#define LWIP_COMPAT_SOCKETS         1
#define LWIP_POSIX_SOCKETS_IO_NAMES 1
#define LWIP_SO_RCVTIMEO            1
#define LWIP_TCP_KEEPALIVE          1

// Debug Options
#define LWIP_DEBUG
#define UDP_LPC_EMAC                LWIP_DBG_OFF
#define SYS_DEBUG                   LWIP_DBG_OFF
#define PPP_DEBUG                   LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define MEM_DEBUG                   LWIP_DBG_OFF
#define MEMP_DEBUG                  LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define API_LIB_DEBUG               LWIP_DBG_OFF
#define API_MSG_DEBUG               LWIP_DBG_OFF
#define TCPIP_DEBUG                 LWIP_DBG_OFF
#define SOCKETS_DEBUG               LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_OFF
#define TCP_INPUT_DEBUG             LWIP_DBG_OFF
#define TCP_FR_DEBUG                LWIP_DBG_OFF
#define TCP_RTO_DEBUG               LWIP_DBG_OFF
#define TCP_CWND_DEBUG              LWIP_DBG_OFF
#define TCP_WND_DEBUG               LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
#define TCP_RST_DEBUG               LWIP_DBG_OFF
#define TCP_QLEN_DEBUG              LWIP_DBG_OFF
#define ETHARP_DEBUG                LWIP_DBG_OFF
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define DHCP_DEBUG                  LWIP_DBG_OFF
#define DNS_DEBUG                   LWIP_DBG_OFF

#ifdef LWIP_DEBUG
#define MEMP_OVERFLOW_CHECK         1
#define MEMP_SANITY_CHECK           1
#else
#define LWIP_NOASSERT               1
#define LWIP_STATS                  0
#endif

#define LWIP_PLATFORM_BYTESWAP      1


/* MSS should match the hardware packet size */
#define TCP_MSS                     1460
//#define TCP_SND_BUF                 (2 * TCP_MSS)
//#define TCP_WND                     (2 * TCP_MSS)
//#define TCP_SND_QUEUELEN            (2 * TCP_SND_BUF/TCP_MSS)

/* Minimal changes to opt.h required for tcp unit tests: */
#define MEM_SIZE                        16000
#define TCP_SND_QUEUELEN                40
#define MEMP_NUM_TCP_SEG                TCP_SND_QUEUELEN
#define TCP_SND_BUF                     (12 * TCP_MSS)
#define TCP_WND                         (10 * TCP_MSS)

// Broadcast
#define IP_SOF_BROADCAST            0
#define IP_SOF_BROADCAST_RECV       0

#define LWIP_BROADCAST_PING         1

#define LWIP_CHECKSUM_ON_COPY       0

#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1



#endif /* LWIPOPTS_H_ */
