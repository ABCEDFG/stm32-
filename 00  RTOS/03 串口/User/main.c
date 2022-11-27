#include "board.h"
#include "rtthread.h"


/* �����߳̿��ƿ� */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t key_thread  = RT_NULL;

/* �߳����庯�� */
static void led1_thread_entry(void *parameter);
static void key_thread_entry(void *parameter);

int main()
{
	/* ���� LED1 �߳� */
	led1_thread = rt_thread_create(
					 "led1",                // �߳���
					 led1_thread_entry,     // �߳���ں���
					 RT_NULL,               // �߳���ں�������
					 512,                   // �߳�ջ��С
					 3,                     // �߳����ȼ�
					 20);                   // �߳�ʱ��Ƭ
	 
	/* �����̣߳��������� */
	if(led1_thread!=RT_NULL)
	{
		rt_thread_startup(led1_thread);
	}
	else
	{
		return -1;
	}
	
	/* ���� KEY �߳� */
	key_thread = rt_thread_create(
					 "key",                // �߳���
					 key_thread_entry,     // �߳���ں���
					 RT_NULL,               // �߳���ں�������
					 512,                   // �߳�ջ��С
					 2,                     // �߳����ȼ�
					 20);                   // �߳�ʱ��Ƭ
	 
	/* �����̣߳��������� */
	if(key_thread!=RT_NULL)
	{
		rt_thread_startup(key_thread);
	}
	else
	{
		return -1;
	}
}


// LED1 �߳�
static void led1_thread_entry(void *parameter)
{
	while(1)
	{
		LED1=!LED1;
		rt_thread_delay(500);
		rt_kprintf("led1\n");
	}
}


// KEY �߳�
static void key_thread_entry(void *parameter)
{
	u8 key=0;
	rt_err_t erflag=0;
	
	while(1)
	{
		key=Key_Scan(0);
		if(key==1)
		{
			rt_kprintf("LED1�̹߳���\n");
			erflag=rt_thread_suspend(led1_thread);
			if(erflag==RT_EOK)
				rt_kprintf("�̹߳���ɹ���\r\n");
			else
				rt_kprintf("�̹߳���ʧ�ܣ�\r\n");
		}
		else if(key==2)
		{
			rt_kprintf("LED1�ָ̻߳���\r\n");
			erflag=rt_thread_resume(led1_thread);
			if(erflag==RT_EOK)
				rt_kprintf("�ָ̻߳��ɹ���\r\n");
			else
				rt_kprintf("�ָ̻߳�ʧ�ܣ�\r\n");
		}
		rt_thread_delay(20);   /* ��ʱ20��tick */
	}
}
