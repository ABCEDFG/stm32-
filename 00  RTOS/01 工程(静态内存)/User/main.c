#include "board.h"
#include "rtthread.h"

/* 定义线程控制块 */
static struct rt_thread led1_thread;

/* 定义线程栈时要求 RT_ALIFN_SIZE 个字节对齐 */
ALIGN(RT_ALIGN_SIZE)

/* 定义线程栈 静态内存 */
static rt_uint8_t rt_led1_thread_stack[128];

/* 线程主体函数 */
static void led1_thread_entry(void *parameter);



int main()
{
	rt_thread_init(&led1_thread,                   // 线程控制块
					"led1",                        // 线程名
					led1_thread_entry,             // 线程入口函数
					RT_NULL,                       // 线程入口函数参数
					&rt_led1_thread_stack[0],      // 线程起始地址
					sizeof(rt_led1_thread_stack),  // 线程的大小
					3,                             // 线程的优先级
					20);                           // 线程时间片
	
	rt_thread_startup(&led1_thread);
}


// LED1 线程
static void led1_thread_entry(void *parameter)
{
	while(1)
	{
		LED1=!LED1;
		rt_thread_delay(500);
	}
}
