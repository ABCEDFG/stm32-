#include "board.h"
#include "rtthread.h"

/* �����߳̿��ƿ� */
static struct rt_thread led1_thread;

/* �����߳�ջʱҪ�� RT_ALIFN_SIZE ���ֽڶ��� */
ALIGN(RT_ALIGN_SIZE)

/* �����߳�ջ ��̬�ڴ� */
static rt_uint8_t rt_led1_thread_stack[128];

/* �߳����庯�� */
static void led1_thread_entry(void *parameter);



int main()
{
	rt_thread_init(&led1_thread,                   // �߳̿��ƿ�
					"led1",                        // �߳���
					led1_thread_entry,             // �߳���ں���
					RT_NULL,                       // �߳���ں�������
					&rt_led1_thread_stack[0],      // �߳���ʼ��ַ
					sizeof(rt_led1_thread_stack),  // �̵߳Ĵ�С
					3,                             // �̵߳����ȼ�
					20);                           // �߳�ʱ��Ƭ
	
	rt_thread_startup(&led1_thread);
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
