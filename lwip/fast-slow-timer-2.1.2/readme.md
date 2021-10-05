本文件为LWIP socket可靠性测试文件

在LWIP2.1.2版本中，如果长时间的反复socket-connect-close，会导致在close并经历2MSL后内存依然无法被回收，进而消耗光内存。打开`EN_connect_close_repeat_test_client_thread`宏定义模拟了这一情况。

使用时，直接调用`test_client_init()`函数进行初始化即可。

增加宏定义用以输出信息：

```c
/**
 * TEST_CLIENT_DEBUG: Enable debugging for test client by lwip TCP.
 */
#define TEST_CLIENT_DEBUG               LWIP_DBG_OFF
```



在PC端打开网络调试助手，选择服务器模式，并将服务器的端口号设置为`CONNECT_CLOSE_REPECT_TEST_SERVER_PORT`，运行后会在网络调试助手上反复输出："test_client_thread"



注意：测试需要长时间运行，吃内存的问题才能暴露，建议也要定时输出内存使用信息，以监视内存堆的使用情况。



仓库中附了另一位工程师和我出现相同问题之后的分析笔记（分析的过程不一定对，但是出现的问题和结论是一致的）。