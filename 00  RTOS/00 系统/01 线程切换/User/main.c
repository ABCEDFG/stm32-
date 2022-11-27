#include <rtthread.h>
#include <ARMCM3.h>

/* 测试用的变量 */
rt_uint8_t flag1;
rt_uint8_t flag2;

/* 线程就绪列表 */
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];  

/* 定义线程控制块 */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;

/* 定义线程栈 */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];

/* 线程声明 */
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);


int main(void)
{
	/* 初始化调度器 */
	rt_system_scheduler_init();
	
	/* 初始化线程 */
	rt_thread_init(	&rt_flag1_thread,                //线程控制块
				    flag1_thread_entry,              //线程入口地址（函数名称）
					RT_NULL,                         //线程形参
					&rt_flag1_thread_stack[0],       //线程栈起始地址
					sizeof(rt_flag1_thread_stack));  //线程栈大小（字节）
	/* 将线程插入到就绪列表 */
	rt_list_insert_before(&(rt_thread_priority_table[0]),&(rt_flag1_thread.tlist));

	rt_thread_init(	&rt_flag2_thread,                //线程控制块
				    flag2_thread_entry,              //线程入口地址（函数名称）
					RT_NULL,                         //线程形参
					&rt_flag2_thread_stack[0],       //线程栈起始地址
					sizeof(rt_flag2_thread_stack));  //线程栈大小（字节）
	/* 将线程插入到就绪列表 */
	rt_list_insert_before( &(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist) );
	
	/* 启动系统调度器 */
	rt_system_scheduler_start();
}

void delay(uint32_t x)
{
   	while(x--);
}

void flag1_thread_entry( void *p_arg )
{
	while(1)
	{
		flag1=1;
		delay(100);
		flag1=0;
		delay(100);
	
		/* 线程切换（手动切换） */
		rt_schedule();
	}
}

void flag2_thread_entry( void *p_arg )
{	
	while(1)
	{
		flag2 = 1;
		delay( 100 );		
		flag2 = 0;
		delay( 100 );
		
		/* 线程切换（手动切换） */
		rt_schedule();
	}
}

