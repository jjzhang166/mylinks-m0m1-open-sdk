


#include "MQTTClient.h"
#include "aes.h"
#include "md5.h"  


#define READ_BUF_SIZE 1000
#define SEND_BUF_SIZE 1000


static MQTTClient c;
static Network n;


static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];
static int mqtt_ready;
static int topic_info_type; /* determined by uart A1 command response */

#define MQTT_SERVER "mfl.51acloud.com"
#define MQTT_PORT  61613
#define MQTT_USER "2016dev@Cloud";
#define MQTT_PASS "2016dev@Passw)rd";
#define WIFI_CLOUD_CONNECTED          (1<<5)
#define WIFI_CLOUD_DISCONNECTED          (1<<14)



#define MD5_SALT "Y2016-10-24Y"
//#define ENABLE_CRYPT 1
static uint8_t aes_key[16];
static uint8_t aes_iv[16];
//uint8_t aes_key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
//uint8_t aes_iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };



/* must be global, it is also used as receive filter*/
static uint8_t subscribe_ack_topic[100] = {0};			//dev/ack/@MAC
static uint8_t publish_reg_topic[100] = {0}; 			//dev/reg/@MAC
static uint8_t subscribe_type_topic[100] = {0}; 		//dev/cmd/@Type/@MAC
static uint8_t publish_type_topic[100] = {0}; 		//dev/status/@Type/@MAC


#define printf serial_printf
unsigned char * get_mac_addr(void);

/*
	Judge if it is A1 command Passed from UART.
	This is the special command when boot.
	
	If it is, it needs to be sent/receive through the link "dev/status/@MAC"
*/
int mcu_a1_cmd(unsigned char *cmd_buffer)
{
	if ((cmd_buffer[0] == 0xAC) && (cmd_buffer[1] == 0xAC) && (cmd_buffer[3] == 0xA1)) {
		printf("A1 command detected!\n");
		return 1;
	} else {
		printf("A1 command undetected!\n");
		return 0;
	}
}


/*
 *	for example, turn 0x06 to '6', turn 0x0a to 'a'
 *
 *
*/
unsigned char hex_to_ascii(unsigned char ascii)
{

	if ((ascii >= 0x00) && (ascii <= 0x09))
		ascii = ascii + '0';
	
	if ((ascii >= 0x0a) && (ascii <= 0x0f))
		ascii = ascii -0x0a + 'a';

	return ascii;
}


int MQTT_set_topic_info(int topic_info_type_in)
{
	topic_info_type = topic_info_type_in;
}





int MQTT_refresh_topic()
{
	char mac_addr_fake[] = "888888888888";
	char *mac_addr = get_mac_addr();
	if (NULL == mac_addr) {
		printf("get mac address failed, using default mac address\n");
		mac_addr = mac_addr_fake;
	}

	sprintf(subscribe_type_topic, "dev/cmd/%06x/%s", topic_info_type, mac_addr);
	sprintf(subscribe_ack_topic, "dev/ack/%s", mac_addr);
	sprintf(publish_type_topic, "dev/status/%06x/%s", topic_info_type, mac_addr);
	sprintf(publish_reg_topic, "dev/reg/%s", mac_addr);

	printf("Refreshed topic=%s, %s, %s, %s\n", subscribe_ack_topic, publish_reg_topic, subscribe_type_topic, publish_type_topic);

}




void MQTT_recv_data(MessageData* md)
{
	MQTTMessage* message = md->message;
	MQTTString * topicName = md->topicName;
	char* input_buffer = NULL;
	unsigned char* output_buffer = NULL;
	int aes_length = 0;
	int rc = -1,i;

	printf("================> messageArrived\n");

	for(i = 0;i < message->payloadlen;i++)
		printf("%02x ",*((unsigned char*)message->payload+i));
	printf("\n");


#if ENABLE_CRYPT

	/* do aes progress */
	aes_length = message->payloadlen/16*16 + ((message->payloadlen % 16) ? 16 : 0);
	printf("message length = %d, AES length = %d\n", message->payloadlen, aes_length);
	input_buffer = malloc(aes_length);
	output_buffer = malloc(aes_length);
	memset(input_buffer, 0, aes_length);
	memset(output_buffer, 0, aes_length);
	memcpy(input_buffer, message->payload, message->payloadlen);
	AES128_CBC_decrypt_buffer(output_buffer, input_buffer,  aes_length, aes_key, aes_iv);
	//printf("Got Message=%s\n", output_buffer);
	printf("MQTT Send to UART\n", output_buffer);


	if (0 == checkIsOTAData(output_buffer,aes_length)) {
		parseOTAData(output_buffer,aes_length);
		goto out_free;
	}

	if (mcu_a1_cmd(output_buffer)) {
		set_mcu_info_status(0);
	}
	else
	{
	if(output_buffer[2] > aes_length)
	aes_length = aes_length;
	else
	aes_length = output_buffer[2]+3;
	Local_SendData(0, output_buffer, aes_length);
	}

#else
	printf("MQTT Send to UART\n");
	Local_SendData(0, message->payload, message->payloadlen);
#endif


out_free:
	if (input_buffer)
		free(input_buffer);
	if (output_buffer)
		free(output_buffer);

//	printf("Topic Name1 = %s\n", topicName->cstring);
//	printf("Topic Name2 =%.*s\n", topicName->lenstring.len, topicName->lenstring.data);
}



int MQTT_send_data(unsigned char *data, int length)
{


	/*
		generate AES CBC encrypt block data
		input is aligned to 16bytes, and the output size is same as input.

	*/

	unsigned char* input_buffer = NULL;
	unsigned char* aes_buffer = NULL;
	int aes_length = 0;
	int rc = -1,i = 0;


	if (!mqtt_ready)
		goto out_free;

	if (!c.isconnected)
		goto out_free;

	/* do aes progress */
	aes_length = length/16*16 + ((length % 16) ? 1 : 0)*16;
	printf("message length = %d, AES length = %d\n", length, aes_length);
	input_buffer = malloc(aes_length);
	aes_buffer = malloc(aes_length);
	memset(input_buffer, 0, aes_length);
	memset(aes_buffer, 0, aes_length);
	memcpy(input_buffer, data, length);
	AES128_CBC_encrypt_buffer(aes_buffer, input_buffer, aes_length, aes_key, aes_iv);
	/* Send message */
	MQTTMessage message;
	message.payload = aes_buffer;
	message.payloadlen = aes_length;

	message.dup = 0;
	message.qos = QOS2;
	message.retained = 0;

	MQTT_refresh_topic();
	if (mcu_a1_cmd(data))
		rc = MQTTPublish(&c, publish_reg_topic, &message);
	else
		rc = MQTTPublish(&c, publish_type_topic, &message);
	

	printf("Publish rc=%d\n", rc);


out_free:
	if (input_buffer)
		free(input_buffer);
	if (aes_buffer)
		free(aes_buffer);
	return rc;
}


int MQTT_deinit()
{

	MQTTDisconnect(&c);
	MQTTClientDeinit(&c);
	FreeRTOS_closesocket(&n);
}


int MQTT_init()
{
	int rc = 0;
	unsigned char * mac_addr = NULL;

	FreeRTOS_NetworkInit(&n);
	rc = FreeRTOS_NetworkConnect(&n, MQTT_SERVER, MQTT_PORT);
	printf("NetworkConnect ret = %d\n", rc);
	if (rc < 0) {
		rc = FAILURE;
		goto exit;
	}
	MQTTClientInit(&c, &n, 2000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));


	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	mac_addr = get_mac_addr();
	if(mac_addr == NULL) {
		rc = FAILURE;
		MQTT_deinit();
		goto exit;
	}

	/* 
		since we have got the mac address
		we can generate the AES key.
	*/
	//MQTT_gen_AES_key(mac_addr);
	
	data.clientID.cstring = mac_addr;
	data.willFlag = 0;
	data.MQTTVersion = 3;
	data.username.cstring =  MQTT_USER;
	data.password.cstring = MQTT_PASS;
	data.keepAliveInterval = 5; //5 second
	data.cleansession = 1;

	rc = MQTTConnect(&c, &data);
	printf("MQTTConnect ret = %d\n", rc);
	if (rc == FAILURE) {
		MQTT_deinit();
		goto exit;
	}
	
	MQTT_refresh_topic();
	rc = MQTTSubscribe(&c, subscribe_ack_topic, QOS2, MQTT_recv_data); 
	printf("MQTTSubscribe ret = %d\n", rc);
	if (rc == FAILURE) {
		MQTT_deinit();
		goto exit;
	}

	MQTT_refresh_topic();
	rc = MQTTSubscribe(&c, subscribe_type_topic, QOS2, MQTT_recv_data); 
	printf("MQTTSubscribe type ret = %d\n", rc);
	if (rc == FAILURE) {
		MQTT_deinit();
		goto exit;
	}

exit:
	return rc;
}

#if 0
void MQTT_test(void *arg)
{
	MQTT_send_data("acde", sizeof("acde"));

	while (1) {
		mallinfo(0, 0);
		MQTT_send_data("acdefff", sizeof("acdefff"));
		sys_msleep(1000);
	}
}



int MQTT_action_test()
{
	int i = 0;
	
	sys_msleep(1000*10); 
	while (1) {
		printf("MQTT Loop starts\n"); 
		while (MQTT_init() == FAILURE) { 
			sys_msleep(1000);	
		}
		printf("MQTT Loop starts end\n"); 

		//sys_msleep(1000*1); 

		printf("MQTT Loop Yield\n"); 
		/*
		while (1) {
			if (FAILURE == MQTTYield(&c, 1000))
				break;
		}
		*/
		MQTTYield(&c, 10);
		MQTTYield(&c, 10);
		MQTTYield(&c, 10);
		MQTTYield(&c, 10);
		MQTTYield(&c, 10);
		printf("MQTT Loop Yield end\n"); 


		//sys_msleep(1000*10); 
		printf("MQTT deinit\n");
		MQTT_deinit();
		printf("MQTT deinit end\n");
		sys_msleep(1000*1);	

	}

	return 0;


}

#endif






#if 0
int MQTT_gen_AES_key(char *mac_addr)
{
	int i = 0;
	int j = 0;
	unsigned char MD5_input[100] = {0};
	unsigned char MD5_output_buffer[16];

	printf("MAC address is %s\n", mac_addr);
	printf("MD5 Salt is %s\n", MD5_SALT);


	MD5_input[0] = mac_addr[0];
	MD5_input[1] = mac_addr[2];
	MD5_input[2] = mac_addr[4];
	MD5_input[3] = mac_addr[6];
	MD5_input[4] = mac_addr[8];
	MD5_input[5] = mac_addr[10];

	/* assume MD5_INIT_SALT is a string that with even numbers*/
	for (i = 0; i < strlen(MD5_SALT); i = i+2) {
		MD5_input[6 + i] = MD5_SALT[i+1];
		MD5_input[6 + i+1] = MD5_SALT[i];
	}
	printf("MD5 Input is :%s\n",MD5_input);  

	md5_string(MD5_input, MD5_output_buffer);

	printf("MD5 Result is:");  
	for(i=4;i<12;i++) {
		printf("%02x",MD5_output_buffer[i]);
	} 
	printf("\n");


	j = 0;
	for(i=4;i<12;i++) {
		aes_key[j++] = ((MD5_output_buffer[i] & 0xF0) >> 4);
		aes_key[j++] = MD5_output_buffer[i] & 0x0F;
	} 

	j = 0;
	for(i=4;i<12;i++) {
		aes_iv[j++] = ((MD5_output_buffer[i] & 0xF0) >> 4);
		aes_iv[j++] = MD5_output_buffer[i] & 0x0F;
	} 

	for (i = 0; i < 16; i++) {
		aes_key[i] = hex_to_ascii(aes_key[i]);
		aes_iv[i] = hex_to_ascii(aes_iv[i]);
	}


	printf("AES Key is:");  
	for (i = 0; i < 16; i++) {
		printf("%c",aes_key[i]);
	}
	printf("\n");

	printf("AES IV is:");  
	for (i = 0; i < 16; i++) {
		printf("%c",aes_iv[i]);
	}
	printf("\n");


}
#endif



int MQTT_action()
{

	while (1) {
		printf("MQTT Loop starts\n"); 
		//mallinfo(0, 0);

		while (MQTT_init() == FAILURE) { 
			sys_msleep(1000);	 /* time to free lwip memory */
		}

		printf("MQTT init success\n");

		mqtt_ready = 1;
		//GAgent_DevCheckWifiStatus(WIFI_CLOUD_CONNECTED, 1);

		while (1) {
			 /*10 ms to faster the MQTT_send_data process */
			if (FAILURE == MQTTYield(&c, 10))
				break;
		}
		printf("MQTT Failed\n");

		//GAgent_DevCheckWifiStatus(WIFI_CLOUD_CONNECTED, 0);
		mqtt_ready = 0;

		MQTT_deinit();
		printf("MQTT deinit\n");

		sys_msleep(3*1000);  /* time to free lwip memory */
		//sys_msleep(300*1000);  /* time to free lwip memory */

	}

	return 0;
}


