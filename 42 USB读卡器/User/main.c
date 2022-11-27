#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h" 
#include "mass_mal.h"
#include "malloc.h"
#include "sd_sdio.h"


extern u8 Max_Lun;	// ֧�ֵĴ��̸���,0��ʾ1��,1��ʾ2��


int main()
{
	u8 i=0; 
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA;
	
	SysTick_Init(72); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	
	my_mem_init(SRAMIN);	//��ʼ���ڲ��ڴ��
	if(SD_Init()!=0)
	{	
		Max_Lun=0;	//SD������,���ֻ��һ������
		//LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"SD Card Error!");	//���SD������
	}
	else //SD ������
	{   															  
		//LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"SD Card Size:     MB"); 
 		Mass_Memory_Size[1]=SDCardInfo.CardCapacity;  // �õ�SD���������ֽڣ�����SD����������4G��ʱ��,��Ҫ�õ�����u32����ʾ
	    Mass_Block_Size[1] =512;  // ��Ϊ������Init����������SD���Ĳ����ֽ�Ϊ512��,��������һ����512���ֽ�.
	    Mass_Block_Count[1]=Mass_Memory_Size[1]/Mass_Block_Size[1];
		//LCD_ShowNum(10+13*8,50,Mass_Memory_Size[1]>>20,5,16);	//��ʾSD������ 
 	}
	
	delay_ms(1800);
	USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(700);
	USB_Port_Set(1);	//USB�ٴ����� 
	
	Data_Buffer=mymalloc(SRAMIN,BULK_MAX_PACKET_SIZE*2*4);	//ΪUSB���ݻ����������ڴ�
	Bulk_Data_Buff=mymalloc(SRAMIN,BULK_MAX_PACKET_SIZE);	//�����ڴ�
 	
	/* USB���� */
 	USB_Interrupts_Config();    
 	Set_USBClock();   
 	USB_Init();	    
	delay_ms(1800);	   	    
	
	while(1)
	{
		delay_ms(1);				  
		if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
		{	 						   
			//LCD_Fill(10,190,240,190+16,WHITE);//�����ʾ			  	   
			if(USB_STATUS_REG&0x01)//����д		  
			{
				//LCD_ShowString(10,190,200,16,16,"USB Writing...");//��ʾUSB����д������	 
			}
			if(USB_STATUS_REG&0x02)//���ڶ�
			{
//				LCD_ShowString(10,190,200,16,16,"USB Reading...");//��ʾUSB���ڶ�������  		 
			}	 										  
//			if(USB_STATUS_REG&0x04)LCD_ShowString(10,210,200,16,16,"USB Write Err ");//��ʾд�����
//			//else LCD_Fill(10,210,240,210+16,WHITE);//�����ʾ	  
//			if(USB_STATUS_REG&0x08)LCD_ShowString(30,230,200,16,16,"USB Read  Err ");//��ʾ��������
//			else LCD_Fill(10,230,240,230+16,WHITE);//�����ʾ    
			USB_STA=USB_STATUS_REG;//��¼����״̬
		}
		if(Divece_STA!=bDeviceState) 
		{
//			if(bDeviceState==CONFIGURED)LCD_ShowString(10,170,200,16,16,"USB Connected    ");//��ʾUSB�����Ѿ�����
//			else LCD_ShowString(10,170,200,16,16,"USB DisConnected ");//��ʾUSB���γ���
			Divece_STA=bDeviceState;
		}
		tct++;
		if(tct==200)
		{
			tct=0;
			LED1=!LED1;//��ʾϵͳ������
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB������,�����offline������
				bDeviceState=CONFIGURED;
			}else//û�еõ���ѯ 
			{
				offline_cnt++;  
				if(offline_cnt>10)bDeviceState=UNCONNECTED;//2s��û�յ����߱��,����USB���γ���
			}
			USB_STATUS_REG=0;
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
