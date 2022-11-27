#include "board.h"
#include "rtthread.h"


/* �����߳̿��ƿ� */
static rt_thread_t led1_thread = RT_NULL;

/* �߳����庯�� */
static void led1_thread_entry(void *parameter);



int main()
{
//	rt_thread_init(&led1_thread,                   // �߳̿��ƿ�
//					"led1",                        // �߳���
//					led1_thread_entry,             // �߳���ں���
//					RT_NULL,                       // �߳���ں�������
//					&rt_led1_thread_stack[0],      // �߳���ʼ��ַ
//					sizeof(rt_led1_thread_stack),  // �̵߳Ĵ�С
//					3,                             // �̵߳����ȼ�
//					20);                           // �߳�ʱ��Ƭ
	
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
}


// LED1 �߳�
static void led1_thread_entry(void *parameter)
{
	while(1)
	{
		LED1=!LED1;
		rt_thread_delay(500);
	}
}
