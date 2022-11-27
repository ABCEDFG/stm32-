#include "board.h"
#include "rtthread.h"


/* �����߳̿��ƿ� */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t send_thread  = RT_NULL;
static rt_thread_t rec_thread = RT_NULL;

/* �߳����庯�� */
static void led1_thread_entry(void *parameter);
static void send_thread_entry(void *parameter);
static void rec_thread_entry(void *parameter);


/* ��Ϣ�����ƿ� */
static rt_sem_t text_sem = RT_NULL;




int main()
{
	/* ������Ϣ���� */
	text_sem = rt_sem_create(
				"text_sem",  // ��Ϣ��������
				10,  		 // �ź�����ֵ��Ĭ����һ���ź���
				RT_IPC_FLAG_FIFO);  // ����ģʽ   RT_IPC_FLAG_PRIO  ���ȼ�ģʽ

	if(text_sem != RT_NULL)
		rt_kprintf("��Ϣ���д����ɹ���\n");
	
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
	
	/* ����������Ϣ�߳� */
	send_thread = rt_thread_create(
						"send",             // �߳�����
						send_thread_entry,  // �߳���ں���
						RT_NULL,                // �̲߳���
						512,                    // �̴߳�С
						2,                      // �߳����ȼ�
						20);                    // �߳�ʱ��Ƭ
	
	/* ����������Ϣ�߳� */
	if(send_thread != RT_NULL)
	{
		rt_thread_startup(send_thread);
	}
	else
	{
		return -1;
	}
	
	/* ������Ϣ�����߳� */
	rec_thread = rt_thread_create(
						"rec",             // �߳�����
						rec_thread_entry,  // �߳���ں���
						RT_NULL,                // �̲߳���
						512,                    // �̴߳�С
						2,                      // �߳����ȼ�
						20);                    // �߳�ʱ��Ƭ 
	
	/* ����������Ϣ�߳� */
	if(rec_thread != RT_NULL)
	{
		rt_thread_startup(rec_thread);
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


/* �����߳� */
static void send_thread_entry(void *parameter)
{
	rt_err_t erflag = 0;
	
	while(1)
	{
		if(Key_Scan(0) == 1)
		{
			erflag = rt_sem_release(text_sem);  // �ͷ�һ�������ź��������ֵΪ 65535
			
			if(erflag == RT_EOK)
			{
				rt_kprintf("�ͷ�һ���ź���\n");
			}
			else
			{
				rt_kprintf("���ź������ͷ�\n");
			}
			
			rt_thread_delay(20);  // ��ʱ 20ms
		}
	}
}


/* �����߳� */
static void rec_thread_entry(void *parameter)
{
	rt_err_t erflag =0;
	
	while(1)
	{
		if(Key_Scan(0) == 2)
		{
			erflag = rt_sem_take(text_sem, 0);  // ��ȡ�ź��������ȴ�
			
			if(erflag == RT_EOK)
			{
				rt_kprintf("�����ź����ɹ�\n");
			}
			else
			{
				rt_kprintf("�ź�������\n");
			}
			
			rt_thread_delay(20);  // ��ʱ 20ms
		}
	}
}
