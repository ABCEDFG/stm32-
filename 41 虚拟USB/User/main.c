#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"


int main()
{
	u8 i=0; 
	u16 len;	
	u16 times=0;    
	u8 usbstatus=0;
	
	SysTick_Init(72); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	
	delay_ms(1800);
	USB_Port_Set(0);  // USB先断开
	delay_ms(700);
	USB_Port_Set(1);  // USB再次连接
 	Set_USBClock();   // 配置 USB 时钟
 	USB_Interrupts_Config();  // USB 唤醒中断  USB 低优先级数据处理中断    
 	USB_Init();	 // 初始化 USB  
	
	while(1)
	{
		if(usbstatus!=bDeviceState)//USB连接状态发生了改变.
		{
			usbstatus=bDeviceState;//记录新的状态
			if(usbstatus==CONFIGURED)
			{
				usb_printf("USB连接成功！\r\n");
				LED1=0;//DS1亮
			}else
			{
				//usb_printf("USB连接失败！\r\n");
				LED1=1;//DS1灭
			}
		}
		if(USB_USART_RX_STA&0x8000)
		{					   
			len=USB_USART_RX_STA&0x3FFF;//得到此次接收到的数据长度
			usb_printf("\r\n您发送的消息长度为:%d，消息内容为：\r\n",len);
			for(i=0;i<len;i++)
			{
				USB_USART_SendData(USB_USART_RX_BUF[i]);//以字节方式,发送给USB 
			}
			usb_printf("\r\n\r\n");//插入换行
			USB_USART_RX_STA=0;
		}else
		{
			times++;
			if(times%200==0)usb_printf("请输入数据,以回车键结束\r\n");  
			if(times%30==0)LED1=!LED1;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
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
