#include "board.h"
#include "rtthread.h"


/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t alloc_thread  = RT_NULL;
static rt_thread_t free_thread = RT_NULL;

/* 线程主体函数 */
static void led1_thread_entry(void *parameter);
static void alloc_thread_entry(void *parameter);
static void free_thread_entry(void *parameter);

/* 内存控制块 */
static rt_mp_t test_mp = RT_NULL;

/* 定义申请内存的指针 */
static rt_uint32_t *p_test = RT_NULL;

#define BLOCK_COUNT 20  // 内存块数量
#define BLOCK_SIZE  3   // 内存块大小


int main()
{
	/* 创建静态内存池 */
	test_mp = rt_mp_create("test_mp",
							BLOCK_COUNT,   // 内存块数量
							BLOCK_SIZE);   // 内存块大小

	if(test_mp != RT_NULL)
		rt_kprintf("邮箱创建成功！\n");
	else
		return -1;
	
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
	alloc_thread = rt_thread_create(
						"alloc",             // 线程名字
						alloc_thread_entry,  // 线程入口函数
						RT_NULL,                // 线程参数
						512,                    // 线程大小
						2,                      // 线程优先级
						20);                    // 线程时间片
	
	/* 启动发送信息线程 */
	if(alloc_thread != RT_NULL)
	{
		rt_thread_startup(alloc_thread);
	}
	else
	{
		return -1;
	}
	
	/* 创建信息接受线程 */
	free_thread = rt_thread_create(
						"rec",             // 线程名字
						free_thread_entry,  // 线程入口函数
						RT_NULL,                // 线程参数
						512,                    // 线程大小
						2,                      // 线程优先级
						20);                    // 线程时间片 
	
	/* 启动接收信息线程 */
	if(free_thread != RT_NULL)
	{
		rt_thread_startup(free_thread);
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
static void alloc_thread_entry(void *parameter)
{
	u8 key = 0;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 1)
		{
			rt_kprintf("正在向内存池申请内存...\n");
			
			p_test = rt_mp_alloc(test_mp, 0);  // 向内存池申请内存
			
			if(p_test == RT_NULL)
			{
				rt_kprintf("静态内存申请失败！\n");
			}
			else
			{
				rt_kprintf("静态内存申请成功！ 地址为：%d! \n\n", p_test);
				rt_kprintf("正在向 p_test 写入数据...\n");
				
				*p_test = 1234;
				
				rt_kprintf("写入数据完成\n");
				rt_kprintf("*p_test = %4d, 地址为：%d \n\n", *p_test, p_test);
			}
		}
		
		rt_thread_delay(20); // 20ms
	}
}


/* 接受线程 */
static void free_thread_entry(void *parameter)
{
	u8 key = 0;
	rt_err_t erfalg = RT_EOK;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 2)
		{
			rt_kprintf("正在释放内存...\n");
			
			rt_mp_free(p_test);  // 释放内存
			
			rt_kprintf("释放内存成功！\n");
			rt_kprintf("正在删除内存...\n");
			
			erfalg = rt_mp_delete(test_mp);  // 删除内存
			
			if(erfalg == RT_EOK)
			{
				rt_kprintf("删除内存成功！\n");
			}
		}
		
		rt_thread_delay(20); // 20ms
	}
}
