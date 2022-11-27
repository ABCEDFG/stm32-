#include "board.h"
#include "rtthread.h"


/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t send_msg_thread  = RT_NULL;
static rt_thread_t rec_msg_thread = RT_NULL;

/* 线程主体函数 */
static void led1_thread_entry(void *parameter);
static void send_msg_thread_entry(void *parameter);
static void rec_msg_thread_entry(void *parameter);


/* 信息队列控制块 */
static rt_mq_t text_mq = RT_NULL;

int main()
{
	/* 创建信息队列 */
	text_mq = rt_mq_create(
				"text_mq",  // 信息队列名字
				40,  // 信息的最大长度
				20,  // 信息队列的最大容量
				RT_IPC_FLAG_FIFO);  // 队列模式   RT_IPC_FLAG_PRIO  优先级模式

	if(text_mq != RT_NULL)
		rt_kprintf("信息队列创建成功！\n");
	
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
	
	/* 创建发送信息线程 */
	send_msg_thread = rt_thread_create(
						"send_msg",             // 线程名字
						send_msg_thread_entry,  // 线程入口函数
						RT_NULL,                // 线程参数
						512,                    // 线程大小
						2,                      // 线程优先级
						20);                    // 线程时间片
	
	/* 启动发送信息线程 */
	if(send_msg_thread != RT_NULL)
	{
		rt_thread_startup(send_msg_thread);
	}
	else
	{
		return -1;
	}
	
	/* 创建信息接受线程 */
	rec_msg_thread = rt_thread_create(
						"rec_msg",             // 线程名字
						rec_msg_thread_entry,  // 线程入口函数
						RT_NULL,                // 线程参数
						512,                    // 线程大小
						2,                      // 线程优先级
						20);                    // 线程时间片 
	
	/* 启动接收信息线程 */
	if(rec_msg_thread != RT_NULL)
	{
		rt_thread_startup(rec_msg_thread);
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


/* 发送信息线程 */
static void send_msg_thread_entry(void *parameter)
{
	u8 key=0;
	rt_err_t erflag = 0;
	u16 msg_value1 = 1;
	u16 msg_value2 = 2;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 1)
		{
			erflag = rt_mq_send(text_mq, &msg_value1, sizeof(msg_value1));
			if(erflag != RT_EOK)
			{
				rt_kprintf("信息不能发送到信息队列！ 错误代码： 0x%lx \n", erflag);
			}
		}
		else if(key == 2)
		{
			erflag = rt_mq_send(text_mq, &msg_value2, sizeof(msg_value2));
			if(erflag != RT_EOK)
			{
				rt_kprintf("信息不能发送到信息队列！ 错误代码： 0x%lx \n", erflag);
			}	
		}
		rt_thread_delay(20);  // 延时 20ms
	}
}


/* 接受信息线程 */
static void rec_msg_thread_entry(void *parameter)
{
	rt_err_t erflag = 0;
	u16 rec_msg_value = 0;
	
	while(1)
	{
		erflag = rt_mq_recv(text_mq, &rec_msg_value, sizeof(rec_msg_value), RT_WAITING_FOREVER);  // 一直等待
		
		if(erflag == RT_EOK)
			rt_kprintf("本次接受的数据: %d \n", rec_msg_value);
		else
			rt_kprintf("数据接受错误！ 错误代码： 0x%lx \n", erflag);
		
		rt_thread_delay(20);  // 延时 20ms
	}
}
