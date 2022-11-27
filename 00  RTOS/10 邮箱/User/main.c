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
static rt_mailbox_t text_mail = RT_NULL;

char text_str1[] = "����1\n";
char text_str2[] = "����2\n";




int main()
{
	/* �������� */
	text_mail = rt_mb_create("text_mail", 10, RT_IPC_FLAG_FIFO);   // ����ģʽ   RT_IPC_FLAG_FIFO  ˳��ģʽ

	if(text_mail != RT_NULL)
		rt_kprintf("���䴴���ɹ���\n");
	
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
			rt_kprintf("��������1\n");
			rt_mb_send(text_mail, (rt_uint32_t)&text_str1);  // ���� ���� 1
		}
		else if(key == 2)
		{
			rt_kprintf("��������2\n");
			rt_mb_send(text_mail, (rt_uint32_t)&text_str2);  // ���� ���� 2
		}
		
		rt_thread_delay(20);  // ��ʱ 20ms
	}
}


/* �����߳� */
static void rec_thread_entry(void *parameter)
{
	rt_err_t erflag = 0;
	char *str;
	
	while(1)
	{
		/* �ȴ�������Ϣ */
		erflag = rt_mb_recv(text_mail, (rt_uint32_t*)&str, RT_WAITING_FOREVER);  // һֱ��
		
		if(erflag == RT_EOK)
		{
			rt_kprintf("���������ǣ�%s\n", str);
		}
		else
		{
			rt_kprintf("������ܴ���");
		}
		
		
		rt_thread_delay(20);  // ��ʱ 20ms
	}
}
