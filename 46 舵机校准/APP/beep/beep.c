#include <beep.h>


void Beep_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  //����GPIO��ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//���� GPIOB ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;		//IO�� 8
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	//��������50MHz
	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	//GPIOB ��ʼ��
	GPIO_SetBits(GPIOB,GPIO_Pin_8);	//��ʼ��GPIOB 8
}
