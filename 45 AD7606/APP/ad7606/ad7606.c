#include "ad7606.h"
#include "systick.h"


#define  IDLE 			0
#define  AD_CONV		1
#define  Wait_1			2
#define  Wait_busy		3
#define  READ_CH1		4
#define  READ_CH2		5
#define  READ_CH3		6
#define  READ_CH4		7
#define  READ_DONE		8



void AD7606_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(AD7606_CS_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_RD_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_RESET_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_COA_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_COB_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_OSl0_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_OSl1_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_OSl2_RCC, ENABLE);
	
#if AD7606_FLAG == 0
	RCC_APB2PeriphClockCmd(AD7606_DB0_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB1_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB2_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB3_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB4_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB5_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB6_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB7_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB8_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB9_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB10_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB11_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB12_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB13_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB14_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD7606_DB15_RCC, ENABLE);
#else  // ���п� �� ����ģʽһ��
	RCC_APB2PeriphClockCmd(AD7606_DB_RCC, ENABLE);
#endif
	
	GPIO_InitStructure.GPIO_Pin = AD7606_CS_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_CS_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD7606_RD_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_RD_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD7606_RESET_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_RESET_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD7606_COA_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_COA_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD7606_COB_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_COB_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD7606_OSl0_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_OSl0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_OSl1_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_OSl1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = AD7606_OSl2_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_OSl2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_STBY_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_STBY_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_BUSY_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_BUSY_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_FRD_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(AD7606_FRD_PORT, &GPIO_InitStructure);
	
#if  AD7606_FLAG == 0  // ��ͨ IO ��ģʽ

	GPIO_InitStructure.GPIO_Pin = AD7606_DB0_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB1_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB2_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB3_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB4_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB4_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB5_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB5_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB6_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB6_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB7_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB7_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB8_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB8_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB9_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB9_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB10_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB10_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB11_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB11_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB12_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB12_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB13_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB13_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB14_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB14_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD7606_DB15_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB15_PORT, &GPIO_InitStructure);

#elif  AD7606_FLAG == 1  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB_PORT, &GPIO_InitStructure);
	
#elif  AD7606_FLAG == 2
	GPIO_InitStructure.GPIO_Pin = AD7606_DB_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  // ��������
	GPIO_Init(AD7606_DB_PORT, &GPIO_InitStructure);
	
#endif

	AD7606_COA = 1;  
	AD7606_COB = 1;
	delay_ms(1);
	
	AD7606_STBY = 1;
	AD7606_CS = 1;
	AD7606_RD = 1;

	AD7606_OSl0 = 0;
	AD7606_OSl1 = 0;  
	AD7606_OSl2 = 0;
	AD7606_Reset();  // ��λ
	delay_ms(1);
	AD7606_Start();  // ����ת��
}


/*
*	�����ʱ��SYSTICK == 72MHz����
*						��ʱ����	��ʱ		���
*		Delay_ns(1):       17       236ns		500ns
*		Delay_ns(2):	   26       361ns		550ns
*		Delay_ns(10)							1.625us
*/
void AD7606_delay_ns(u32 t)
{
	while(--t); 
}


/* AD7606 ��λ */
void AD7606_Reset()
{
	AD7606_RESET = 0;
	AD7606_delay_ns(10);
	AD7606_RESET = 1;
	delay_us(1);
	AD7606_RESET = 0;
}


/* AD7606 ����ת�� */
void AD7606_Start()
{
	AD7606_COA = 0;
	AD7606_COB = 0;
	AD7606_delay_ns(100);
	AD7606_COA = 1;
	AD7606_COB = 1;
}


/*
*	AD7606 ��ȡ����
*	*data����ָ��Ϊָ��ṹ��������׵�ַ  
*/
void AD7606_Read(s16 *data)
{
	u8 i;
	
	AD7606_Start();  // ����ת��
	AD7606_delay_ns(1);
	
	while(AD7606_BUSY == 1)  // �ȴ�ת�����
	{
		AD7606_delay_ns(10);
	}
	
	AD7606_CS = 0;
	AD7606_delay_ns(20);
	for(i = 0; i < 8; i++)
	{
		AD7606_RD = 0;
		AD7606_delay_ns(25);
		AD7606_RD = 1;
		AD7606_delay_ns(25);
		data[i] = AD7606_ReadData();
	}
	AD7606_CS = 1;
}


/* ��ȡ�������� */
u16 AD7606_ReadData()
{
	u16 data = 0;
	
#if AD7606_FLAG == 0
	data <<= 1 + AD7606_DB15;
	data <<= 1 + AD7606_DB14;
	data <<= 1 + AD7606_DB13;
	data <<= 1 + AD7606_DB12;
	data <<= 1 + AD7606_DB11;
	data <<= 1 + AD7606_DB10;
	data <<= 1 + AD7606_DB9;
	data <<= 1 + AD7606_DB8;
	data <<= 1 + AD7606_DB7;
	data <<= 1 + AD7606_DB6;
	data <<= 1 + AD7606_DB5;
	data <<= 1 + AD7606_DB4;
	data <<= 1 + AD7606_DB3;
	data <<= 1 + AD7606_DB2;
	data <<= 1 + AD7606_DB1;
	data <<= 1 + AD7606_DB0;
#elif AD7606_FLAG == 1
	data = GPIO_ReadInputData(AD7606_DB_PORT);
#elif AD7606_FLAG == 2
	for(s8 i = 15; i >= 0; i--)
	{
		AD7606_RD = 0;
		AD7606_delay_ns(25);
		data += AD7606_DB << i;
		AD7606_RD = 1;
		AD7606_delay_ns(35);
	}

#endif
	
	return data;
}




/*
*	AD7606 ����������
*	rate������
*/
/******************************* ���������� ************************************
*  	OS2:   0		0		0		0		1		1		1		1
*  	OS1:   0		0		1		1		0		0		1		1
*  	OS0:   0		1		0		1		0		1		0		1
* 	����:  No OS	2		4		8		16		32		64		��Ч
*******************************************************************************/
void AD7606_Rate(u8 rate)
{
	switch(rate)
	{
		case(0):  AD7606_OSl2 = 0;  AD7606_OSl1 = 0;  AD7606_OSl0 = 0;  break;  // 200KHz
		case(2):  AD7606_OSl2 = 0;  AD7606_OSl1 = 0;  AD7606_OSl0 = 1;  break;  // 100KHz
		case(4):  AD7606_OSl2 = 0;  AD7606_OSl1 = 1;  AD7606_OSl0 = 0;  break;  // 50KHz
		case(8):  AD7606_OSl2 = 0;  AD7606_OSl1 = 1;  AD7606_OSl0 = 1;  break;  // 25KHz
		case(16):  AD7606_OSl2 = 1;  AD7606_OSl1 = 0;  AD7606_OSl0 = 0;  break;  // 12.5KHz
		case(32):  AD7606_OSl2 = 1;  AD7606_OSl1 = 0;  AD7606_OSl0 = 1;  break;  // 6.25KHz
		case(64):  AD7606_OSl2 = 1;  AD7606_OSl1 = 1;  AD7606_OSl0 = 0;  break;  // 3.125KHz
		default:  break;
	}
}


/*
*	AD7606 ��ѹת��
*	data����������
*	���ֵ����ʵ��ѹֵ
*/
double AD7606_Tran(u16 data, u8 flag)
{
	u8 fx = 0;
	double volt = 0.0;
	
	/* �ж����� */
	if((data & 32768) == 32768)
	{
		fx = 1;  // ��
	}
	else
	{
		fx = 0;  // ��
	}
	
	if(fx == 1)  // �㷨ѡ��  0--���ֵ  1--FFT 
	{
		if(flag == 0)  
		{
			volt = (double)((int)(((65535 - data + 1) * 0.0001526) * 10000) / 10000.0);
		}
		else
		{
			volt =  -(double)((int)(((65535 - data + 1) * 0.0001526) * 10000) / 10000.0);	 // ����ѹ,����ת�������
		}
	}
	else
	{
		volt = (double)((int)((data * 0.0001526) * 10000) / 10000.0);  // ֱ�Ӽ���
	}
	
	return volt;
}











