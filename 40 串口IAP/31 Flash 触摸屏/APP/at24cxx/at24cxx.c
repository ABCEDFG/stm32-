#include <at24cxx.h>
#include <sysTick.h>

//开发板时使用的是AT24C02  256个字节的储存空间
//8字节的页缓冲区

void AT24CXX_Init(void)
{
	IIC_Init();//IIC初始化
	
	while(AT24CXX_Check())
	{
		//printf("AT24Cxx检测不正常！\r\n");
		delay_ms(500);
	}
	//printf("AT24Cxx检测正常！\r\n");
}

/* 在指定的地址读数 输入开始读数的地址 返回读到的数据 一个字节 */
u8 AT24CXX_ReadOneByte(u16 ReadAddr)	
{				  
	u8 temp=0;
	
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
		
	}
	else 
	{
		IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据
	}
	
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	
    
	IIC_Start();  	
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件
	
	return temp;
}

/* 向指定地址写入数据 一个字节*/
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)	
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}
	else 
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据
	} 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}

/* 
*  在AT24CXX里面的指定地址开始写入长度为 Len 位的数据 
*  Led取值：16 32
*/
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)	
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}


/* 
*  在AT24CXX里面的指定地址开始读出长度为 Len 位的数据 
*  Led取值：16 32
*/
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}

/* 检测AT24Cxx是否正常 0没问题  1有问题 */
u8 AT24CXX_Check(void)	
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255); // 避免每次开机都写AT24CXX			   
	if(temp==0x36)
		return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X36);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X36)return 0;
	}
	return 1;											  
}


/* 
*   在AT24CXX里面的指定地址开始读出指定个数的数据
*   ReadAddr:   开始读取的地址
*	pBuffer:    储存返回数据的地址
*	NumToRead:  数据数量
*   单位:       字节
*/
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)	
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 


/* 
*   在AT24CXX里面的指定地址开始读出指定个数的数据
*   ReadAddr:   开始读取的地址
*	pBuffer:    储存返回数据的地址
*	NumToWrite: 数据数量
*   单位:       字节
*/
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)	//在AT24CXX里面的指定地址开始写入指定个数的数据
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

