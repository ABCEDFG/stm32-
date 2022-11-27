#include <rtthread.h>
#include "ARMCM3.h"
#include <rthw.h>

/* 测试用的变量 */
rt_uint8_t flag1;
rt_uint8_t flag2;
rt_uint8_t flag3;

/* 线程就绪列表 */
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];  

/* 定义线程控制块 */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
struct rt_thread rt_flag3_thread;

/* 定义线程栈 */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
rt_uint8_t rt_flag3_thread_stack[512];

/* 线程声明 */
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);
void flag3_thread_entry(void *p_arg);


int main(void)
{
	/* 硬件初始化 */
	
	/* 关中断 */
    rt_hw_interrupt_disable();
    
    /* SysTick中断频率设置 */
    SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );
	
	/* 系统定时器列表初化 */
	rt_system_timer_init();
	
	/* 初始化调度器 */
	rt_system_scheduler_init();
	
	/* 初始化空闲线程 */
	rt_thread_idle_init();	
	
	/* 初始化线程 */
	rt_thread_init(	&rt_flag1_thread,                //线程控制块
					"rt_flag1_thread",                /* 线程名字，字符串形式 */
				    flag1_thread_entry,              //线程入口地址（函数名称）
					RT_NULL,                         //线程形参
					&rt_flag1_thread_stack[0],       //线程栈起始地址
					sizeof(rt_flag1_thread_stack),  //线程栈大小（字节）
					2,
					4);
	/* 将线程插入到就绪列表 */
	//rt_list_insert_before(&(rt_thread_priority_table[0]),&(rt_flag1_thread.tlist));
	rt_thread_startup(&rt_flag1_thread);
					
	rt_thread_init(	&rt_flag2_thread,                //线程控制块
					"rt_flag2_thread",                /* 线程名字，字符串形式 */
				    flag2_thread_entry,              //线程入口地址（函数名称）
					RT_NULL,                         //线程形参
					&rt_flag2_thread_stack[0],       //线程栈起始地址
					sizeof(rt_flag2_thread_stack),  //线程栈大小（字节）
					3,
					2);
	/* 将线程插入到就绪列表 */
	//rt_list_insert_before( &(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist) );
	rt_thread_startup(&rt_flag2_thread);
					
	rt_thread_init(	&rt_flag3_thread,                //线程控制块
					"rt_flag3_thread",                /* 线程名字，字符串形式 */
				    flag3_thread_entry,              //线程入口地址（函数名称）
					RT_NULL,                         //线程形参
					&rt_flag3_thread_stack[0],       //线程栈起始地址
					sizeof(rt_flag3_thread_stack),  //线程栈大小（字节）
					3,
					3);
	/* 将线程插入到就绪列表 */
	//rt_list_insert_before( &(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist) );
	rt_thread_startup(&rt_flag3_thread);
					
	/* 启动系统调度器 */
	rt_system_scheduler_start();
}

void delay(uint32_t x)
{
   	while(x--);
}

void flag1_thread_entry( void *p_arg )
{
	for(;;)
	{
//		flag1=1;
//		delay(100);
//		flag1=0;
//		delay(100);
//	
//		/* 线程切换（手动切换） */
//		rt_schedule();

		flag1=1;
		rt_thread_delay(4);
		flag1=0;
		rt_thread_delay(4);
	}
}

void flag2_thread_entry( void *p_arg )
{	
	for(;;)
	{
//		flag2 = 1;
//		delay( 100 );		
//		flag2 = 0;
//		delay( 100 );
//		
//		/* 线程切换（手动切换） */
//		rt_schedule();

		flag2=1;
		delay(100);
		flag2=0;
		delay(100);
	}
}


void flag3_thread_entry( void *p_arg )
{	
	for(;;)
	{
//		flag2 = 1;
//		delay( 100 );		
//		flag2 = 0;
//		delay( 100 );
//		
//		/* 线程切换（手动切换） */
//		rt_schedule();

		flag3=1;
		delay(100);
		flag3=0;
		delay(100);
	}
}

void SysTick_Handler(void)
{
    /* 进入中断 */
    rt_interrupt_enter();

    rt_tick_increase();

    /* 离开中断 */
    rt_interrupt_leave();
}
