#ifndef __IOT_ALIYUN_H
#define __IOT_ALIYUN_H
#include <MQTTClient.h>

/*函数功能：
    初始化阿里云物联网套件环境变量
    char *produt_key:对应PRODUCT_KEY，在物联网套件中“创建产品”时生成
    char *dev_name:对应DEVICE_NAME，在物联网套件中“创建产品”后，在对应的产品中“添加设备”
    char *dev_secret:对应DEVICE_SECRET，点击设备查看“deviceSecret”
    keepalive:与阿里云物联网套件心跳包，默认180，单位秒。应该大于等于180
*/
void IOTAliyunDataInit(char *produt_key,char *dev_name,char *dev_secret,uint16_t keepalive);




/*函数功能：
    异常处理释放阿里云物联网套件环境变量
*/
void IOTAliyunDeinit(void);




/*函数功能：
    向阿里云物联网套件发布一条消息
    const char* topicName:发布号
    MQTTMessage* message:MQTT发送结构体
*/
int IOTAliyunPublish(const char* topicName, MQTTMessage* message);




/*函数功能：
    向阿里云物联网套件订阅一个主题
    const char* topicFilter:订阅号
    enum QoS qos:只支持QOS0和QOS1
    messageHandler messageHandler:接收到数据的回调函数
*/
int IOTAliyunSubscribe(const char* topicFilter, enum QoS qos, messageHandler messageHandler);




/*函数功能：
    连接阿里云物联网套件
    char* addr:阿里云物联网套件的域名
    int port：阿里云物联网套件的端口号，默认为1883
*/
int IOTAliyunStart(char* addr, int port);




/*函数功能：
    与阿里云物联网套件保持通信的功能函数
    int timeout_ms:阻塞任务的时间，单位：毫秒
*/
int IOTAliyunYield(int timeout_ms);


#endif

