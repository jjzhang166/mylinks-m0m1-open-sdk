/*=============================================================================+
|                                                                              |
| Copyright 2015                                                             |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file app_init.c
*   \brief main entry
*   \author Montage
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <c_types.h>
#include <stdint.h>
#include <event.h>
#include <common.h>
#include <os_api.h>
#include <net_api.h>
#include <wla_api.h>
#include <cfg_api_new.h>
#include <gpio.h>
#include <version.h>
#include <built_info.h>
#include <omniconfig.h>

#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#if defined(CONFIG_LWIP)
#include <net_api.h>
#endif
#include <user_config.h>
#include <mylinks_wifi.h>
#include <lwip/sockets.h>
/*=============================================================================+
| Define                                                                       |
+=============================================================================*/



/*----------------------------------------------------------------*/
/**
 * The function is called once application start-up. Users can initial
 * structures or global parameters here.
 *
 * @param None.
 * @return int Protothread state.
 */
/*----------------------------------------------------------------*/
int app_main(void)
{
#ifdef CONFIG_WLA_LED
	pin_mode(WIFI_LED_PIN, 1);
	digital_write(WIFI_LED_PIN, 0);
#endif
	/* Do not add any process here, user should add process in user_thread */
	hw_sys_init();
	return PT_EXITED;
}


static char *testStr = "Only Test";


static void  tcpclient( void *arg )
{
	int fd;
	int ret;
	int len;
	struct sockaddr_in *tcp_from;
	uint8_t *buf = NULL;
	//1 s超时收数据
	int nNetTimeout = 1000;
	//等待获取IP地址
	while(get_slinkup() != STA_LINK_GET_IP){
		sys_msleep(10);
	}
	tcp_from = (struct sockaddr_in *)zalloc(sizeof(struct sockaddr_in));
	fd = socket(PF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		goto exit;
	}
	memset(tcp_from,0, sizeof(struct sockaddr_in));
	tcp_from->sin_family = AF_INET;
	//连接tcp 服务器的端口号
	tcp_from->sin_port = htons(8080);
	//连接tcp 服务器的IP地址
	tcp_from->sin_addr.s_addr = inet_addr("192.168.1.100");
	ret = connect(fd,(struct sockaddr *)tcp_from,sizeof(struct sockaddr));
	if(0 != ret){
		goto exit;		
	}
	buf = (uint8_t *)malloc(256);
	for(;;){
		if(strlen(testStr) == write(fd,testStr,strlen(testStr))){
			//发送成功;
		}
		//设置接收超时时间
		setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));
		len = read(fd,buf,256);
		if(len <= 0){
			//此时已经断开了tcp的连接，需要重新连接tcp.
			ret = connect(fd,(struct sockaddr *)tcp_from,sizeof(struct sockaddr));
			continue;
		}
		//接收到的数据长度为len，在下面做处理。
		
	}
exit:
    vTaskDelete(NULL);
	return;
}





void user_init(void){
	struct station_config s;
	wifi_set_opmode(OPMODE_STA);
	memset(&s,0,sizeof(s));
	strcpy(s.ssid,"ssid");
	strcpy(s.password,"password");
	wifi_station_set_config(&s);
	xTaskCreate(tcpclient, "client", TASK_HEAP_LEN, 0, 5, NULL);
	return;
}



