/*=============================================================================+
|                                                                              |
| Copyright 2017                                                               |
| Mylinks Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file at_command_demo.c
*   \brief main entry
*   \author Mylinks
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <stdint.h>
#include <common.h>
#include <os_api.h>
#include <cfg_api_new.h>
#include <user_config.h>


/*AT查询，此函数将在串口输入AT+TEST执行
下例中，执行结果为+ok=Mylinks */
static void at_exeCmdCtest(uint8_t id){
	at_backOkHead;
	uart0_sendStr("Mylinks");
	at_backTail;
	return;
}


/*AT设置功能，此函数将在串口输入AT+TEST=Mylinks执行
  当输入AT+TEST=mylinks,*pPara指向"Mylinks",即pPara="Mylinks"
  用户可以自行进行处理,以下例子实现了:
  当AT指令输入AT+Mylinks时，串口回复:+ok,否者输出:+ERR=-1 */
static void at_setupCmdtest(uint8_t id, char *pPara){
	if(!strcmp(pPara,"Mylinks")){
		at_backOk;
	}else{
		at_backError;
		uart0_sendStr("-1");
	}
	return;
}

at_funType at_test[]={
	{"TEST", 4, at_setupCmdtest, at_exeCmdCtest},
	//自定义的AT指令，必须以以下方式结尾
	{NULL, 0, NULL, NULL},
};



