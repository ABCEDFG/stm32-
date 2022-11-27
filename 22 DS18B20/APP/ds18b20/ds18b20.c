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

//从DS18B20输入
void DS18B20_IO_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DS18B20_PORT,&GPIO_InitStructure);
}

//向DS18B20输出
void DS18B20_IO_OUT()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=DS18B20_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //推挽输出
	GPIO_Init(DS18B20_PORT,&GPIO_InitStructure);
}

//复位DS18B20
void DS18B20_Reset()
{
	DS18B20_IO_OUT();  //
	DS18B20_OUT=0;  //拉低
	delay_us(750);  //拉低750us
	DS18B20_OUT=1;  //拉高
	delay_us(15);   //拉高15us
}

//检测DS18B20是否存在
// 0：存在   1：不存在
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

//从DS18B20读取一个位
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

//从DS18B20读取一个字节
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
		t=dat&0x01;  //最低位
		dat=dat>>1;  //向右移一位
		if(t==1)
		{
			DS18B20_OUT=0;  //写1
			delay_us(2);
			DS18B20_OUT=1;
			delay_us(60);
		}
		else
		{
			DS18B20_OUT=0;  //写0
			delay_us(60);
			DS18B20_OUT=1;
			delay_us(2);
		}
	}
}

//开始温度转换
void DS18B20_Start()
{
	DS18B20_Reset();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);  //跳过 rom
	DS18B20_Write_Byte(0x44);  //开始温度转换
}



float DS18B20_Get_Temp()
{
	u16 temp;
	u8 a,b;
	float val;
	DS18B20_Start();  //DS18B20 开始转换
	DS18B20_Reset();  //重启
	DS18B20_Check();  //检验
	DS18B20_Write_Byte(0xcc);  //跳过 rom
	DS18B20_Write_Byte(0xbe);  //读取温度值
	a=DS18B20_Read_Byte(); // LSB   
    b=DS18B20_Read_Byte(); // MSB  
	temp=b;
	temp=(temp<<8)+a;
	if((temp & 0xf800) == 0xf800)  //是否 <0°
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
