#include <SysTick.h>

static u8  fac_us=0;
static u16 fac_ms=0;

void SysTick_Init(u8 SYSCLK)
{
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//��AHBʱ�ӳ���8��Ϊʱ��Դ
		fac_us=SYSCLK/8;	
	  fac_ms=(u16)fac_us * 1000;
}

void delay_us(u32 nus)	//���1.864s
{
		u32 temp;
		SysTick->LOAD=nus*fac_us;	//ʱ�����
		SysTick->VAL=0x00;	//��ն�ʱ����
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;	//�򿪶�ʱ������ʼ����
		do
		{
				temp=SysTick->CTRL;	//��ȡ��ʱ����ֵ
		}while((temp&0x01)&&!(temp&(1<<16)));	//�Ѵ򿪶�ʱ���Ҷ�ʱ����
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رն�ʱ��
		SysTick->VAL=0x00;	//��ն�ʱ��
}

void delay_ms(u16 nms) 	//���1.864s
{
		u32 temp;
		SysTick->LOAD=nms*fac_ms;	//ʱ�����
		SysTick->VAL=0x00;	//��ն�ʱ��
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;	//�򿪶�ʱ������ʼ����
		do
		{
				temp=SysTick->CTRL;	//��ȡ��ʱ����ֵ
		}while((temp&0x01)&&!(temp&(1<<16)));	//�Ѵ򿪶�ʱ���Ҷ�ʱ����
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رն�ʱ��
		SysTick->VAL=0x00;	//��ն�ʱ��
}
