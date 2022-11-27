#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "picture.h"
#include "lcd.h"
#include "A.h"


int main()
{
	u8 i=0; 
	u16 color = 0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	TFTLCD_Init();
	
	FRONT_COLOR=BLACK;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,12,"a");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"AAA");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,24,"Hello World!");
	LCD_ShowString(10,120,tftlcd_data.width,tftlcd_data.height,24,"afdfah");
//	LCD_ShowFontHZ(10, 80,"普中科技");

	LCD_ShowNum(70,70,9,1,BLUE);
	
	//LCD_DrawLine(50,50,100,52);
	
	LCD_Fill(10,150,60,180,GRAY);
	color=LCD_ReadPoint(20,160);
	LCD_Fill(100,150,150,180,color);
//	printf("color=%x\r\n",color);
	
//	LCD_ShowPicture(5,5,200,112,(u8 *)gImage_picture);
//	LCD_ShowPicture(5,205,155,212,(u8 *)gImage_lcd);
	LCD_ShowPicture(5,100,263,214,(u8 *)gImage_A);
	
	while(1)
	{
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
