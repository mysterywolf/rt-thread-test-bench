#include "lwip/opt.h"
#if LWIP_SOCKET && LWIP_TEST_CLIENT

#include "apps/test_client.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include <string.h>

/*使能测试线程*/
#define EN_connect_close_repeat_test_client_thread  (1)
#define EN_repeat_test_client_thread                (0)

#define TEST_SERVER_ADDR                        "192.168.1.100"/*服务器IP地址*/

/*服务器端口号*/
#define CONNECT_CLOSE_REPECT_TEST_SERVER_PORT   10000
#define REPECT_TEST_SERVER_PORT                 10001

#define REPEAT_TIME_MS                          500 /*线程循环delay时间(ms)*/
#define PRINTF(...)    LWIP_DEBUGF(TEST_CLIENT_DEBUG, (__VA_ARGS__))

#if EN_connect_close_repeat_test_client_thread || \
EN_repeat_test_client_thread
static const char *test_string = "test_client_thread\r\n";
#endif

#if EN_connect_close_repeat_test_client_thread
//该线程用于测试本机作为客户端 反复connect-发送数据-close的过程
//在LWIP2.1.2版本，该测试有可能测试不通过
static void connect_close_repeat_test_client_thread(void *par)
{
    struct sockaddr_in addr;
    int sockfd;
    
    MEMSET(&addr,0,sizeof(addr));
    addr.sin_len=sizeof(addr);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(CONNECT_CLOSE_REPECT_TEST_SERVER_PORT);/*服务器端口号*/
    addr.sin_addr.s_addr=inet_addr(TEST_SERVER_ADDR);/*服务器地址*/
   
    while(1)
    {
        sockfd=socket(AF_INET,SOCK_STREAM,0);
        int ret = connect (sockfd,(struct sockaddr*)&addr,sizeof(addr));
        if(ret!=0){/*err*/         
            PRINTF("connect_close_repeat_test_client_thread: connect err!\r\n");
        }
        lwip_write(sockfd,test_string,strlen(test_string)); 
        lwip_close(sockfd);
        
        sys_msleep(REPEAT_TIME_MS);
    }
}
#endif

#if EN_repeat_test_client_thread
//该线程用于测试本机作为客户端 connect-反复发送数据的过程(不close)
static void repeat_test_client_thread(void *par)
{
    struct sockaddr_in addr;
    int sockfd;
    
    MEMSET(&addr,0,sizeof(addr));
    addr.sin_len=sizeof(addr);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(REPECT_TEST_SERVER_PORT);/*服务器端口号*/
    addr.sin_addr.s_addr=inet_addr(TEST_SERVER_ADDR);/*服务器地址*/

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    int ret = connect (sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret!=0){/*err*/         
        PRINTF("connect_close_repeat_test_client_thread: connect err!\r\n");
    }  
    
    while(1)
    {
        lwip_write(sockfd,test_string,strlen(test_string)); 
        sys_msleep(REPEAT_TIME_MS);
    }
    
    //lwip_close(sockfd);
}
#endif

void test_client_init(void)
{
#if EN_connect_close_repeat_test_client_thread    
    sys_thread_new("connect_close_repeat_test_client_thread", connect_close_repeat_test_client_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);    
#endif
    
#if EN_repeat_test_client_thread
    sys_thread_new("repeat_test_client_thread", repeat_test_client_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);    
#endif
}

#endif
