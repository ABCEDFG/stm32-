#include <rtthread.h>
#include <ARMCM3.h>

/* �����õı��� */
rt_uint8_t flag1;
rt_uint8_t flag2;

/* �߳̾����б� */
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];  

/* �����߳̿��ƿ� */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;

/* �����߳�ջ */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];

/* �߳����� */
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);


int main(void)
{
	/* ��ʼ�������� */
	rt_system_scheduler_init();
	
	/* ��ʼ���߳� */
	rt_thread_init(	&rt_flag1_thread,                //�߳̿��ƿ�
				    flag1_thread_entry,              //�߳���ڵ�ַ���������ƣ�
					RT_NULL,                         //�߳��β�
					&rt_flag1_thread_stack[0],       //�߳�ջ��ʼ��ַ
					sizeof(rt_flag1_thread_stack));  //�߳�ջ��С���ֽڣ�
	/* ���̲߳��뵽�����б� */
	rt_list_insert_before(&(rt_thread_priority_table[0]),&(rt_flag1_thread.tlist));

	rt_thread_init(	&rt_flag2_thread,                //�߳̿��ƿ�
				    flag2_thread_entry,              //�߳���ڵ�ַ���������ƣ�
					RT_NULL,                         //�߳��β�
					&rt_flag2_thread_stack[0],       //�߳�ջ��ʼ��ַ
					sizeof(rt_flag2_thread_stack));  //�߳�ջ��С���ֽڣ�
	/* ���̲߳��뵽�����б� */
	rt_list_insert_before( &(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist) );
	
	/* ����ϵͳ������ */
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
	
		/* �߳��л����ֶ��л��� */
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
		
		/* �߳��л����ֶ��л��� */
		rt_schedule();
	}
}

