#include <i2c.h>
#include <sysTick.h>



void IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	//GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC|IIC_SDA_PORT_RCC,ENABLE);	//使能时钟
	
	GPIO_InitStructure.GPIO_Pin=IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //开漏输出
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}


/* 输出模式 */
void SDA_OUT()	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}


/* 输入模式 */
void SDA_IN(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

/* 起始信号 */
void IIC_Start(void)	
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(5);	//要求时间>4.7us
 	IIC_SDA=0;	
	delay_us(6);	//要求时间>4us
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}


/* 停止信号 */
void IIC_Stop(void)	
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;
 	IIC_SCL=1; 
	delay_us(6); 	//要求时间>4us
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(6);	//要求时间>4us							   	
}


/* 
*  等待应答信号 
*  返回 0 失败   1 成功 
*/
u8 IIC_Wait_Ack(void)	
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
			return 0;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 1;  
} 


/* 产生应答 */
void IIC_Ack(void)	
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(5);	//要求>4us
	IIC_SCL=0;
}


/* 产生非应答 */
void IIC_NAck(void)	
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(5);	//要求>4us
	IIC_SCL=0;
}


/* 发送一个字节 */
void IIC_Send_Byte(u8 txd)	
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


/*  
*   读取一个字节 
*   ack=1  发送应答   
*   ack=0  发送非应答 
*/
u8 IIC_Read_Byte(u8 ack)	
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
