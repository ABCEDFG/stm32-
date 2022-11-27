#include <pact.h>
#include <usart.h>
#include <systick.h>


u16 pact_a=0;  // 用于保存接收的数据
s16 pact_b=0;
s32 pact_c=0;


/* 协议发送 */
void Pact_Send(u16 a,s16 b,s32 c)
{
	u8  pactx[20]={0};
	u8 cnt=0;
	u16 sc=0;  //和校验
	u8 i;
	
	pactx[cnt++]=0xAA;  //帧头
	pactx[cnt++]=0xAA;  //帧头
	
	pactx[cnt++]=8;     //数据长度 字节
	
	/* 数据拆分 高位在前 */
//	pactx[cnt++]=BYET3(a);
//	pactx[cnt++]=BYET2(a);
	pactx[cnt++]=BYET1(a);
	pactx[cnt++]=BYET0(a);
	
//	pactx[cnt++]=BYET3(b);
//	pactx[cnt++]=BYET2(b);
	pactx[cnt++]=BYET1(b);
	pactx[cnt++]=BYET0(b);
	
	pactx[cnt++]=BYET3(c);
	pactx[cnt++]=BYET2(c);
	pactx[cnt++]=BYET1(c);
	pactx[cnt++]=BYET0(c);
	
	/* 和校验计算 */
	for(i=2;i<pactx[2]+3;i++)
	{
		sc+=pactx[i];
	}
	pactx[cnt++]=BYET0(sc);
	
	/* 发送 */
	for(i=0;i<cnt;i++)
	{
		USART_SendData(USART1, pactx[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	//等待数据发送结束
		delay_ms(10);
	}
}


/* 协议接收 */
void Pact_Receive(u8 data)
{
	static u8 i=0;
	static u8 receivex[20]={0};
	u8 x;
	u16 sc=0;
	
	if(i<2)  //帧头
	{
		if(data==0xAA)
		{
			i++;
		}
		else
		{
			i=0;
		}
	}
	else 
	{
		if(i==2)  //接收数据长度位
		{
			i=3;
			receivex[0]=data;
		}
		else
		{
			if(i<receivex[0]+4)  // 接收数据位和校验位
			{
				i++;
				receivex[i-3]=data;
			} 
			else
			{
				/* 计算校验位 */
				for(i=0;i<receivex[0]+1;i++)
				{
					sc+=receivex[i];
					printf(" %x",receivex[i]);
				}
				printf("Aa \r\n\r\n");
				sc=BYET0(sc);
				
				if(sc==receivex[i])  //校验
				{
					/* 数据复合 */
					x=1+2;  // 1+第一个数据的长度
					pact_a=0;
					for(i=0+1;i<x;i++)
					{
						pact_a<<=8;
						pact_a|=receivex[i];
					}
				
					x=x+2;  // x+第二个数据的长度
					pact_b=0;
					for(;i<x;i++)
					{
						pact_b<<=8;
						pact_b|=receivex[i];
					}
				
					x=x+4;  // x+第三给数据的长度
					pact_c=0;
					for(;i<x;i++)
					{
						pact_c<<=8;
						pact_c|=receivex[i];
					}
					i=0;
				}
				i=0;
			}
		}
	}
}


