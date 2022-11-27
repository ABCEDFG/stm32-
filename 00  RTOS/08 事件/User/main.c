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


/* �¼����ƿ� */
static rt_event_t text_event = RT_NULL;

#define K_UP_EVENT (0X01<<0)    // �����¼������λ 0
#define K_DOWN_EVENT (0X01<<1)  // �����¼������λ 1




int main()
{
	/* �����¼� */
	text_event = rt_event_create("text_event", RT_IPC_FLAG_PRIO);   // ����ģʽ   RT_IPC_FLAG_FIFO  ˳��ģʽ

	if(text_event != RT_NULL)
		rt_kprintf("�¼������ɹ���\n");
	
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
	u8 key = 0;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 1)
		{
			rt_kprintf("���� KEY_UP �¼�\n");
			rt_event_send(text_event,K_UP_EVENT);  // ���� K_UP_EVENT �¼�
		}
		else if(key == 2)
		{
			rt_kprintf("���� KEY_DOWN �¼�\n");
			rt_event_send(text_event,K_DOWN_EVENT);  // ���� K_DOWN_EVENT �¼�
		}
		
		rt_thread_delay(20);  // ��ʱ 20ms
	}
}


/* �����߳� */
static void rec_thread_entry(void *parameter)
{
	rt_uint32_t ree_value = 0;
	
	while(1)
	{
		rt_event_recv(text_event, K_UP_EVENT | K_DOWN_EVENT, 
					RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,  // ���߼������¼� | ���ܺ������־
					RT_WAITING_FOREVER, &ree_value);  // ָ����ʱ�¼���һֱ��
		
		if(ree_value == K_UP_EVENT)
		{
			rt_kprintf("���յ� KEY_UP �¼�\n");
		}
		else if(ree_value == K_DOWN_EVENT)
		{
			rt_kprintf("���յ� K_DOWN_EVENT �¼�\n");
		}
		
		rt_thread_delay(20);  // ��ʱ 20ms
	}
}
