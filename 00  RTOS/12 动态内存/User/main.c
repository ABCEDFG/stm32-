#include "board.h"
#include "rtthread.h"


/* �����߳̿��ƿ� */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t alloc_thread  = RT_NULL;
static rt_thread_t free_thread = RT_NULL;

/* �߳����庯�� */
static void led1_thread_entry(void *parameter);
static void alloc_thread_entry(void *parameter);
static void free_thread_entry(void *parameter);


/* ���������ڴ��ָ�� */
static rt_uint32_t *p_test = RT_NULL;

#define TEST_SIZE  100   // �ڴ���С


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
	
	/* ����������Ϣ�߳� */
	alloc_thread = rt_thread_create(
						"alloc",             // �߳�����
						alloc_thread_entry,  // �߳���ں���
						RT_NULL,                // �̲߳���
						512,                    // �̴߳�С
						2,                      // �߳����ȼ�
						20);                    // �߳�ʱ��Ƭ
	
	/* ����������Ϣ�߳� */
	if(alloc_thread != RT_NULL)
	{
		rt_thread_startup(alloc_thread);
	}
	else
	{
		return -1;
	}
	
	/* ������Ϣ�����߳� */
	free_thread = rt_thread_create(
						"rec",             // �߳�����
						free_thread_entry,  // �߳���ں���
						RT_NULL,                // �̲߳���
						512,                    // �̴߳�С
						2,                      // �߳����ȼ�
						20);                    // �߳�ʱ��Ƭ 
	
	/* ����������Ϣ�߳� */
	if(free_thread != RT_NULL)
	{
		rt_thread_startup(free_thread);
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
static void alloc_thread_entry(void *parameter)
{
	u8 key = 0;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 1)
		{
			rt_kprintf("�������ڴ�������ڴ�...\n");
			
			p_test = rt_malloc(100);  // ���ڴ�������ڴ�
			
			if(p_test == RT_NULL)
			{
				rt_kprintf("��̬�ڴ�����ʧ�ܣ�\n");
			}
			else
			{
				rt_kprintf("��̬�ڴ�����ɹ��� ��ַΪ��%d! \n\n", p_test);
				rt_kprintf("������ p_test д������...\n");
				
				*p_test = 1234567;
				
				rt_kprintf("д���������\n");
				rt_kprintf("*p_test = %d, ��ַΪ��%d \n\n", *p_test, p_test);
			}
		}
		
		rt_thread_delay(20); // 20ms
	}
}


/* �����߳� */
static void free_thread_entry(void *parameter)
{
	u8 key = 0;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 2)
		{
			rt_kprintf("�����ͷ��ڴ�...\n");
			
			rt_free(p_test);  // �ͷ��ڴ�
			
			rt_kprintf("�ͷ��ڴ�ɹ���\n");
			
		}
		
		rt_thread_delay(20); // 20ms
	}
}
