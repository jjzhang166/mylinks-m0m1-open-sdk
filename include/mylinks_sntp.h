#ifndef __MYLINKS_SNTP_H
#define __MYLINKS_SNTP_H
#include <time.h>
/*函数功能：
    同步网络时间到系统时间
    char *server_name:网络时间服务器的域名或者IP地址
    返回值:
    0   成功
    -1  失败
*/
extern int8_t sntp_init(char *server_name);


/*函数功能：
    获取当时系统的相对于1970年时的秒数
    int8_t timezone:时区
    返回值:当时的相对秒数
*/
extern uint32_t sntp_get_current_timestamp(int8_t timezone);

/*函数功能：
    获取当时系统的相对于1970年时的秒数，并转化为tm的结构体
    int8_t timezone:时区
    返回值:struct tm结构体
*/
extern struct tm *get_local_time(int8_t timezone);
#endif

