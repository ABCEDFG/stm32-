#include "board.h"
#include "rtthread.h"


/* �����߳̿��ƿ� */
static rt_thread_t led1_thread = RT_NULL;

/* �߳����庯�� */
static void led1_thread_entry(void *parameter);

/* �����ʱ�����ƿ� */
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;

/* ��ʱ���ص����� */
static void swtmr1_callback(void *parameter);
static void swtmr2_callback(void *parameter);

/* ��ʱ���ص����� */
static uint32_t timcb_coun1 = 0;
static uint32_t timcb_coun2 = 0;



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
	
	/* ����һ�������ʱ�� */
	swtmr1 = rt_timer_create(
						"swtmr1",  // ����
						swtmr1_callback,  // �ص�����
						0,             // ��ʱ��������ڲ���
						1000,          // ��ʱʱ��
						RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);  //��ʱ��ģʽ��һ����
	
	/* ������ʱ�� */
	if(swtmr1 != RT_NULL)
	{
		rt_timer_start(swtmr1);                                                                                                                     
	}
	else
	{
		return -1;
	}
	
	/* ����һ�������ʱ�� */
	swtmr2 = rt_timer_create(
						"swtmr2",  // ����
						swtmr2_callback,  // �ص�����
						0,             // ��ʱ��������ڲ���
						5000,          // ��ʱʱ��
						RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);  //��ʱ��ģʽ��һ����
	
	/* ������ʱ�� */
	if(swtmr2 != RT_NULL)
	{
		rt_timer_start(swtmr2);
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


/* �����ʱ��1�Ļص����� */
static void swtmr1_callback(void *parameter)
{
	uint32_t tick_num1;
	
	timcb_coun1++;
	
	tick_num1 = (uint32_t)rt_tick_get();  // ��ȡ�δ�ʱ��ֵ
	
	rt_kprintf("time1 ������%d\n", timcb_coun1);
	rt_kprintf("�δ�ʱ��ֵ��%d", tick_num1);
}


/* �����ʱ�� 2 �Ļص����� */
static void swtmr2_callback(void *parameter)
{
	uint32_t tick_num2;
	
	timcb_coun2++;
	
	tick_num2 = (uint32_t)rt_tick_get();  // ��ȡ�δ�ʱ��ֵ
	
	rt_kprintf("time2 ������%d\n", timcb_coun2);
	rt_kprintf("�δ�ʱ��ֵ��%d", tick_num2);
}

