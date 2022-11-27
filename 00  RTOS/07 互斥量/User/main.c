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


/* ���������ƿ� */
static rt_mutex_t text_mutex = RT_NULL;


/* ȫ�ֱ��� */
u8 gvalue[2] = {0x00, 0x00};



int main()
{
	/* ������Ϣ���� */
	text_mutex = rt_mutex_create("text_mutex", RT_IPC_FLAG_FIFO);   // ����ģʽ   RT_IPC_FLAG_PRIO  ���ȼ�ģʽ

	if(text_mutex != RT_NULL)
		rt_kprintf("�����������ɹ���\n");
	
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
	while(1)
	{
		rt_mutex_take(text_mutex, RT_WAITING_FOREVER);  // ��ȡ��������һֱ�ȴ�
		gvalue[0]++;
		rt_thread_delay(100);  // ��ʱ 100ms
		gvalue[1]++;
		rt_mutex_release(text_mutex);  // �ͷŶ�ֵ�ź�
		rt_thread_delay(100);  // ��ʱ 100ms	
		rt_thread_yield();  // ����ʣ��ʱ��Ƭ������һ���߳��л�
	}
}


/* �����߳� */
static void rec_thread_entry(void *parameter)
{
	while(1)
	{
		rt_mutex_take(text_mutex, RT_WAITING_FOREVER);  // ��ȡ��������һֱ�ȴ�
		if(gvalue[0] == gvalue[1])
			rt_kprintf("�ɹ���\n");
		else
			rt_kprintf("ʧ�ܣ�\n");
		
		rt_mutex_release(text_mutex);  // �ͷŶ�ֵ�ź���
		rt_thread_delay(100);  // ��ʱ 100ms
	}
}
