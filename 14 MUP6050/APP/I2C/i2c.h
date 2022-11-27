#ifndef _i2c_H
#define _i2c_H

#include <system.h>

/*  IIC_SCL时钟端口、引脚定义 */
#define IIC_SCL_PORT 		GPIOA   
#define IIC_SCL_PIN 		(GPIO_Pin_11)
#define IIC_SCL_PORT_RCC	RCC_APB2Periph_GPIOA

/*  IIC_SDA时钟端口、引脚定义 */
#define IIC_SDA_PORT 		GPIOA  
#define IIC_SDA_PIN 		(GPIO_Pin_12)
#define IIC_SDA_PORT_RCC	RCC_APB2Periph_GPIOA

//IO操作函数	 
#define IIC_SCL    PAout(11) //SCL
#define IIC_SDA    PAout(12) //输出 SDA	 
#define READ_SDA   PAin(12)  //输入 SDA

//IIC所有操作函数
void IIC_Init(void);            //初始化IIC的IO口				 
void IIC_Start(void);			//发送IIC开始信号
void IIC_Stop(void);	  		//发送IIC停止信号
void IIC_Send_Byte(u8 txd);		//IIC发送一个字节
u8 IIC_Read_Byte(u8 ack);		//IIC读取一个字节
u8 IIC_Wait_Ack(void); 			//IIC等待ACK信号
void IIC_Ack(void);				//IIC发送ACK信号
void IIC_NAck(void);			//IIC不发送ACK信号

#endif
