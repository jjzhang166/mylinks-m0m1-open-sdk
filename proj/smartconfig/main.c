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

static int tcp_client_connect(void){
	int fd = -1;
	struct sockaddr_in tcp_from;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(fd == -1){
		goto TCP_ERR;
	}
	memset(&tcp_from,0, sizeof(struct sockaddr_in));
	tcp_from.sin_family = AF_INET;
	//设置连接tcp 服务器的端口号
	tcp_from.sin_port = htons(8080);
	//设置连接tcp 服务器的IP地址
	tcp_from.sin_addr.s_addr = inet_addr("192.168.1.141");
	if(connect(fd,(struct sockaddr *)&tcp_from,sizeof(struct sockaddr))){
		close(fd);
		fd = -1;
	}
TCP_ERR:
	return fd;
}


static void  tcpclient( void *arg )
{
	int fd = -1;
	int len;
	int ret = 0;
	uint8_t *buf = NULL;
	//1 s超时收数据
	struct timeval tv;
	fd_set fdsr;
	buf = (uint8_t *)malloc(256);

	for(;;){
		//此任务循环时间为50ms
		sys_msleep(50);
		//等待STA获取IP地址
		if(get_slinkup() != STA_LINK_GET_IP){
			continue;
		}
		//如果fd为-1,则创建一个tcp 客户端
		if(fd == -1){
			fd = tcp_client_connect();
		}
		//如果fd创建失败,则继续创建
		if(fd == -1){
			continue;
		}
		//发送数据，如果发送数据返回与实际的发送的长度相同,则说明发送成功
		if(strlen(testStr) == write(fd,testStr,strlen(testStr))){
			//从串口0发送字符串
			uart0_sendStr("Send:");
			uart0_sendStr(testStr);
			uart0_sendStr(" OK\r\n");
			//发送成功;
		}
		//清空fdsr句柄
		FD_ZERO(&fdsr);
		//监控fd读句柄发生的变成，将fd加入到fdsr中进入监控
		FD_SET(fd, &fdsr); 
		//超时1秒读取读操作监控超时时间为1秒
	    tv.tv_sec = 1;  
	    tv.tv_usec = 0; 
	    //开始监控
		ret = select(fd + 1, &fdsr, NULL, NULL, &tv);
		/*
			ret:<0,监控出错
				=0,超时
				>0,读操作有变化
		*/
		if (ret <= 0)  
		{
			continue;  
		}

		//将数据读入buf中，长时为256
		len = read(fd,buf,256);
		//如果读取的长度为<=0,则说明读取数据返回值为不正确操作
		if(len <= 0){
			//此时已经断开了tcp的连接，需要重新连接tcp.
			close(fd);
			fd = -1;
			continue;
		}
		//接收到的数据长度为len，在下面做处理。
		
	}
exit:
    vTaskDelete(NULL);
	return;
}


void uart_init(void)
{
	/*
	设置串口参数：
	参数1->波特率：115200
	参数2->校验位：无
	参数3->停止位：1位
	参数4->串口号：0号串口
	*/
	serial_conf(baudrate_select(115200), 0, 1, 0,0 );
	serial_init(0);

}


void test_uart0_rev( void * arg){
	uint8_t temp;
	extern struct serial_buffer *ur0_rxbuf;
	for(;;){
		//等待串口是否有数据传入
		if(0 != uart_recv_sem_wait(portMAX_DELAY)){
			continue;
		}
		//判断串口数据是否为空
		while(serial_buffer_empty(ur0_rxbuf)){
			//读取一个字节的串口数据
			temp = serial_buffer_getchar(ur0_rxbuf);
		}
	}
}

#define STA_SSID "Mylinks"
#define STA_PWD "welcometomylinks"

#define AP_SSID "AP_Test"
#define AP_PWD "TEST123456"



void user_init(void){
	struct station_config sta;
	struct softap_config ap;
	uart_init();
	//注册一个串口0的接收任务进行数据接收
	uart0_rev_register(test_uart0_rev);
	//设置模块为AP+STA工作模式
	//wifi_set_opmode(OPMODE_STA);
	wifi_set_opmode(OPMODE_APSTA);
	//开机设置为直接进入smartconfig配网模式
	smartconfig_start(60);
	xTaskCreate(tcpclient, "client", TASK_HEAP_LEN, 0, 5, NULL);
	return;
}



