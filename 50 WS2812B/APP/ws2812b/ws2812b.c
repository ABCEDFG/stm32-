#include "ws2812b.h"


void Ws2812b_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(WS2812B_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=WS2812B_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(WS2812B_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	GPIO_SetBits(WS2812B_PORT,WS2812B_PIN);   //��LED�˿����ߣ�Ϩ������LED
}


static void Ws2812b_Delay(u32 i)
{
	while (i--);
}


static void Ws2812b_WriteBit_1()
{
	WS2812B = 1;
	//Ws2812b_Delay(0);
	WS2812B = 0;
	Ws2812b_Delay(0);
}


static void Ws2812b_WriteBit_0()
{
	WS2812B = 1;
	Ws2812b_Delay(100);
	WS2812B = 0;
	Ws2812b_Delay(100);
}

void Ws2812b_WriteByte(u32 data)
{
	u32 i = 0;
	u8 temp = 0;
	
	for (i = 0; i < 24; ++i)
	{
		temp = (data >> i) & 0x0001;
		if (temp)
		{
			Ws2812b_WriteBit_1();
		}
		else
		{
			Ws2812b_WriteBit_0();
		}
	}
}


void Ws2812b_Write(u32 *data, u8 len)
{
	while (len--)
	{
		Ws2812b_WriteByte(*data);
		++data;
	}
}

