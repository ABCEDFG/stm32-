#include <rtthread.h>
#include "ARMCM3.h"
#include <rthw.h>

/* �����õı��� */
rt_uint8_t flag1;
rt_uint8_t flag2;
rt_uint8_t flag3;

/* �߳̾����б� */
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];  

/* �����߳̿��ƿ� */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
struct rt_thread rt_flag3_thread;

/* �����߳�ջ */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
rt_uint8_t rt_flag3_thread_stack[512];

/* �߳����� */
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);
void flag3_thread_entry(void *p_arg);


int main(void)
{
	/* Ӳ����ʼ�� */
	
	/* ���ж� */
    rt_hw_interrupt_disable();
    
    /* SysTick�ж�Ƶ������ */
    SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );
	
	/* ϵͳ��ʱ���б���� */
	rt_system_timer_init();
	
	/* ��ʼ�������� */
	rt_system_scheduler_init();
	
	/* ��ʼ�������߳� */
	rt_thread_idle_init();	
	
	/* ��ʼ���߳� */
	rt_thread_init(	&rt_flag1_thread,                //�߳̿��ƿ�
					"rt_flag1_thread",                /* �߳����֣��ַ�����ʽ */
				    flag1_thread_entry,              //�߳���ڵ�ַ���������ƣ�
					RT_NULL,                         //�߳��β�
					&rt_flag1_thread_stack[0],       //�߳�ջ��ʼ��ַ
					sizeof(rt_flag1_thread_stack),  //�߳�ջ��С���ֽڣ�
					2,
					4);
	/* ���̲߳��뵽�����б� */
	//rt_list_insert_before(&(rt_thread_priority_table[0]),&(rt_flag1_thread.tlist));
	rt_thread_startup(&rt_flag1_thread);
					
	rt_thread_init(	&rt_flag2_thread,                //�߳̿��ƿ�
					"rt_flag2_thread",                /* �߳����֣��ַ�����ʽ */
				    flag2_thread_entry,              //�߳���ڵ�ַ���������ƣ�
					RT_NULL,                         //�߳��β�
					&rt_flag2_thread_stack[0],       //�߳�ջ��ʼ��ַ
					sizeof(rt_flag2_thread_stack),  //�߳�ջ��С���ֽڣ�
					3,
					2);
	/* ���̲߳��뵽�����б� */
	//rt_list_insert_before( &(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist) );
	rt_thread_startup(&rt_flag2_thread);
					
	rt_thread_init(	&rt_flag3_thread,                //�߳̿��ƿ�
					"rt_flag3_thread",                /* �߳����֣��ַ�����ʽ */
				    flag3_thread_entry,              //�߳���ڵ�ַ���������ƣ�
					RT_NULL,                         //�߳��β�
					&rt_flag3_thread_stack[0],       //�߳�ջ��ʼ��ַ
					sizeof(rt_flag3_thread_stack),  //�߳�ջ��С���ֽڣ�
					3,
					3);
	/* ���̲߳��뵽�����б� */
	//rt_list_insert_before( &(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist) );
	rt_thread_startup(&rt_flag3_thread);
					
	/* ����ϵͳ������ */
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
//		/* �߳��л����ֶ��л��� */
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
//		/* �߳��л����ֶ��л��� */
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
//		/* �߳��л����ֶ��л��� */
//		rt_schedule();

		flag3=1;
		delay(100);
		flag3=0;
		delay(100);
	}
}

void SysTick_Handler(void)
{
    /* �����ж� */
    rt_interrupt_enter();

    rt_tick_increase();

    /* �뿪�ж� */
    rt_interrupt_leave();
}
