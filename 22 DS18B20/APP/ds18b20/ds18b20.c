#include <ds18b20.h>
#include <SysTick.h>


u8 DS18B20_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DS18B20_RCC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=DS18B20_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(DS18B20_PORT,&GPIO_InitStructure);
	
	DS18B20_Reset();
	return DS18B20_Check();
}

//��DS18B20����
void DS18B20_IO_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DS18B20_PORT,&GPIO_InitStructure);
}

//��DS18B20���
void DS18B20_IO_OUT()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=DS18B20_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //�������
	GPIO_Init(DS18B20_PORT,&GPIO_InitStructure);
}

//��λDS18B20
void DS18B20_Reset()
{
	DS18B20_IO_OUT();  //
	DS18B20_OUT=0;  //����
	delay_us(750);  //����750us
	DS18B20_OUT=1;  //����
	delay_us(15);   //����15us
}

//���DS18B20�Ƿ����
// 0������   1��������
u8 DS18B20_Check()
{
	u8 t=0;
	DS18B20_IO_IN();
	while(DS18B20_IN && t<200)
	{
		t++;
		delay_us(1);
	}
	if(t>=200)
	{
		return 1;
	}
	else
	{
		t=0;
	}
	while(!DS18B20_IN && t<240)
	{
		t++;
		delay_us(1);
	}
	if(t>=240)
	{
		return 1;
	}
	return 0;
}

//��DS18B20��ȡһ��λ
u8 DS18B20_Read_Bit()
{
	u8 data;
	DS18B20_IO_OUT();
	DS18B20_OUT=0;
	delay_us(2);
	DS18B20_OUT=1;
	DS18B20_IO_IN();
	delay_us(12);
	if(DS18B20_IN)
	{
		data=1;
	}
	else
	{
		data=0;
	}
	delay_us(50);
	return data;
}

//��DS18B20��ȡһ���ֽ�
u8 DS18B20_Read_Byte()
{
	u8 i,j,dat;
	for(i=0;i<8;i++)
	{
		j=DS18B20_Read_Bit();
		dat=(j<<7) | (dat>>1);
	}
	return dat;
}

void DS18B20_Write_Byte(u8 dat)
{
	u8 i;
	u8 t;
	DS18B20_IO_OUT();
	for(i=0;i<8;i++)
	{
		t=dat&0x01;  //���λ
		dat=dat>>1;  //������һλ
		if(t==1)
		{
			DS18B20_OUT=0;  //д1
			delay_us(2);
			DS18B20_OUT=1;
			delay_us(60);
		}
		else
		{
			DS18B20_OUT=0;  //д0
			delay_us(60);
			DS18B20_OUT=1;
			delay_us(2);
		}
	}
}

//��ʼ�¶�ת��
void DS18B20_Start()
{
	DS18B20_Reset();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);  //���� rom
	DS18B20_Write_Byte(0x44);  //��ʼ�¶�ת��
}



float DS18B20_Get_Temp()
{
	u16 temp;
	u8 a,b;
	float val;
	DS18B20_Start();  //DS18B20 ��ʼת��
	DS18B20_Reset();  //����
	DS18B20_Check();  //����
	DS18B20_Write_Byte(0xcc);  //���� rom
	DS18B20_Write_Byte(0xbe);  //��ȡ�¶�ֵ
	a=DS18B20_Read_Byte(); // LSB   
    b=DS18B20_Read_Byte(); // MSB  
	temp=b;
	temp=(temp<<8)+a;
	if((temp & 0xf800) == 0xf800)  //�Ƿ� <0��
	{
		temp=(~temp)+1;
		val=temp*(-0.0625);
	}
	else
	{
		val=temp*0.0625;
	}
	return val;
}
