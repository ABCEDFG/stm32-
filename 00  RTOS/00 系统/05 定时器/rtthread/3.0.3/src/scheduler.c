/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/
#include <rtthread.h>
#include <rthw.h>

extern struct rt_thread idle;
extern struct rt_thread rt_flag1_thread;
extern struct rt_thread rt_flag2_thread;

/*
*************************************************************************
*                                 全局变量
*************************************************************************
*/
/* 线程控制块指针，用于指向当前线程 */
struct rt_thread *rt_current_thread;

/* 线程优先级表 */
rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

/* 线程就绪优先级组 */
rt_uint32_t rt_thread_ready_priority_group;

/* 当前优先级 */
rt_uint8_t rt_current_priority;

/* 线程休眠列表 */
rt_list_t rt_thread_defunct;


/*
*************************************************************************
*                               函数实现
*************************************************************************
*/

/* 初始化系统调度器 */
void rt_system_scheduler_init(void)
{	
	register rt_base_t offset;	

	
	/* 线程就绪列表初始化 */
	for (offset = 0; offset < RT_THREAD_PRIORITY_MAX; offset ++)
	{
		rt_list_init(&rt_thread_priority_table[offset]);
	}
	
	/* 初始化当前优先级为空闲线程的优先级 */
    rt_current_priority = RT_THREAD_PRIORITY_MAX - 1;
	
	/* 初始化当前线程控制块指针 */
	rt_current_thread = RT_NULL;
	
	/* 初始化线程就绪优先级组 */
    rt_thread_ready_priority_group = 0;
	
//	/* 初始化线程休眠列表，当线程创建好没有启动之前会被放入到这个列表 */
//	rt_list_init(&rt_thread_defunct);
}


/* 启动系统调度器 */
void rt_system_scheduler_start(void)
{
//	register struct rt_thread *to_thread;
//	

//	/* 手动指定第一个运行的线程 */
//	to_thread = rt_list_entry(rt_thread_priority_table[0].next,
//							  struct rt_thread,
//							  tlist);
//	rt_current_thread = to_thread;
//														
//	/* 切换到第一个线程，该函数在context_rvds.S中实现，在rthw.h声明，
//       用于实现第一次任务切换。当一个汇编函数在C文件中调用的时候，
//       如果有形参，则执行的时候会将形参传到CPU寄存器r0。*/
//	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
	
	
	register struct rt_thread *to_thread;
    register rt_ubase_t highest_ready_priority;
                              
    /* 获取就绪的最高优先级 */
    highest_ready_priority = __rt_ffs(rt_thread_ready_priority_group) - 1;
                              
    /* 获取将要运行线程的线程控制块 */
    to_thread = rt_list_entry(rt_thread_priority_table[highest_ready_priority].next,
                              struct rt_thread,
                              tlist);

    rt_current_thread = to_thread;

    /* 切换到新的线程 */
    rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
}


/* 将线程插入到线程优先级表 */
void rt_schedule_insert_thread(struct rt_thread *thread)
{
    register rt_base_t temp;

    /* 关中断 */
    temp = rt_hw_interrupt_disable();

    /* 改变线程状态 */
    thread->stat = RT_THREAD_READY;

    /* 将线程插入就绪列表 */
    rt_list_insert_before(&(rt_thread_priority_table[thread->current_priority]),
                          &(thread->tlist));

    /* 设置线程就绪优先级组中对应的位 */
    rt_thread_ready_priority_group |= thread->number_mask;

    /* 开中断 */
    rt_hw_interrupt_enable(temp);
}


/* 调度器删除线程 */
void rt_schedule_remove_thread(struct rt_thread *thread)
{
    register rt_base_t temp;


    /* 关中断 */
    temp = rt_hw_interrupt_disable();
    
    /* 将线程从就绪列表删除 */
    rt_list_remove(&(thread->tlist));
    
    if (rt_list_isempty(&(rt_thread_priority_table[thread->current_priority])))
    {
        rt_thread_ready_priority_group &= ~thread->number_mask;
    }

    /* 开中断 */
    rt_hw_interrupt_enable(temp);
}


/* 系统调度 */
void rt_schedule(void)
{
	rt_base_t level;
    register rt_ubase_t highest_ready_priority;
    struct rt_thread *to_thread;
    struct rt_thread *from_thread;

    /* 关中断 */
    level = rt_hw_interrupt_disable();

    /* 获取就绪的最高优先级 */
    highest_ready_priority = __rt_ffs(rt_thread_ready_priority_group) - 1;
    /* 获取就绪的最高优先级对应的线程控制块 */
    to_thread = rt_list_entry(rt_thread_priority_table[highest_ready_priority].next,
                              struct rt_thread,
                              tlist);

    /* 如果目标线程不是当前线程，则要进行线程切换 */
    if (to_thread != rt_current_thread)
    {
        rt_current_priority = (rt_uint8_t)highest_ready_priority;
        from_thread         = rt_current_thread;
        rt_current_thread   = to_thread;

        rt_hw_context_switch((rt_uint32_t)&from_thread->sp,
                             (rt_uint32_t)&to_thread->sp);
        
        /* 开中断 */
        rt_hw_interrupt_enable(level);

    }
    else 
    {
        /* 开中断 */
        rt_hw_interrupt_enable(level);
    }
	
	
	
	
	
	
	
	
	
	
	
	
	
//	struct rt_thread *to_thread;
//	struct rt_thread *from_thread;

//	
//	/* 如果当前线程是空闲线程，那么就去尝试执行线程1或者线程2，
//       看看他们的延时时间是否结束，如果线程的延时时间均没有到期，
//       那就返回继续执行空闲线程 */
//	if( rt_current_thread == &idle )
//	{
//		if(rt_flag1_thread.remaining_tick == 0)
//		{            
//            from_thread = rt_current_thread;
//            to_thread = &rt_flag1_thread;
//            rt_current_thread = to_thread;
//		}
//		else if(rt_flag2_thread.remaining_tick == 0)
//		{
//            from_thread = rt_current_thread;
//            to_thread = &rt_flag2_thread;
//            rt_current_thread = to_thread;
//		}
//		else
//		{
//			return;		/* 线程延时均没有到期则返回，继续执行空闲线程 */
//		} 
//	}
//	else
//	{
//		/*如果当前线程是线程1或者线程2的话，检查下另外一个线程,如果另外的线程不在延时中，就切换到该线程
//        否则，判断下当前线程是否应该进入延时状态，如果是的话，就切换到空闲线程。否则就不进行任何切换 */
//		if(rt_current_thread == &rt_flag1_thread)
//		{
//			if(rt_flag2_thread.remaining_tick == 0)
//			{
//                from_thread = rt_current_thread;
//                to_thread = &rt_flag2_thread;
//                rt_current_thread = to_thread;
//			}
//			else if(rt_current_thread->remaining_tick != 0)
//			{
//                from_thread = rt_current_thread;
//                to_thread = &idle;
//                rt_current_thread = to_thread;
//			}
//			else 
//			{
//				return;		/* 返回，不进行切换，因为两个线程都处于延时中 */
//			}
//		}
//		else if(rt_current_thread == &rt_flag2_thread)
//		{
//			if(rt_flag1_thread.remaining_tick == 0)
//			{
//                from_thread = rt_current_thread;
//                to_thread = &rt_flag1_thread;
//                rt_current_thread = to_thread;
//			}
//			else if(rt_current_thread->remaining_tick != 0)
//			{
//                from_thread = rt_current_thread;
//                to_thread = &idle;
//                rt_current_thread = to_thread;
//			}
//			else 
//			{
//				return;		/* 返回，不进行切换，因为两个线程都处于延时中 */
//			}
//		}
//	}
//	
//	/* 产生上下文切换 */
	//rt_hw_context_switch((rt_uint32_t)&from_thread->sp,(rt_uint32_t)&to_thread->sp);	
	
}
