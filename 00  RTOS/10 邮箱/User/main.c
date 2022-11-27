#include "board.h"
#include "rtthread.h"


/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t send_thread  = RT_NULL;
static rt_thread_t rec_thread = RT_NULL;

/* 线程主体函数 */
static void led1_thread_entry(void *parameter);
static void send_thread_entry(void *parameter);
static void rec_thread_entry(void *parameter);


/* 事件控制块 */
static rt_mailbox_t text_mail = RT_NULL;

char text_str1[] = "邮箱1\n";
char text_str2[] = "邮箱2\n";




int main()
{
	/* 创建邮箱 */
	text_mail = rt_mb_create("text_mail", 10, RT_IPC_FLAG_FIFO);   // 队列模式   RT_IPC_FLAG_FIFO  顺序模式

	if(text_mail != RT_NULL)
		rt_kprintf("邮箱创建成功！\n");
	
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
	send_thread = rt_thread_create(
						"send",             // 线程名字
						send_thread_entry,  // 线程入口函数
						RT_NULL,                // 线程参数
						512,                    // 线程大小
						2,                      // 线程优先级
						20);                    // 线程时间片
	
	/* 启动发送信息线程 */
	if(send_thread != RT_NULL)
	{
		rt_thread_startup(send_thread);
	}
	else
	{
		return -1;
	}
	
	/* 创建信息接受线程 */
	rec_thread = rt_thread_create(
						"rec",             // 线程名字
						rec_thread_entry,  // 线程入口函数
						RT_NULL,                // 线程参数
						512,                    // 线程大小
						2,                      // 线程优先级
						20);                    // 线程时间片 
	
	/* 启动接收信息线程 */
	if(rec_thread != RT_NULL)
	{
		rt_thread_startup(rec_thread);
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


/* 发送线程 */
static void send_thread_entry(void *parameter)
{
	u8 key = 0;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 1)
		{
			rt_kprintf("发送邮箱1\n");
			rt_mb_send(text_mail, (rt_uint32_t)&text_str1);  // 发送 邮箱 1
		}
		else if(key == 2)
		{
			rt_kprintf("发送邮箱2\n");
			rt_mb_send(text_mail, (rt_uint32_t)&text_str2);  // 发送 邮箱 2
		}
		
		rt_thread_delay(20);  // 延时 20ms
	}
}


/* 接受线程 */
static void rec_thread_entry(void *parameter)
{
	rt_err_t erflag = 0;
	char *str;
	
	while(1)
	{
		/* 等待邮箱信息 */
		erflag = rt_mb_recv(text_mail, (rt_uint32_t*)&str, RT_WAITING_FOREVER);  // 一直等
		
		if(erflag == RT_EOK)
		{
			rt_kprintf("邮箱内容是：%s\n", str);
		}
		else
		{
			rt_kprintf("邮箱接受错误！");
		}
		
		
		rt_thread_delay(20);  // 延时 20ms
	}
}
