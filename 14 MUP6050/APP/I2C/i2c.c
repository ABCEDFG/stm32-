#include <i2c.h>
#include <systick.h>

void IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC|IIC_SDA_PORT_RCC,ENABLE);	//ʹ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin=IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}

void SDA_OUT()	//���ģʽ
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//�������
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

void SDA_IN(void)	//����ģʽ
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

void IIC_Start(void)	//��ʼ�ź�
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(5);	//Ҫ��ʱ��>4.7us
 	IIC_SDA=0;	
	delay_us(6);	//Ҫ��ʱ��>4us
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

void IIC_Stop(void)	//ֹͣ�ź�
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;
 	IIC_SCL=1; 
	delay_us(6); 	//Ҫ��ʱ��>4us
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(6);	//Ҫ��ʱ��>4us							   	
}

u8 IIC_Wait_Ack(void)	//�ȴ�Ӧ���ź� ���� 0�ɹ� 1ʧ��
{
	u8 tempTime=0;
	
	IIC_SDA=1;
	delay_us(1);
	SDA_IN();      //SDA����Ϊ����  	   
	IIC_SCL=1;
	delay_us(1);	 
	while(READ_SDA)	//�ȴ�Ӧ��
	{
		tempTime++;
		if(tempTime>250)	//�ȴ�Ӧ������ʱ�� ������ʧ��
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

void IIC_Ack(void)	//����Ӧ��
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(5);	//Ҫ��>4us
	IIC_SCL=0;
}

void IIC_NAck(void)	//������Ӧ��
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(5);	//Ҫ��>4us
	IIC_SCL=0;
}

void IIC_Send_Byte(u8 txd)	//����һ���ֽ�
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>0) //�����λ��ʼ����
		{
			IIC_SDA=1;
		}
		else
		{
			IIC_SDA=0;
        }
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
}

u8 IIC_Read_Byte(u8 ack)	//��ȡһ���ֽ� ack=1 ����Ӧ��   ack=0  ���ͷ�Ӧ��
{
	u8 i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    }
	else
	{
		IIC_Ack(); //����ACK   
	}
	return receive;
}

