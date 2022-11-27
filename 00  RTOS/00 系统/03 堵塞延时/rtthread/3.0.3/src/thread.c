#include <rtthread.h>
#include <rthw.h>


extern struct rt_thread *rt_current_thread;

/* 线程初始化 */
rt_err_t rt_thread_init(struct rt_thread *thread,          //线程控制块指针
						const char    *name,
                        void (*entry)(void *parameter),    //线程函数名
                        void             *parameter,       //线程形参
                        void             *stack_start,     //线程栈的起始地址
                        rt_uint32_t       stack_size)      //线程栈的大小  字节
{

	rt_object_init((rt_object_t)thread, RT_Object_Class_Thread, name);  //线程对象初始化  线程结构体开头部分的成员就是rt_object_t类型
	
	rt_list_init(&(thread->tlist));  //初始化线程链表节点
	
	thread->entry = (void *)entry;  //保存线程入口
	thread->parameter = parameter;  //保存形参

	thread->stack_addr = stack_start;  //线程栈起始地址
	thread->stack_size = stack_size;   //线程栈大小
	
	/* 初始化线程栈，并返回线程栈指针 */
	thread->sp = (void *)rt_hw_stack_init( thread->entry, 
		                                   thread->parameter,
							               (void *)((char *)thread->stack_addr + thread->stack_size - 4) );
	
	return RT_EOK;
}

//rt_err_t rt_thread_resume(rt_thread_t thread)
//{
//	//rt_list_remove(&(thread->tlist));
//	rt_schedule_insert_thread(thread);
//}

//rt_err_t rt_thread_startup(rt_thread_t thread)
//{
//	rt_thread_resume(thread);
//}

void rt_thread_delay(rt_tick_t tick)
{
    struct rt_thread *thread;    
    
    /* 获取当前线程的线程控制块 */    
    thread = rt_current_thread;
    
    /* 设置延时时间 */
    thread->remaining_tick = tick;
	
	/* 进行系统调度 */
	rt_schedule();
}

