#include "board.h"
#include "rtthread.h"


/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t key_thread  = RT_NULL;

/* 线程主体函数 */
static void led1_thread_entry(void *parameter);
static void key_thread_entry(void *parameter);

int main()
{
	/* 创建 LED1 线程 */
	led1_thread = rt_thread_create(
					 "led1",                // 线程名
					 led1_thread_entry,     // 线程入口函数
					 RT_NULL,               // 线程入口函数参数
					 512,                   // 线程栈大小
					 3,                     // 线程优先级
					 20);                   // 线程时间片
	 
	/* 启动线程，开启调度 */
	if(led1_thread!=RT_NULL)
	{
		rt_thread_startup(led1_thread);
	}
	else
	{
		return -1;
	}
	
	/* 创建 KEY 线程 */
	key_thread = rt_thread_create(
					 "key",                // 线程名
					 key_thread_entry,     // 线程入口函数
					 RT_NULL,               // 线程入口函数参数
					 512,                   // 线程栈大小
					 2,                     // 线程优先级
					 20);                   // 线程时间片
	 
	/* 启动线程，开启调度 */
	if(key_thread!=RT_NULL)
	{
		rt_thread_startup(key_thread);
	}
	else
	{
		return -1;
	}
}


// LED1 线程
static void led1_thread_entry(void *parameter)
{
	while(1)
	{
		LED1=!LED1;
		rt_thread_delay(500);
		rt_kprintf("led1\n");
	}
}


// KEY 线程
static void key_thread_entry(void *parameter)
{
	u8 key=0;
	rt_err_t erflag=0;
	
	while(1)
	{
		key=Key_Scan(0);
		if(key==1)
		{
			rt_kprintf("LED1线程挂起！\n");
			erflag=rt_thread_suspend(led1_thread);
			if(erflag==RT_EOK)
				rt_kprintf("线程挂起成功！\r\n");
			else
				rt_kprintf("线程挂起失败！\r\n");
		}
		else if(key==2)
		{
			rt_kprintf("LED1线程恢复！\r\n");
			erflag=rt_thread_resume(led1_thread);
			if(erflag==RT_EOK)
				rt_kprintf("线程恢复成功！\r\n");
			else
				rt_kprintf("线程恢复失败！\r\n");
		}
		rt_thread_delay(20);   /* 延时20个tick */
	}
}
