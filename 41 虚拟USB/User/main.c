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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	
	delay_ms(1800);
	USB_Port_Set(0);  // USB�ȶϿ�
	delay_ms(700);
	USB_Port_Set(1);  // USB�ٴ�����
 	Set_USBClock();   // ���� USB ʱ��
 	USB_Interrupts_Config();  // USB �����ж�  USB �����ȼ����ݴ����ж�    
 	USB_Init();	 // ��ʼ�� USB  
	
	while(1)
	{
		if(usbstatus!=bDeviceState)//USB����״̬�����˸ı�.
		{
			usbstatus=bDeviceState;//��¼�µ�״̬
			if(usbstatus==CONFIGURED)
			{
				usb_printf("USB���ӳɹ���\r\n");
				LED1=0;//DS1��
			}else
			{
				//usb_printf("USB����ʧ�ܣ�\r\n");
				LED1=1;//DS1��
			}
		}
		if(USB_USART_RX_STA&0x8000)
		{					   
			len=USB_USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
			usb_printf("\r\n�����͵���Ϣ����Ϊ:%d����Ϣ����Ϊ��\r\n",len);
			for(i=0;i<len;i++)
			{
				USB_USART_SendData(USB_USART_RX_BUF[i]);//���ֽڷ�ʽ,���͸�USB 
			}
			usb_printf("\r\n\r\n");//���뻻��
			USB_USART_RX_STA=0;
		}else
		{
			times++;
			if(times%200==0)usb_printf("����������,�Իس�������\r\n");  
			if(times%30==0)LED1=!LED1;//��˸LED,��ʾϵͳ��������.
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
