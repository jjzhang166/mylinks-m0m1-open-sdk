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
#include <iot_aliyun.h>
/*=============================================================================+
| Define                                                                       |
+=============================================================================*/



/*----------------------------------------------------------------*/
/**
 * 此例中使用没有无SSL连接方式,请在查看代码前做以下工作:
 * 将include文件夹中的“#define CONFIG_AXTLS 1”注释
 * make lib，重新编译mqtt.a静态库
 * make mqtt进行例程编译，生成mqtt.img文件
 */
/*----------------------------------------------------------------*/

#define SSID "Mylinks"
#define PWD	 "welcometomylinks"

#define TESTSTR "Hello,IOT Aliyun"
//The product and device information from IOT console
#define PRODUCT_KEY         "7MAqltJKJYR"
#define DEVICE_NAME         "M0M1_123456"
#define DEVICE_SECRET       "e6HPIjeD3QQsHTBYms1CSx8uieXjfKXe"
#define KEEPALIVE	180


//This is the pre-defined topic
#define TOPIC_UPDATE         "/"PRODUCT_KEY"/"DEVICE_NAME"/update"
#define TOPIC_ERROR          "/"PRODUCT_KEY"/"DEVICE_NAME"/update/error"
#define TOPIC_GET            "/"PRODUCT_KEY"/"DEVICE_NAME"/get"

#define IOTALIYUN_ADDR		 ""PRODUCT_KEY".iot-as-mqtt.cn-shanghai.aliyuncs.com"

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

//串口接收回调函数
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

//接收数据回调函数
void IOTAliyunRecvData(MessageData* md)
{
	MQTTMessage* message = md->message;
	MQTTString * topicName = md->topicName;
	int i;

	if (strncmp(topicName->lenstring.data, TOPIC_GET, topicName->lenstring.len) == 0)
	{
		uart0_tx_buffer(message->payload,message->payloadlen);
	}
	return;
}

//发布数据函数
int IOTAliyunSendData(void)
{

	MQTTMessage message;
	/* Send message */
	message.payload = TESTSTR;
	message.payloadlen = strlen(TESTSTR);

	message.dup = 0;
	message.qos = QOS1;
	message.retained = 0;

	return IOTAliyunPublish(TOPIC_UPDATE, &message);
}







//MQTT处理任务
static void  iotaliunclient( void *arg )
{
	int rc = FAILURE;
	for(;;){

		//等待STA获取IP地址
		if(get_slinkup() != STA_LINK_GET_IP){
			goto ALIYUN_ERR;
		}
		
		if(rc == FAILURE){
			//开始阿里云物联网套件的连接
			rc = IOTAliyunStart(IOTALIYUN_ADDR, 1883);
			//如果rc创建失败,则继续创建
			if(rc == FAILURE){
				uart0_sendStr("IOTAliyun connect fail\r\n");
				IOTAliyunDeinit();
				goto ALIYUN_ERR;
			}else{
				//创建成功，则发布订阅号
				rc = IOTAliyunSubscribe(TOPIC_GET, QOS1, IOTAliyunRecvData);
				if(rc == FAILURE){
					uart0_sendStr("IOTAliyun subscribe fail\r\n");
					IOTAliyunDeinit();
					goto ALIYUN_ERR;
				}
			}			
		}

		//向Aliyun物联网套件发送数据
#if 0
		if(IOTAliyunSendData() < 0){
			//用户可自行处理，这里为断开后再次连接
			IOTAliyunDeinit();
			goto ALIYUN_ERR;
		}
#endif
		//Aliyun物联网套件 keepactive 重连接处理等处理，用户可以不用理会此处设置延时1000ms
		rc = IOTAliyunYield(1000);
		if (rc == FAILURE){
			uart0_sendStr("IOTAliyun yield fail\r\n");
			//用户可自行处理，这里为断开后再次连接
			IOTAliyunDeinit();
			continue;
		}
ALIYUN_ERR:
		if(rc != FAILURE){
			rc =  FAILURE;
			IOTAliyunDeinit();
		}
		//此任务循环时间为500ms
		sys_msleep(500);
	}
exit:
    vTaskDelete(NULL);
	return;
}



void user_init(void){
	struct station_config s;
	uart_init();
	//注册一个串口0的接收任务进行数据接收
	uart0_rev_register(test_uart0_rev);
	//设置模块为STA工作模式
	wifi_set_opmode(OPMODE_STA);
	//读取当前模块的STA配置信息
	wifi_station_set_config(&s);
	if(strcmp(s.ssid,SSID) ||
		strcmp(s.password,PWD)){

		memset(&s,0,sizeof(s));
		//设置连接的路由器ssid
		strcpy(s.ssid,SSID);
		//设置连接的路由器密码
		strcpy(s.password,PWD);
		wifi_station_set_config(&s);
	}
	IOTAliyunDataInit(PRODUCT_KEY,DEVICE_NAME,DEVICE_SECRET,KEEPALIVE);
	xTaskCreate(iotaliunclient, "iotaliyun", TASK_HEAP_LEN, 0, 5, NULL);
	return;
}



