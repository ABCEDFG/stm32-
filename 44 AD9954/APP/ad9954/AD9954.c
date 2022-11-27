#include "ad9954.h"
#include "systick.h"


/* ģ��ʱ����� */
#define  AD9954_FOSC  20  // ����Ƶ�ʣ��ⲿ����Ƶ�ʣ�
#define  AD9954_PLL_MULTIPLIER  19  // PLL ��Ƶ����4--20��
#define  AD9954_FS  (AD9954_FOSC * AD9954_PLL_MULTIPLIER)  // ϵͳʱ��Ƶ��


double f_num = 11.3025455157895;  // Ƶ�����  �������ã�����������������������������������������


/*
*	AD9954 �� GPIO ��ʼ��
*	��Ϊ AD9954 ��������ٶ�Ϊ 25MHz���� GPIO �ٶ� < 25MHz, 
*	�ʱ������������ʱ��us ����
*/
void AD9954_GPIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(AD9954_CS_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_SCLK_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_SDIO_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_OSK_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_PS0_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_PS1_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_UPD_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_SDO_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_IOSY_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_RES_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_PWR_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_OSK_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_OSK_PORT, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = AD9954_CS_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_CS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_SCLK_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_SCLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_SDIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_SDIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_PS0_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_PS0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_UPD_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_UPD_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_RES_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_RES_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_PS1_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_PS1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_IOSY_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_IOSY_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_PWR_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_PWR_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_SDO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	  // �������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_SDO_PORT, &GPIO_InitStructure);
	
	AD9954_IOSY = 0;
	AD9954_OSK = 0;
	AD9954_PWR = 0;
}


/*
*	��λ AD9954
*	����λҲ����
*/
void AD9954_Reset()
{
	AD9954_CS = 0;
	AD9954_RES = 0;
	AD9954_RES = 1;
	delay_ms(100);
	AD9954_RES = 0;
	AD9954_CS = 0;
	AD9954_SCLK = 0;
	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_UPD = 0;
	AD9954_CS = 1;
}


/*
*	�� AD9954 ����һ���ֽ�
*	AD9954�Ĵ����ٶ����Ϊ25M�����Բ�����ʱҲ����
*	data: �跢�͵��ֽ�
*/
void AD9954_Send_Byte(u8 data)
{
	u8 i;
	
	for (i = 0; i < 8; i++)
	{
		AD9954_SCLK = 0;
		if (data & 0x80)
		{
			AD9954_SDIO = 1;
		}
		else
		{
			AD9954_SDIO = 0;
		}
		AD9954_SCLK = 1;
		data <<= 1;
	}
}


/* �� AD9954 ��ȡһ���ֽ� */
u8 AD9954_Read_Byte()
{
	u8 i;
	u8 data = 0;
	
	for(i = 0; i < 8; i++)
	{
		AD9954_SCLK = 0;
		data |= AD9954_SDO;
		AD9954_SCLK = 1;
		data <<= 1;  // �����Ƿ���Ҫ�ŵ�ǰ��  ����������������������������������������������������
	}
	
	return data;
}


/* AD9954 ��ʼ�� */
void AD9954_Init()
{
	AD9954_GPIO_Init();
	AD9954_Reset();  // ��λ AD9954
	delay_ms(300);
	AD9954_CS = 0;
	
	/* ��Ƶģʽ */
	AD9954_Send_Byte(CFR1);  // ��ַ 0 д����
	AD9954_Send_Byte(0x02);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	//AD9954_Send_Byte(0x40);  // �Ƚ����ϵ�
	AD9954_Send_Byte(0x00);
	
	AD9954_Send_Byte(CFR2);  // ��ַ 1 д����
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	
#if AD9954_FS > 400
	#error "ϵͳƵ�ʳ���оƬ���ֵ"  
#elseif AD9954_FS >= 250
	AD9954_Send_Byte(AD9954_PLL_MULTIPLIER << 3 | 0x04 | 0x03);  
#else
	AD9954_Send_Byte(AD9954_PLL_MULTIPLIER << 3); 
#endif
	
	//AD9954_Send_Byte(0x24);  // �ı�Ƶ���� VCO ���Ƹ�λ��ϵͳʱ�ӱ�Ƶ��Ϊ 400MHz
	AD9954_CS = 1;
}


/*
*	������ʵֵ�õ�Ƶ�ʣ���Ҫ���� AD9954 ��ֵ��
*	f_real����ʵƵ��ֵ
*/
u32 AD9954_Get_Ftw(double f_real)
{
	return (u32)(f_real * f_num);
}


/*
*	����һ�������źţ����� AD9954 �ڲ��Ĵ���
*	���Բ����κ���ʱ
*/
void AD9954_Upadte()
{
	AD9954_UPD = 0;
	AD9954_UPD = 1;
	//delay_us(200);
	AD9954_UPD = 0;
}


/* 
*	���� AD9954 ��Ƶ�����
*	��Ϊʹ�õĸ��������㣬ת�������л������
*	ͨ������ f_num ��ֵ	���Ծ�ȷ�� 0.1 Hz����
*	f����ʵƵ��ֵ
*/
void AD9954_Set_Fre(double f)
{
	u32 f_data;
	
	AD9954_CS = 0;
	f_data = AD9954_Get_Ftw(f);  // �õ�����ֵ
	AD9954_Send_Byte(FTW0);  // FTW0 ��ַ
	AD9954_Send_Byte((u8)(f_data >> 24));//Ƶ�ʿ�����
	AD9954_Send_Byte((u8)(f_data >> 16));
	AD9954_Send_Byte((u8)(f_data >> 8));
	AD9954_Send_Byte((u8)f_data);
	AD9954_CS = 1;
	AD9954_Upadte();
}


/* ���Ĵ��� CFR1 */
u32 AD9954_Read_Vau1()
{
	u8 i;
	u32 data = 0;
	
	AD9954_CS = 1;     // �����ź��ǲ���д���� ����������������������������������������������������������
	AD9954_Send_Byte(CFR1);
	for(i = 0; i < 4; i++)
	{
		data <<= 8;
		data += AD9954_Read_Byte();
	}
	AD9954_CS = 1;    // �����ź��ǲ���д���� ����������������������������������������������������������
	
	return data;
}


/* ���Ĵ��� CFR2 */
u32 AD9954_Read_Vau2()
{
	u8 i;
	u32 data = 0;
	
	AD9954_CS = 0;  // �����ź��ǲ���д���� ����������������������������������������������������������
	AD9954_Send_Byte(CFR2);
	for(i = 0; i < 3; i++)
	{
		data <<= 8;
		data += AD9954_Read_Byte();
	}
	AD9954_CS = 1;  // �����ź��ǲ���д���� ����������������������������������������������������������
	
	return data;
}


// �⺯����ʲô�� ?????????????????????????????????????????????????????????????????????????????????????????????????????????????????
void AD9954_PSK(float f, float phase1, float phase2, float phase3, float phase4)
{
	u16 date;
	
	AD9954_CS = 0;
	AD9954_UPD = 0;
	delay_us(1);
	AD9954_Send_Byte(0x00);  // ��ַ0 д����
	AD9954_Send_Byte(0x00);  // �� RAM ����λ���� FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	//
	AD9954_Send_Byte(0x01);  // ��ַ1д����
	AD9954_Send_Byte(0x00);  
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0xA4);  // 4 ��Ƶ���� VCO ���Ƹ�λ��ϵͳʱ�ӱ�Ƶ��Ϊ 400M
	AD9954_Set_Fre(f);  // �ز�Ƶ��
	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_Send_Byte(0x07);
	AD9954_Send_Byte(0x01);  //ramp rate=0x0010
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);  // final address:0x000
	AD9954_Send_Byte(0x00);  // start address:0x000;
	AD9954_Send_Byte(0x00);  // RAM0 ������ģʽ 0,��ͣ��λû�м���
	AD9954_Upadte();
	date = 45.51*phase1;
	date = date << 2;
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte((u8)(date >> 8));  // Ƶ�ʿ�����
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);

	AD9954_PS0 = 1;//ram1
	AD9954_PS1 = 0;

	AD9954_Send_Byte(0x08);
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x01);  // final address:0x0001
	AD9954_Send_Byte(0x04);  // start address:0x0001
	AD9954_Send_Byte(0x00);  // RAM1 ������ģʽ 0,��ͣ��λû�м���
	AD9954_Upadte();
	date = 45.51*phase2;
	date = date << 2;
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte((u8)(date >> 8));  // Ƶ�ʿ�����
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);

	AD9954_PS0 = 0;  // ram2
	AD9954_PS1 = 1;

	AD9954_Send_Byte(0x09);
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x02);  // final address:0x0002
	AD9954_Send_Byte(0x08);  // start address:0x0002
	AD9954_Send_Byte(0x00);
	AD9954_Upadte();
	date = 45.51*phase3;
	date = date << 2;
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte((u8)(date >> 8));  // Ƶ�ʿ�����
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_PS0 = 1;//ram3
	AD9954_PS1 = 1;
	AD9954_Send_Byte(0x0a);
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x03);  // final address:0x0003
	AD9954_Send_Byte(0x0c);  // start address:0x0003
	AD9954_Send_Byte(0x00);
	AD9954_Upadte();
	date = 45.51*phase4;
	date = date << 2;
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte((u8)(date >> 8));  // Ƶ�ʿ�����
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);

	AD9954_Send_Byte(0x00);  // ��ַ0д����
	AD9954_Send_Byte(0xa0);  // ��RAM����λ����FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	PSK �����ź����
*	ͨ�� RAM ���������źŵ���λ���Ӷ�ʵ�� PSK �źŵĵ���
*   ����ʹ��һ�� RAM �˵�ѭ��ģʽ��PSK �ź�Ϊ 8 λ����λ��
*   RAM �ĸ����ٶ��� ram rate ��ʱ�Ĵ������ƣ��Ĵ���Ϊ 16 λ��ʵ�ֶ�ϵͳ�ṩ�� 100M ʱ�� 1~65535 ��Ƶ��
*	д0����Ч�ģ�������� PSK �ز���Ƶ���� ram rate ����Ƶ�ʵ�������ʱ�����ܱ�֤ÿ�ε���λ�ı�һ��
*	f��      PSK �ز���Ƶ��
*	*phase�� д����λ�ۼ�������λֵ��180--��λ��ת��0--���ı���λ��
*			 PSK �ź�Ϊ 8 λ����Ϊ 8 λ����ֵ
*/
void AD9954_Generate_PSK(float f, u16 *phase)
{
	u8 i;
	u16 date;
	AD9954_CS = 0;
	AD9954_UPD = 0;

	AD9954_Send_Byte(0x00);  // ��ַ 0 д����
	AD9954_Send_Byte(0x00);  // �� RAM ����λ���� FTW
	AD9954_Send_Byte(0x00); 
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);

	AD9954_Send_Byte(0x01);  // ��ַ 1 д����
	AD9954_Send_Byte(0x00);  //
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0xA4);  // 4 ��Ƶ���� VCO ���Ƹ�λ��ϵͳʱ�ӱ�Ƶ��Ϊ 400M
	AD9954_Set_Fre(f);  // �ز�Ƶ��

	AD9954_Send_Byte(0x04);  // FTW0 ��ַ
	AD9954_Send_Byte(0x00);  // Ƶ�ʿ�����
	AD9954_Send_Byte(0x10);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Upadte();

	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_Send_Byte(0x07);
	AD9954_Send_Byte(0x00);  // ramp rate=0x0400
	AD9954_Send_Byte(0x04);
	AD9954_Send_Byte(0x07);  // final address:0x007
	AD9954_Send_Byte(0x00);  // start address:0x000;
	AD9954_Send_Byte(0x80);  // RAM0 ������ģʽ 4,��ͣ��λû�м���
	AD9954_Upadte();

	AD9954_Send_Byte(0x0b);
	for (i = 0;i<8;i++)
	{
		date = 45.51*phase[i];
		date = date << 2;
		AD9954_Send_Byte((u8)(date >> 8));  // Ƶ�ʿ�����
		AD9954_Send_Byte((u8)date);
		AD9954_Send_Byte(0x00);
		AD9954_Send_Byte(0x00);
	}
	AD9954_Send_Byte(0x00);  // ��ַ 0 д����
	AD9954_Send_Byte(0xc0);  // �� RAM ����λ���� FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	FM �����ź����
*	ͨ�� RAM ���������źŵ�Ƶ�ʣ��Ӷ�ʵ�� FM �źŵĵ���
*	����ʹ��һ�� RAM �ε�ѭ��ģʽ��PSK �ź�Ϊ 8 λ����λ
*	RAM �ĸ����ٶ��� ram rate ��ʱ�Ĵ������ƣ��Ĵ���λ 16 λ��ʵ�ֶ�ϵͳ�ṩ�� 100M ʱ�� 1~65535 ��Ƶ��
*	ram rate �ĸ����ٶ�Ϊ�����źŵ�Ƶ�ʣ������� 1KHz, д�� ram rate ��ֵΪ 0x0C35,
*	*fre��FM �ź�Ƶ��ֵ��������õ��� 32 ��Ƶ�ʲ��������ô˺���֮ǰ�������ú�Ƶ�ʱ��ֵ���䰴���ҹ��ɸı�
*	for(i = 0; i < 32; i++)
*	{
*		fre[i] = Fc + Fshift * sin(wt);
*	}
*/
void AD9954_Generate_FM(u32 *fre)
{
	u8 i;
	u32 date;
	
	AD9954_CS = 0;
	AD9954_UPD = 0;

	AD9954_Send_Byte(0x00);  // ��ַ 0 д����
	AD9954_Send_Byte(0x00);  // �� RAM ����λ����FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);

	AD9954_Send_Byte(0x01);  // ��ַ 1 д����
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x24);  // 4 ��Ƶ���� VCO ���Ƹ�λ��ϵͳʱ�ӱ�Ƶ��Ϊ 400M

	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_Send_Byte(0x07);
	AD9954_Send_Byte(0x35);  // ramp rate=32kHz
	AD9954_Send_Byte(0x0C);
	AD9954_Send_Byte(0x1F);  // final address:0x000
	AD9954_Send_Byte(0x00);  // start address:0x000;
	AD9954_Send_Byte(0x80);  // RAM0 ������ģʽ 0,��ͣ��λû�м���
	AD9954_Upadte();

	AD9954_Send_Byte(0x0b);
	for (i = 0; i < 32; i++)
	{
		date = 10.73741 * fre[i];
		AD9954_Send_Byte((u8)(date >> 24));  // Ƶ�ʿ�����
		AD9954_Send_Byte((u8)(date >> 16));
		AD9954_Send_Byte((u8)(date >> 8));
		AD9954_Send_Byte((u8)date);
	}
	AD9954_Send_Byte(0x00);  // ��ַ 0 д����
	AD9954_Send_Byte(0x80);  //�� RAM ����λ���� FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	ASK �����ź������ʼ��
*	ASK ���ƣ���Ҫ�����úõ��ز�Ƶ�ʣ�Ȼ��ı� DAC �������� scale factor ��Ϊ 14 λ��
*	ͨ������Ϊ���� 0 ����ֵʵ�� ASK �źŵĵ��ƣ����� Write_ASF(u16 factor) �ı�
*/
void AD9954_ASK_Init()
{
	AD9954_CS = 0;
	AD9954_Send_Byte(0x00);  // ��ַ0д����
	AD9954_Send_Byte(0x02);  // �ֶ���OSK��
	AD9954_Send_Byte(0x00);

	AD9954_Send_Byte(0x20);  // ���ܵ�UPDATE�źţ��Զ������λ�ۼ���		//
//	AD9954_Send_Byte(0x00);  // ���ı��κ�
	AD9954_Send_Byte(0x40);

	// ������λƫ�ƣ���Ĭ����������Ҳ��������������λ�ۼ������ۼ���λΪ270
	AD9954_Send_Byte(0X05);
	AD9954_Send_Byte(0X30);
	AD9954_Send_Byte(0X00);

	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	AM �����ź������ʼ��
*	AM ���ƣ���Ҫ�����ú��ز�Ƶ�ʣ�Ȼ��ı� DAC �������� scale factor ��Ϊ 14 λ��
*	AM �ķ���ֵ�����ҹ��ɱ任������ʵ�� AM����߿ɲ��� 32 �����
*	���� AD9954_Write_ASF(u16 factor) ���ı��ֵ 
*/
void AD9954_AM_Init()
{
	AD9954_CS = 0;
	AD9954_Send_Byte(0x00);  // ��ַ 0 д����
	AD9954_Send_Byte(0x02);  // �ֶ��� OSK ��
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);  // ���ı��κ�
	AD9954_Send_Byte(0x40);
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	�ı� scale factor ��ֵ���ı� DAC �������
*	factor��Ҫ�ĳɵ���ֵ
*	��Χ�� 0 ~ 0x3fff
*/
void AD9954_Write_ASF(u16 factor)
{
	AD9954_CS = 0;
	AD9954_Send_Byte(0x02);  // ����	
	AD9954_Send_Byte(factor >> 8);
	AD9954_Send_Byte(factor);
	AD9954_CS = 1;
	
	AD9954_Upadte();
}


/*
*	AD9954 ɨƵģʽ���
*	MinFreq��ɨƵ����Ƶ�ʣ�
*	MaxFreq��ɨƵ����Ƶ��,
*	UpStepFreq������ɨƵ������
*	UpStepTime������ɨƵ����Ƶʱ��,
*	DownStepFreq������ɨƵ������
*	DownStepTime������ɨƵ����Ƶʱ��,
*	Mode��ɨƵģʽ  
*		0--����ɨƵģʽ��
*		1--����ɨƵģʽ��
*		2--˫��ɨƵ
*	��Ƶ����صĲ����ĵ�λ���� MHz
*	��˫��ɨ���ǣ�AD9954_PS0 = 1����Ϊ������ɨƵ��
*	������������ɨƵ�ļĴ�������ֱ��ͨ�� AD9954_PS0_WriteBit() �ı�ɨƵ����
*/
void AD9954_Linear_Sweep(double  MinFreq, double MaxFreq, double UpStepFreq, u8 UpStepTime, 
						 double DownStepFreq, u8 DownStepTime, ScanMode Mode)
{
	u32 FTW_Vau;
    
	AD9954_PS0 = 0;
    AD9954_PS1 = 0;
    AD9954_CS = 0;
    AD9954_Send_Byte(CFR1);
    AD9954_Send_Byte(0x02);
    AD9954_Send_Byte(0x20);  // Linear_Sweep Enable
    AD9954_Send_Byte(0);
	
    if (Mode & No_Dwell)
        AD9954_Send_Byte(0x44);  // Comp Power-Down & Linear Sweep No Dwell
    else
        AD9954_Send_Byte(0x40);  // Comp Power-Down
    
    /* д��FTW0----��СƵ�� */
    FTW_Vau =  AD9954_Get_Ftw(MinFreq);
    AD9954_Send_Byte(FTW0);
    AD9954_Send_Byte(FTW_Vau >> 24);
    AD9954_Send_Byte(FTW_Vau >> 16);
    AD9954_Send_Byte(FTW_Vau >> 8);
    AD9954_Send_Byte(FTW_Vau);
    
    /* д��FTW1----���Ƶ�� */
    FTW_Vau = AD9954_Get_Ftw(MaxFreq);
    AD9954_Send_Byte(FTW1);
    AD9954_Send_Byte(FTW_Vau>>24);
    AD9954_Send_Byte(FTW_Vau>>16);
    AD9954_Send_Byte(FTW_Vau>>8);
    AD9954_Send_Byte(FTW_Vau);
    
    Mode &= 0x7F;
    
    if(Mode != UpScan)
    {
        /* д��NLSCW----�½�Ƶ�ʲ�������Ƶʱ�� */
        FTW_Vau = AD9954_Get_Ftw(DownStepFreq);
        AD9954_Send_Byte(NLSCW);
        AD9954_Send_Byte(DownStepTime); // ��Ƶʱ�䣨DownStepTime���������ڣ�     
        AD9954_Send_Byte(FTW_Vau>>24);  // Ƶ�ʲ���
        AD9954_Send_Byte(FTW_Vau>>16);
        AD9954_Send_Byte(FTW_Vau>>8);
        AD9954_Send_Byte(FTW_Vau);
        AD9954_PS0 = 0;  // �½�ɨƵ
    }
    if (Mode != DownScan)
    {
        /* д��PLSCW----����Ƶ�ʲ�������Ƶʱ�� */
        FTW_Vau = AD9954_Get_Ftw(UpStepFreq);
        AD9954_Send_Byte(PLSCW);
        AD9954_Send_Byte(UpStepTime);  // ��Ƶʱ�䣨0XFF���������ڣ�
        AD9954_Send_Byte(FTW_Vau>>24); // Ƶ�ʲ���
        AD9954_Send_Byte(FTW_Vau>>16);
        AD9954_Send_Byte(FTW_Vau>>8);
        AD9954_Send_Byte(FTW_Vau);
        AD9954_PS0 = 1;  // ����ɨƵ
    }
    AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	ͨ���ı� PS0 ״̬���ı� AD9954 ɨƵ����
*	flag��PS0 ״̬
*/
void AD9954_PS0_WriteBit(BitAction flag)
{
	AD9954_PS0 = flag;
}


/*
*	���� FSK �ź����
*	���ĸ� RAM ������һ��Ƶ��ֵ��ͨ���ı� PS0 �� PS1 �ĵ�ƽѡ���Ӧ�� RAM �������Ӧ��Ƶ��ֵ��ʵ�� FSK��
*	Ҳ��ʵ�ֶ���� FSK��ͨ�����ö�ʱ���жϿ��� PS0, PS1 �ܽŵĵ�ƽ�Ϳ��Խ������Ƶı���ת��Ϊ FSK �ź����
*/
void AD9954_FSK(double f1, double f2, double f3, double f4)
{
	u32 FTW_Vau;
	AD9954_CS = 0;

	AD9954_Send_Byte(0x00);  // ��ַ 0 д����   
	AD9954_Send_Byte(0x00);  // �� RAM ����λ���� FTW   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x40);   

	AD9954_Send_Byte(0x01);  // ��ַ 1 д����   
	AD9954_Send_Byte(0x00);  //   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0xA4);  // 8 ��Ƶ���� VCO ���Ƹ�λ��ϵͳʱ�ӱ�Ƶ��Ϊ 400M   
	AD9954_PS0 = 0;   
	AD9954_PS1 = 0;   
	AD9954_Send_Byte(0x07);   
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x00);  // final address:0x000   
	AD9954_Send_Byte(0x00);  // start address:0x000;   
	AD9954_Send_Byte(0x00);  // RAM0������ģʽ0,��ͣ��λû�м���   
	AD9954_Upadte();
	
	FTW_Vau = AD9954_Get_Ftw(f1);
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte(FTW_Vau>>24);  // Ƶ�ʲ���
	AD9954_Send_Byte(FTW_Vau>>16);
	AD9954_Send_Byte(FTW_Vau>>8);
	AD9954_Send_Byte(FTW_Vau);

	AD9954_PS0 = 1;  // ram1   
	AD9954_PS1 = 0;   

	AD9954_Send_Byte(0x08);   
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x01);  // final address:0x0001   
	AD9954_Send_Byte(0x04);  // start address:0x0001   
	AD9954_Send_Byte(0x00);  // RAM1 ������ģʽ 0,��ͣ��λû�м���   
	AD9954_Upadte();
	
	FTW_Vau = AD9954_Get_Ftw(f2);   
	AD9954_Send_Byte(0x0b);   
	AD9954_Send_Byte(FTW_Vau>>24);  // Ƶ�ʲ���
	AD9954_Send_Byte(FTW_Vau>>16);
	AD9954_Send_Byte(FTW_Vau>>8);
	AD9954_Send_Byte(FTW_Vau);  

	AD9954_PS0 = 0;  // ram2   
	AD9954_PS1 = 1;   

	AD9954_Send_Byte(0x09);   
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x02);  // final address:0x0002   
	AD9954_Send_Byte(0x08);  // start address:0x0002   
	AD9954_Send_Byte(0x00);   
	AD9954_Upadte();
	
	FTW_Vau = AD9954_Get_Ftw(f3);   
	AD9954_Send_Byte(0x0b);   
	AD9954_Send_Byte(FTW_Vau>>24);  // Ƶ�ʲ���
	AD9954_Send_Byte(FTW_Vau>>16);
	AD9954_Send_Byte(FTW_Vau>>8);
	AD9954_Send_Byte(FTW_Vau); 
	AD9954_PS0 = 1;  // ram3   
	AD9954_PS1 = 1;   
	AD9954_Send_Byte(0x0a);   
	AD9954_Send_Byte(0x01); // ramp rate=0x0010   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x03); // final address:0x0003   
	AD9954_Send_Byte(0x0c); // start address:0x0003   
	AD9954_Send_Byte(0x00);     
	AD9954_Upadte();
	
	FTW_Vau = AD9954_Get_Ftw(f4);   
	AD9954_Send_Byte(0x0b);   
	AD9954_Send_Byte(FTW_Vau>>24);  // Ƶ�ʲ���
	AD9954_Send_Byte(FTW_Vau>>16);
	AD9954_Send_Byte(FTW_Vau>>8);
	AD9954_Send_Byte(FTW_Vau); 

	AD9954_Send_Byte(0x00); // ��ַ 0 д����   
	AD9954_Send_Byte(0x80); // �� RAM ����λ���� FTW   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x40);   
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	����ɨ�����ģʽ
*	�� f1 ɨ�� f2
*	f1: ��ʼƵ��
*	f2: ��ֹƵ��
*/
void AD9954_Line_Sweep(float f1, float f2)
{
	u32 date;
	
	AD9954_SCLK = 0;
	AD9954_RES = 0;
	AD9954_UPD = 0;
	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_CS = 0;

	/* ����ɨ��ģʽ */
	AD9954_Send_Byte(0x00);  // ��ַ 0 д����
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x20);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	

	AD9954_Send_Byte(0x01);  // ��ַ 1 д����
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
//	AD9954_Send_Byte(0x24);  // 4 ��Ƶ���� VCO ���Ƹ�λ��ϵͳʱ�ӱ�Ƶ��Ϊ 400M
	AD9954_Send_Byte(AD9954_PLL_MULTIPLIER << 3 | 0x04);
	
	date = 10.7374 * f1;
	AD9954_Send_Byte(0x04);  // FTW0 ��ַ
	AD9954_Send_Byte((u8)(date>>24));  // Ƶ�ʿ�����
	AD9954_Send_Byte((u8)(date>>16));
	AD9954_Send_Byte((u8)(date>>8));
	AD9954_Send_Byte((u8)date);
	
	date = 10.7374 * f2;
	AD9954_Send_Byte(0x06);  // FTW1 ��ַ
	AD9954_Send_Byte((u8)(date >> 24));  // Ƶ�ʿ�����
	AD9954_Send_Byte((u8)(date >> 16));
	AD9954_Send_Byte((u8)(date >> 8));
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x07);  // NLSCW
	AD9954_Send_Byte(0x01);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x1b);
	AD9954_Send_Byte(0xf4);
	AD9954_Send_Byte(0x08);  // RLSCW
	AD9954_Send_Byte(0x01);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x01);
	AD9954_Send_Byte(0xa3);
	AD9954_Send_Byte(0x6f);
	
	AD9954_CS=1;
	AD9954_Upadte();
}


















