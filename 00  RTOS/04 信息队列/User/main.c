#include "board.h"
#include "rtthread.h"


/* �����߳̿��ƿ� */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t send_msg_thread  = RT_NULL;
static rt_thread_t rec_msg_thread = RT_NULL;

/* �߳����庯�� */
static void led1_thread_entry(void *parameter);
static void send_msg_thread_entry(void *parameter);
static void rec_msg_thread_entry(void *parameter);


/* ��Ϣ���п��ƿ� */
static rt_mq_t text_mq = RT_NULL;

int main()
{
	/* ������Ϣ���� */
	text_mq = rt_mq_create(
				"text_mq",  // ��Ϣ��������
				40,  // ��Ϣ����󳤶�
				20,  // ��Ϣ���е��������
				RT_IPC_FLAG_FIFO);  // ����ģʽ   RT_IPC_FLAG_PRIO  ���ȼ�ģʽ

	if(text_mq != RT_NULL)
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
	send_msg_thread = rt_thread_create(
						"send_msg",             // �߳�����
						send_msg_thread_entry,  // �߳���ں���
						RT_NULL,                // �̲߳���
						512,                    // �̴߳�С
						2,                      // �߳����ȼ�
						20);                    // �߳�ʱ��Ƭ
	
	/* ����������Ϣ�߳� */
	if(send_msg_thread != RT_NULL)
	{
		rt_thread_startup(send_msg_thread);
	}
	else
	{
		return -1;
	}
	
	/* ������Ϣ�����߳� */
	rec_msg_thread = rt_thread_create(
						"rec_msg",             // �߳�����
						rec_msg_thread_entry,  // �߳���ں���
						RT_NULL,                // �̲߳���
						512,                    // �̴߳�С
						2,                      // �߳����ȼ�
						20);                    // �߳�ʱ��Ƭ 
	
	/* ����������Ϣ�߳� */
	if(rec_msg_thread != RT_NULL)
	{
		rt_thread_startup(rec_msg_thread);
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


/* ������Ϣ�߳� */
static void send_msg_thread_entry(void *parameter)
{
	u8 key=0;
	rt_err_t erflag = 0;
	u16 msg_value1 = 1;
	u16 msg_value2 = 2;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 1)
		{
			erflag = rt_mq_send(text_mq, &msg_value1, sizeof(msg_value1));
			if(erflag != RT_EOK)
			{
				rt_kprintf("��Ϣ���ܷ��͵���Ϣ���У� ������룺 0x%lx \n", erflag);
			}
		}
		else if(key == 2)
		{
			erflag = rt_mq_send(text_mq, &msg_value2, sizeof(msg_value2));
			if(erflag != RT_EOK)
			{
				rt_kprintf("��Ϣ���ܷ��͵���Ϣ���У� ������룺 0x%lx \n", erflag);
			}	
		}
		rt_thread_delay(20);  // ��ʱ 20ms
	}
}


/* ������Ϣ�߳� */
static void rec_msg_thread_entry(void *parameter)
{
	rt_err_t erflag = 0;
	u16 rec_msg_value = 0;
	
	while(1)
	{
		erflag = rt_mq_recv(text_mq, &rec_msg_value, sizeof(rec_msg_value), RT_WAITING_FOREVER);  // һֱ�ȴ�
		
		if(erflag == RT_EOK)
			rt_kprintf("���ν��ܵ�����: %d \n", rec_msg_value);
		else
			rt_kprintf("���ݽ��ܴ��� ������룺 0x%lx \n", erflag);
		
		rt_thread_delay(20);  // ��ʱ 20ms
	}
}
