#ifndef __WEBSERVER_H
#define __WEBSERVER_H

#define CONTENT_LEN 128
#define BUF_LEN 512
#define METHOD_LEN 192
#define URL_LEN METHOD_LEN


#define STRSEND(fd,str) send(fd,str,strlen(str),0)

#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: Mylinks/0.1.0\r\n"


typedef struct _DNSHEAD{        //dns 头部  
    uint16_t ID;  
    uint16_t tag; // dns 标志(参数)  
    uint16_t numQ;        // 问题数  
    uint16_t numA;        // 答案数  
    uint16_t numA1;       // 权威答案数  
    uint16_t numA2;       // 附加答案数  
}dnsHead;


typedef enum fileType
{
    JS = 0,
    CSS,
    PNG,
    JPG,
    ICO,
    TEXT,
    GIF,
    MO,
    FTYPEMAX,   
}fileType;


typedef struct{
    fileType type;  //类型
    uint8_t typelen:3;//类型长度
    char *name;  //后缀句
    char *ctype;//Content-Type

}fType;


typedef enum folderType
{
    CGIBIN = 0,
    SYSTEM,
    ROOT,  
}folderType;





typedef enum ProtocolType {
    HTTPNONE = 0,
    GET,
    POST,
    HEAD,
    PTYPEMAX,
} ProtocolType;

typedef enum HostType {
    NONE_HOST = 0,
    APPLE_HOST,
    ANDROID_HOST,
} HostType;



typedef struct accept
{
    char *buf;
    char *method;
    char *url;
    
}accept;


typedef struct webConn{
    uint8_t encrypt:1;
    uint8_t cache:1;
    uint8_t nocache:1;
    uint8_t websocket:1;
    uint8_t keepalive:1;
    uint8_t type:2;
    uint8_t folder:2;
    int32_t postLen;

}webConn;


typedef struct php_funcationType
{
    char *php_cmdName;
    int8_t php_cmdLen;
    int8_t (*php_exeCmd)(int client);
}php_funcationType;

typedef struct php_funPostType
{
    char *php_cmdName;
    int8_t php_cmdLen;
    int8_t (*php_exeCmd)(int client,webConn *conn);
}php_funPostType;

extern int8_t not_found(int client);
extern int8_t post_not_found(int client,webConn *conn);
extern void set_web_login(char *name);
extern void set_web_pwd(char *pwd);

#endif

