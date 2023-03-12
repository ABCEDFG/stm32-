#include <i2c.h>
#include <systick.h>

void IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	//GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC|IIC_SDA_PORT_RCC,ENABLE);	//使能时钟
	
	GPIO_InitStructure.GPIO_Pin=IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}

void SDA_OUT()	//输出模式
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

void SDA_IN(void)	//输入模式
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

void IIC_Start(void)	//起始信号
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(5);	//要求时间>4.7us
 	IIC_SDA=0;	
	delay_us(6);	//要求时间>4us
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

void IIC_Stop(void)	//停止信号
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;
 	IIC_SCL=1; 
	delay_us(6); 	//要求时间>4us
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(6);	//要求时间>4us							   	
}

u8 IIC_Wait_Ack(void)	//等待应答信号 返回 0成功 1失败
{
	u8 tempTime=0;
	
	IIC_SDA=1;
	delay_us(1);
	SDA_IN();      //SDA设置为输入  	   
	IIC_SCL=1;
	delay_us(1);	 
	while(READ_SDA)	//等待应答
	{
		tempTime++;
		if(tempTime>250)	//等待应答的最大时间 超过则失败
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

void IIC_Ack(void)	//产生应答
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(5);	//要求>4us
	IIC_SCL=0;
}

void IIC_NAck(void)	//产生非应答
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(5);	//要求>4us
	IIC_SCL=0;
}

void IIC_Send_Byte(u8 txd)	//发送一个字节
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>0) //从最高位开始发送
		{
			IIC_SDA=1;
		}
		else
		{
			IIC_SDA=0;
        }
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
}

u8 IIC_Read_Byte(u8 ack)	//读取一个字节 ack=1 发送应答   ack=0  发送非应答
{
	u8 i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if(!ack)
	{
        IIC_NAck();//发送nACK
    }
	else
	{
		IIC_Ack(); //发送ACK   
	}
	return receive;
}

