#include "SysTick.h"
#include "rtthread.h"

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������


extern volatile rt_uint8_t rt_interrupt_nest;

// �� board.c �ļ��� rt_hw_board_init() ���潫������Ϊ1
uint8_t OSRunning=0;  

#ifdef RT_THREAD_PRIORITY_MAX  // RT_THREAD_PRIORITY �����ˣ�˵��Ҫ֧�� RT_Thread
#define delay_osrunning OSRunning  // OS �Ƿ����б�� 0��������  1������
#define delay_ostickspersec RT_TICK_PER_SECOND // OS ʱ�ӽ��ģ���ÿ����ȴ���
#define delay_osintnesting rt_interrupt_nest  // �ж�Ƕ�׼��𣬼��ж�Ƕ�״���
#endif


// us����ʱ���ر��������(��ֹ���us����ʱ)
void delay_osschedlock()
{
#ifdef RT_THREAD_PRIORITY_MAX
	rt_enter_critical();
#endif
}

// us����ʱ���ָ��������
void delay_osschedunlock()
{
#ifdef RT_THREAD_PRIORITY_MAX
	rt_exit_critical();
#endif
}

// ���� OS �Դ�����ʱ������ʱ
// ticks: ��ʱ�Ľ�����
void delay_ostimedly(u32 ticks)
{
#ifdef RT_THREAD_PRIORITY_MAX
	rt_thread_delay(ticks);
#endif
}


//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��Ƶ��
void SysTick_Init(u8 SYSCLK)
{
	u32 reload;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;
	reload=SYSCLK/8;  // ÿ���ӵĴ���
	reload*=1000000/delay_ostickspersec;  // ����delay_ostickspersec �趨���ʱ��
	
	fac_ms = 1000/delay_ostickspersec;  // ������� OS ��ʱ�����ٵ�λ
		
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;  // ���� SYSTICK �ж�
	SysTick->LOAD=reload;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;  // ���� SYSTICK
}								    


//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{			
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;  // LOAD ��ֵ
	
	ticks=nus*fac_us;  // ��Ҫ�Ľ�����
	delay_osschedlock();  // ��ֹ OS ���ȣ���ֹ��� us ��ʱ
	told=SysTick->VAL;  // �ս����ǵļ�����ֵ
	
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)
			{
				tcnt+=told-tnow;  // ע�� SYSTICK ��һ���ݼ��ļ�����
			}
			else
			{
				tcnt+=reload-tnow+told;
			}
			told=tnow;
			if(tcnt>=ticks)  // ʱ�䳬��
				break;
		}
	}
	delay_osschedunlock();  // �ָ� OS ����
}

//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
//	SysTick->VAL =0x00;							//��ռ�����
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
//	SysTick->VAL =0X00;       					//��ռ�����	  	 

	/* OS �Ѿ������У��Ҳ������ж���ж��ﲻ��������ȣ� */
	if(delay_osrunning&&delay_osintnesting==0)
	{
		if(nms>=fac_ms)
		{
			delay_ostimedly(nms/fac_ms);  // OS ��ʱ
		}
		nms%=fac_ms;  // OS �޷��ṩ��ôС����ʱ��ʹ����ͨ��������
	}
	delay_us((u32)(nms*1000));  // ��ͨ��ʽ��ʱ
} 


