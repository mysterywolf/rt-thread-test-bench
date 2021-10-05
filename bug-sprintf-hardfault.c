/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-16     Meco Man   first version
 */

/* 此程序用于测试sprintf是否为线程安全，是否会引发死机 */
/*
经测试，在gcc下，sprintf确实会引发死机问题，主要问题是sprintf内部会使用一个全局性共享内存导致线程不安全。
但是Keil、IAR不存在此问题，经测试，Keil下sprintf表现非常好。
gcc下，sprintf死机也不是生么时候都会发生，必须是两个线程或多个线程高密度的调用sprintf才会发生这个问题。
try=10可能不会死机，但是try=30就会死机了。
*/


#include <rtthread.h>
#include <stdio.h>

static void thread1_entry(void *parameter)
{
    char str[20];
    double a=3.14141414;
    int try = 30;
    while (1)
    {
        rt_thread_delay(5 - rt_tick_get()%5);
        try = 30;
        while(try--)
            sprintf(str, "t1:%lf\n", a);
        rt_kprintf("%s",str);
    }
}

static void thread2_entry(void *parameter)
{
    char str[20];
    double a=3.14141414;
    int try = 30;
    while (1)
    {
        rt_thread_delay(5 - rt_tick_get()%5);
        try = 30;
        while(try--)
            sprintf(str, "t2:%lf\n", a);
        rt_kprintf("%s",str);
    }
}

static void thread3_entry(void *parameter)
{
    char str[20];
    double a=3.14141414;
    int try = 30;
    while (1)
    {
        rt_thread_delay(5 - rt_tick_get()%5);
        try = 30;
        while(try--)
            sprintf(str, "t3:%lf\n", a);
        rt_kprintf("%s",str);
    }
}

int main1(void)
{
    static rt_thread_t tid1, tid2, tid3;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    tid1 = rt_thread_create("thread1",
                            thread1_entry, RT_NULL,
                            1024,
                            2, 0);
    tid2 = rt_thread_create("thread2",
                            thread2_entry, RT_NULL,
                            1024,
                            3, 0);
    tid3 = rt_thread_create("thread3",
                            thread3_entry, RT_NULL,
                            1024,
                            4, 0);
    
    rt_thread_startup(tid1);
    rt_thread_startup(tid2);
    rt_thread_startup(tid3);
    
    while (1)
    {
        rt_thread_mdelay(500);
    }
}
INIT_APP_EXPORT(main1)
