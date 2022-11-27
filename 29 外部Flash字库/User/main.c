#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "sd_sdio.h"
#include "ff.h"
#include "fatfs_app.h"
#include "malloc.h"
#include "tftlcd.h"
#include "font_show.h"
#include "key.h"
#include "flash.h"



int main()
{
	u8 i = 0; 
	u8 key = 0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // �ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	TFTLCD_Init();
	Key_Init();
	EN25QXX_Init();				//��ʼ��EN25Q128	 
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	
	FRONT_COLOR = BLACK;
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,24,"Hello World!");
	
	while(SD_Init())
	{
		printf("SD ����\r\n");
		delay_ms(500);
	}
	printf("SD Card OK!\r\n");	// ��� SD ���ɹ� 	
	
	FATFS_Init();				// Ϊ fatfs ��ر��������ڴ�				 
  	f_mount(fs[0],"0:",1); 		// ���� SD ��
	f_mount(fs[1],"1:",1); 		// ���� SPI FLASH
	
	while(font_init()) 		        //����ֿ�
	{
Update:    
		LCD_ShowString(10,80,tftlcd_data.width,tftlcd_data.height,16,"Font Updating...");  // �����ֿ���
		key = update_font(10,100,16,"0:");//�����ֿ�
		while(key)//����ʧ��		
		{		
			 LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"Font Update Failed! ");  // �����ֿ�ʧ��
			 delay_ms(200);
		}
		LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"Font Update Success!    ");  // �����ֿ�ɹ�
		delay_ms(1500);
	} 
	//LCD_ShowFontString(10,190,tftlcd_data.width,tftlcd_data.height,"...",16,0);
	LCD_ShowFontString(10,190,tftlcd_data.width,tftlcd_data.height,"����K_UP�������ֿ����...",24,0);
	
	while(1)
	{
		key = Key_Scan(0);
		if(key == 1)
		{
			goto Update;
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
