#include <rtthread.h>
#include <rthw.h>


extern struct rt_thread *rt_current_thread;

/* �̳߳�ʼ�� */
rt_err_t rt_thread_init(struct rt_thread *thread,          //�߳̿��ƿ�ָ��
						const char    *name,
                        void (*entry)(void *parameter),    //�̺߳�����
                        void             *parameter,       //�߳��β�
                        void             *stack_start,     //�߳�ջ����ʼ��ַ
                        rt_uint32_t       stack_size)      //�߳�ջ�Ĵ�С  �ֽ�
{

	rt_object_init((rt_object_t)thread, RT_Object_Class_Thread, name);  //�̶߳����ʼ��  �߳̽ṹ�忪ͷ���ֵĳ�Ա����rt_object_t����
	
	rt_list_init(&(thread->tlist));  //��ʼ���߳�����ڵ�
	
	thread->entry = (void *)entry;  //�����߳����
	thread->parameter = parameter;  //�����β�

	thread->stack_addr = stack_start;  //�߳�ջ��ʼ��ַ
	thread->stack_size = stack_size;   //�߳�ջ��С
	
	/* ��ʼ���߳�ջ���������߳�ջָ�� */
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
    
    /* ��ȡ��ǰ�̵߳��߳̿��ƿ� */    
    thread = rt_current_thread;
    
    /* ������ʱʱ�� */
    thread->remaining_tick = tick;
	
	/* ����ϵͳ���� */
	rt_schedule();
}

