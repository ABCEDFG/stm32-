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
	u16 oldcount=0;				    //老的串口接收数据值
	u32 applenth=0;				    //接收到的app代码长度
	u8 clearflag=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	Key_Init();
	
	
	
	while(1)
	{
		if(USART1_RX_CNT)
		{
			if(oldcount==USART1_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART1_RX_CNT;
				oldcount=0;
				USART1_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
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
				//if(clearflag==0)LCD_Fill(30,210,240,210+16,WHITE);//清除显示
			}
		}	
		
		key=Key_Scan(0);
		if(key==1)
		{
			if(applenth)
			{
				printf("开始更新固件...\r\n");	
				//LCD_ShowString(30,210,200,16,16,"Copying APP2FLASH...");
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART1_RX_BUF,applenth);//更新FLASH代码   
					//LCD_ShowString(30,210,200,16,16,"Copy APP Successed!!");
					printf("固件更新完成!\r\n");	
				}else 
				{
					//LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!  ");	   
					printf("非FLASH应用程序!\r\n");
				}
 			}else 
			{
				printf("没有可以更新的固件!\r\n");
				//LCD_ShowString(30,210,200,16,16,"No APP!");
			}
			clearflag=7;  // 标志更新了显示,并且设置7*300ms后清除显示									 
		}
		
		if(key==2)
		{
			printf("开始执行FLASH用户代码!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}else 
			{
				printf("非FLASH应用程序,无法执行!\r\n");
				//LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");	   
			}									 
			clearflag=7;  // 标志更新了显示,并且设置7*300ms后清除显示	  
		}
		
		if(key==2)
		{
			printf("开始执行SRAM用户代码!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//判断是否为0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM地址
			}else 
			{
				printf("非SRAM应用程序,无法执行!\r\n");
				//LCD_ShowString(30,210,200,16,16,"Illegal SRAM APP!");	   
			}									 
			clearflag=7;  // 标志更新了显示,并且设置7*300ms后清除显示	 
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
