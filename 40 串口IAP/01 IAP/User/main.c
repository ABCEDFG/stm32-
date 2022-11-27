#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "stm32_flash.h"
#include "iap.h"



int main()
{
	u8 i = 0; 
	u8 key;
	u16 oldcount=0;				    //�ϵĴ��ڽ�������ֵ
	u32 applenth=0;				    //���յ���app���볤��
	u8 clearflag=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	Key_Init();
	
	
	
	while(1)
	{
		if(USART1_RX_CNT)
		{
			if(oldcount==USART1_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART1_RX_CNT;
				oldcount=0;
				USART1_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
			}
			else 
				oldcount=USART1_RX_CNT;			
		}
		
		i++;
		delay_ms(10);
		if(i==30)
		{
			LED1=!LED1;
			i=0;
			if(clearflag)
			{
				clearflag--;
				//if(clearflag==0)LCD_Fill(30,210,240,210+16,WHITE);//�����ʾ
			}
		}	
		
		key=Key_Scan(0);
		if(key==1)
		{
			if(applenth)
			{
				printf("��ʼ���¹̼�...\r\n");	
				//LCD_ShowString(30,210,200,16,16,"Copying APP2FLASH...");
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART1_RX_BUF,applenth);//����FLASH����   
					//LCD_ShowString(30,210,200,16,16,"Copy APP Successed!!");
					printf("�̼��������!\r\n");	
				}else 
				{
					//LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!  ");	   
					printf("��FLASHӦ�ó���!\r\n");
				}
 			}else 
			{
				printf("û�п��Ը��µĹ̼�!\r\n");
				//LCD_ShowString(30,210,200,16,16,"No APP!");
			}
			clearflag=7;  // ��־��������ʾ,��������7*300ms�������ʾ									 
		}
		
		if(key==2)
		{
			printf("��ʼִ��FLASH�û�����!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}else 
			{
				printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
				//LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");	   
			}									 
			clearflag=7;  // ��־��������ʾ,��������7*300ms�������ʾ	  
		}
		
		if(key==2)
		{
			printf("��ʼִ��SRAM�û�����!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//�ж��Ƿ�Ϊ0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM��ַ
			}else 
			{
				printf("��SRAMӦ�ó���,�޷�ִ��!\r\n");
				//LCD_ShowString(30,210,200,16,16,"Illegal SRAM APP!");	   
			}									 
			clearflag=7;  // ��־��������ʾ,��������7*300ms�������ʾ	 
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
