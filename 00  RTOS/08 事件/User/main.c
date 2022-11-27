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
static rt_event_t text_event = RT_NULL;

#define K_UP_EVENT (0X01<<0)    // 设置事件掩码的位 0
#define K_DOWN_EVENT (0X01<<1)  // 设置事件掩码的位 1




int main()
{
	/* 创建事件 */
	text_event = rt_event_create("text_event", RT_IPC_FLAG_PRIO);   // 队列模式   RT_IPC_FLAG_FIFO  顺序模式

	if(text_event != RT_NULL)
		rt_kprintf("事件创建成功！\n");
	
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
			rt_kprintf("发送 KEY_UP 事件\n");
			rt_event_send(text_event,K_UP_EVENT);  // 发送 K_UP_EVENT 事件
		}
		else if(key == 2)
		{
			rt_kprintf("发送 KEY_DOWN 事件\n");
			rt_event_send(text_event,K_DOWN_EVENT);  // 发送 K_DOWN_EVENT 事件
		}
		
		rt_thread_delay(20);  // 延时 20ms
	}
}


/* 接受线程 */
static void rec_thread_entry(void *parameter)
{
	rt_uint32_t ree_value = 0;
	
	while(1)
	{
		rt_event_recv(text_event, K_UP_EVENT | K_DOWN_EVENT, 
					RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,  // 或逻辑接受事件 | 接受后清除标志
					RT_WAITING_FOREVER, &ree_value);  // 指定超时事件，一直等
		
		if(ree_value == K_UP_EVENT)
		{
			rt_kprintf("接收到 KEY_UP 事件\n");
		}
		else if(ree_value == K_DOWN_EVENT)
		{
			rt_kprintf("接收到 K_DOWN_EVENT 事件\n");
		}
		
		rt_thread_delay(20);  // 延时 20ms
	}
}
