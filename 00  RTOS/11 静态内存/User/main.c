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

/* �ڴ���ƿ� */
static rt_mp_t test_mp = RT_NULL;

/* ���������ڴ��ָ�� */
static rt_uint32_t *p_test = RT_NULL;

#define BLOCK_COUNT 20  // �ڴ������
#define BLOCK_SIZE  3   // �ڴ���С


int main()
{
	/* ������̬�ڴ�� */
	test_mp = rt_mp_create("test_mp",
							BLOCK_COUNT,   // �ڴ������
							BLOCK_SIZE);   // �ڴ���С

	if(test_mp != RT_NULL)
		rt_kprintf("���䴴���ɹ���\n");
	else
		return -1;
	
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
			
			p_test = rt_mp_alloc(test_mp, 0);  // ���ڴ�������ڴ�
			
			if(p_test == RT_NULL)
			{
				rt_kprintf("��̬�ڴ�����ʧ�ܣ�\n");
			}
			else
			{
				rt_kprintf("��̬�ڴ�����ɹ��� ��ַΪ��%d! \n\n", p_test);
				rt_kprintf("������ p_test д������...\n");
				
				*p_test = 1234;
				
				rt_kprintf("д���������\n");
				rt_kprintf("*p_test = %4d, ��ַΪ��%d \n\n", *p_test, p_test);
			}
		}
		
		rt_thread_delay(20); // 20ms
	}
}


/* �����߳� */
static void free_thread_entry(void *parameter)
{
	u8 key = 0;
	rt_err_t erfalg = RT_EOK;
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 2)
		{
			rt_kprintf("�����ͷ��ڴ�...\n");
			
			rt_mp_free(p_test);  // �ͷ��ڴ�
			
			rt_kprintf("�ͷ��ڴ�ɹ���\n");
			rt_kprintf("����ɾ���ڴ�...\n");
			
			erfalg = rt_mp_delete(test_mp);  // ɾ���ڴ�
			
			if(erfalg == RT_EOK)
			{
				rt_kprintf("ɾ���ڴ�ɹ���\n");
			}
		}
		
		rt_thread_delay(20); // 20ms
	}
}
