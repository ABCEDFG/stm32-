#include <pact.h>
#include <usart.h>
#include <systick.h>


u16 pact_a=0;  // ���ڱ�����յ�����
s16 pact_b=0;
s32 pact_c=0;


/* Э�鷢�� */
void Pact_Send(u16 a,s16 b,s32 c)
{
	u8  pactx[20]={0};
	u8 cnt=0;
	u16 sc=0;  //��У��
	u8 i;
	
	pactx[cnt++]=0xAA;  //֡ͷ
	pactx[cnt++]=0xAA;  //֡ͷ
	
	pactx[cnt++]=8;     //���ݳ��� �ֽ�
	
	/* ���ݲ�� ��λ��ǰ */
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
	
	/* ��У����� */
	for(i=2;i<pactx[2]+3;i++)
	{
		sc+=pactx[i];
	}
	pactx[cnt++]=BYET0(sc);
	
	/* ���� */
	for(i=0;i<cnt;i++)
	{
		USART_SendData(USART1, pactx[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	//�ȴ����ݷ��ͽ���
		delay_ms(10);
	}
}


/* Э����� */
void Pact_Receive(u8 data)
{
	static u8 i=0;
	static u8 receivex[20]={0};
	u8 x;
	u16 sc=0;
	
	if(i<2)  //֡ͷ
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
		if(i==2)  //�������ݳ���λ
		{
			i=3;
			receivex[0]=data;
		}
		else
		{
			if(i<receivex[0]+4)  // ��������λ��У��λ
			{
				i++;
				receivex[i-3]=data;
			} 
			else
			{
				/* ����У��λ */
				for(i=0;i<receivex[0]+1;i++)
				{
					sc+=receivex[i];
					printf(" %x",receivex[i]);
				}
				printf("Aa \r\n\r\n");
				sc=BYET0(sc);
				
				if(sc==receivex[i])  //У��
				{
					/* ���ݸ��� */
					x=1+2;  // 1+��һ�����ݵĳ���
					pact_a=0;
					for(i=0+1;i<x;i++)
					{
						pact_a<<=8;
						pact_a|=receivex[i];
					}
				
					x=x+2;  // x+�ڶ������ݵĳ���
					pact_b=0;
					for(;i<x;i++)
					{
						pact_b<<=8;
						pact_b|=receivex[i];
					}
				
					x=x+4;  // x+���������ݵĳ���
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


