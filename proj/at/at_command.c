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

extern at_funType at_test[];

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


//定义一个升级指令
void at_exeCmdCupdate(uint8_t id){
	char *filename = NULL;
	char type;
	char *buffer = NULL;
	//当路由器连接失败，则返回+ERR=-5
	if(STA_LINK_GET_IP != get_slinkup()){
		at_backOkHead;
		uart0_sendStr("-5");
		at_backTail;
		return;		
	}
	buffer = (char *)malloc(128);
	if(NULL == buffer){
		goto UPERR;
	}
	//根据AT中的命令名来判断是要更新哪个分区中的功能
	if(!memcmp(at_test[id].at_cmdName,"UPGRADE",7)){
		filename = "user.img";
		type = UPDATE_SOFT_TYPE;
	}else{
		filename = "minifs_rom.img";
		type = UPDATE_WEB_TYPE;
	}
	//格式化http中的数据，即升级的url地址
	sprintf(buffer,"118.178.87.170/products/M0M100x/upgrade/AT/Mylinks/001/%s",filename);
	if(!Firmware_WIFIOTAByUrl(type,buffer,80)){
		free(buffer);
		//如果是升级固件，则升启动
		if(type == UPDATE_SOFT_TYPE){
			reboot(1);
		}
		at_backOk;
		return;
	}
	free(buffer);
UPERR:
	at_backErrHead;
	uart0_sendStr("-4");
	at_backTail;
	return;	
}

void at_setupCmdCupdate(uint8_t id,char *pPara){
	char type;
	if(STA_LINK_GET_IP != get_slinkup()){
		at_backOkHead;
		uart0_sendStr("-5");
		at_backTail;
		return;		
	}
	if(!memcmp(at_test[id].at_cmdName,"UPGRADE",7)){
		type = UPDATE_SOFT_TYPE;
	}else{
		type = UPDATE_WEB_TYPE;
	}
	//这部分的url有用户自己在输入AT指令中填写
	if(!Firmware_WIFIOTAByUrl(type,pPara,80)){
		at_backOk;
		return;
	}
	at_backErrHead;
	uart0_sendStr("-4");
	at_backTail;
	return;	
}

at_funType at_test[]={
	{"TEST", 4, at_setupCmdtest, at_exeCmdCtest},
	{"UPGRADE",7,at_setupCmdCupdate,at_exeCmdCupdate},
	{"WUPDATE",7,NULL,at_exeCmdCupdate},
	//自定义的AT指令，必须以以下方式结尾
	{NULL, 0, NULL, NULL},
};



