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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	TFTLCD_Init();
	Key_Init();
	EN25QXX_Init();				//初始化EN25Q128	 
	my_mem_init(SRAMIN);		//初始化内部内存池
	
	FRONT_COLOR = BLACK;
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,24,"Hello World!");
	
	while(SD_Init())
	{
		printf("SD 错误！\r\n");
		delay_ms(500);
	}
	printf("SD Card OK!\r\n");	// 检测 SD 卡成功 	
	
	FATFS_Init();				// 为 fatfs 相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 		// 挂载 SD 卡
	f_mount(fs[1],"1:",1); 		// 挂载 SPI FLASH
	
	while(font_init()) 		        //检查字库
	{
Update:    
		LCD_ShowString(10,80,tftlcd_data.width,tftlcd_data.height,16,"Font Updating...");  // 更新字库中
		key = update_font(10,100,16,"0:");//更新字库
		while(key)//更新失败		
		{		
			 LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"Font Update Failed! ");  // 更新字库失败
			 delay_ms(200);
		}
		LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"Font Update Success!    ");  // 更新字库成功
		delay_ms(1500);
	} 
	//LCD_ShowFontString(10,190,tftlcd_data.width,tftlcd_data.height,"...",16,0);
	LCD_ShowFontString(10,190,tftlcd_data.width,tftlcd_data.height,"按下K_UP键进行字库更新...",24,0);
	
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
