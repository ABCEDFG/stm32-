#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "can.h"


/* δ���� */

int main()
{
	u8 i=0, j; 
	u8 key;
	u8 mode=0;
	u8 res;
	u8 tbuf[8];
	u8 rbuf[8];
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	CANx_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//500Kbps������
	
	
	while(1)
	{
		key=Key_Scan(0);
		if(key==1)  // ģʽ�л�
		{
			mode=!mode;
			CANx_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);
			if(mode==0)
			{
				printf("Normal Mode\r\n");
			}
			else
			{
				printf("LoopBack Mode\r\n");
			}
		}
		else if(key==2)  // ��������
		{
			for(j=0;j<8;j++)
			{
				tbuf[j]=j;
			}
			res=CAN_Send_Msg(tbuf,8);
			if(res)
			{
				printf("Send Failed!\r\n");
			}
			else
			{
				printf("�������ݣ�");
				for(j=0;j<8;j++)
				{
					printf("%X  ",tbuf[j]);
				}
				printf("\r\n");
			}
		}
		
		/* ������Ϣ */
		res=CAN_Receive_Msg(rbuf);
		if(res)
		{	
			printf("�������ݣ�");
			for(j=0;j<8;j++)
			{
				printf("%X  ",rbuf[j]);
			}
			printf("\r\n");
		}
		
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
