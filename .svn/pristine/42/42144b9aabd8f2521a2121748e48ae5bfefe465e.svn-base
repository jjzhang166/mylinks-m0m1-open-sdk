#include <lwip/sockets.h>

#define OMNI_MODE_UNKNOWN		0
#define OMNI_MODE_AP			1
#define OMNI_MODE_WISP			3
#define OMNI_MODE_STA			9

#define OMNI_VENDOR_ID			"FFFF"//"0000"
#define OMNI_PRODUCT_ID			"FFFF"//"0067"

#define OMNI_VERSION			1
#define OMNI_PKT_MAX_LENGTH		180

#define OMNI_DEST_PORT			5354
#define OMNI_BROADCAST_ADDR		IPADDR_BROADCAST // 255.255.255.255

#define OMNI_SEND_TIME_GAP		5

#define printf(fmt,args...)		serial_printf(fmt ,##args)

static int omni_fd = (-1);
static char *omni_pkt = 0;

static void release_socket(void)
{
	if (-1 == omni_fd)
		return;
	
	//printf("Release Socket.\r\n");
	lwip_close(omni_fd);
	omni_fd = -1;
}

static int send_message(int fd, const void *data, size_t size)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = OMNI_BROADCAST_ADDR; //inet_addr("255.255.255.255");
	addr.sin_port = htons(OMNI_DEST_PORT);
	
	return lwip_sendto(fd, data, size, 0, (struct sockaddr *)&addr, sizeof(addr));
}

static int create_socket(void)
{
	int ret = -1;
	int b_opt_val = 1;
	omni_fd = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	if (-1 == omni_fd) {
		printf("Fail to create LWIP UDP socket.\r\n");
		return ret;
	}
	
	if (0 != lwip_setsockopt(omni_fd, SOL_SOCKET, SO_BROADCAST, &b_opt_val, sizeof(int))) {
		printf("Fail to LWIP setopt Broadcast.\r\n");
		goto exit;
	}
	
	ret = 0;
	return ret;
	
exit:
	release_socket();
	return ret;
}

static int send_broadcast_packet(char* buffer)
{
	int cnt = 0;
	int ret = -1;
	
	if (-1 == omni_fd) {
		if (0 != create_socket()) {
			printf("Create_socket Fail.\r\n");
			return ret;
		}
	}
	
	cnt = send_message(omni_fd, buffer, strlen(buffer));
	
	if (0 > cnt) {
		printf("Sendto fail.\r\n");
	} else {
		ret = 0;
		//printf("Broadcast Cnt %d, Buffer <%d>%s.\r\n", cnt, strlen(buffer), buffer);
	}

	release_socket();
	return ret;
}

static void sent_omnicofig_broadcast(int result, char* ipaddr, char* mac_str, int new)
{
	int new_conf ;
	int time_ver = sys_now() ;
	
	if (new) {
		new_conf = 1 ;
		//printf("OMNI String : ");
	} else {
		new_conf = 0 ;
		//printf("End OMNI String : ");
	}
	
	sprintf(omni_pkt, "{\"ver\":\"%d\",\"name\":\"OMNICFG@%s\",\"omi_result\":\"%d\",\"sw_vid\":\"%s\",\"sw_pid\":\"%s\",\"lan_ip\":\"%s\",\"version\":\"%d\",\"new_conf\":\"%d\"}",
		OMNI_VERSION,mac_str,result,OMNI_VENDOR_ID,OMNI_PRODUCT_ID,ipaddr,time_ver,new_conf);
	
	send_broadcast_packet(omni_pkt);
	//printf("%s \r\n",omni_pkt);
	
	return;
}

static void omnicfg_broadcast_stop(void)
{
	if (NULL != omni_pkt) {
		free(omni_pkt);
		omni_pkt = NULL;
	}
		
	return;
}

static int omnicfg_broadcast_start(void)
{
	if (NULL != omni_pkt)
		return (-1);
	
	omni_pkt = (char *)malloc(OMNI_PKT_MAX_LENGTH);
	if (NULL == omni_pkt)
		return (-1);
	
	memset(omni_pkt, 0, OMNI_PKT_MAX_LENGTH);
	return 0;
}

int omnicfg_broadcast(int result, char* ipaddr, char* mac_str, int new)
{
	if (0 > omnicfg_broadcast_start())
		return (-1);

	if (NULL == omni_pkt) {
		printf("Error : Broadcast Packet Memory not Allocated!\r\n");
		return (-1);
	}
	
	sent_omnicofig_broadcast(result, ipaddr, mac_str, new);
	
	omnicfg_broadcast_stop();
	return 0;
}

