#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "picture.h"
#include "lcd.h"
#include "A.h"
#include "touch.h"
#include "key.h"



void kai_display()  //������ʾ
{
	FRONT_COLOR=BLACK;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"Touch Test");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.net");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"K_UP:Adjust");	
}

void display_init()  //��ʼ����ʾ
{
	FRONT_COLOR=RED;
	LCD_ShowString(tftlcd_data.width-8*4,0,tftlcd_data.width,tftlcd_data.height,16,"RST");
	LCD_Fill(120, tftlcd_data.height - 16, 139, tftlcd_data.height, BLUE);
    LCD_Fill(140, tftlcd_data.height - 16, 159, tftlcd_data.height, RED);
    LCD_Fill(160, tftlcd_data.height - 16, 179, tftlcd_data.height, MAGENTA);
    LCD_Fill(180, tftlcd_data.height - 16, 199, tftlcd_data.height, GREEN);
    LCD_Fill(200, tftlcd_data.height - 16, 219, tftlcd_data.height, CYAN);
    LCD_Fill(220, tftlcd_data.height - 16, 239, tftlcd_data.height, YELLOW);
}


int main()
{
	u8 i = 0; 
	u8 key = 0;
	u16 penColor = BLUE;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Key_Init();
	Usart1_Init(115200);
	TFTLCD_Init();
	TP_Init();
	
	kai_display();
	delay_ms(2000);	
	
	LCD_Clear(WHITE);
	display_init();
	
	
	
	while(1)
	{
		key = Key_Scan(0);
		if(key == 1)
		{
			TP_Adjust();  //У��
			display_init();
		}
		
		if(TP_Scan(0))
		{
			//ѡ�񻭱���ɫ
			if(tp_dev.y[0]>tftlcd_data.height-18 && tp_dev.y[0]<tftlcd_data.height)
			{
				if(tp_dev.x[0]>220)
				{
					penColor = YELLOW;
				}
				else if(tp_dev.x[0]>200)
                {
                    penColor = CYAN;
                }
                
                else if(tp_dev.x[0]>180)
                {
                    penColor = GREEN;
                }
                else if(tp_dev.x[0]>160)
                {
                   penColor = MAGENTA;
                }
                else if(tp_dev.x[0]>140)
                {
                    penColor = RED;

                }
                else if(tp_dev.x[0]>120)
                {
                    penColor = BLUE;
                }       
			}
			else  //����
			{
				LCD_Fill(tp_dev.x[0]-1,tp_dev.y[0]-1,tp_dev.x[0]+2,tp_dev.y[0]+2,penColor);
			}
		}
		
		if(tp_dev.x[0]>tftlcd_data.width-8*4 && tp_dev.y[0]<16)
		{
			LCD_Fill(0,0,tftlcd_data.width-1,tftlcd_data.height-16-1,BACK_COLOR);
			LCD_ShowString(tftlcd_data.width-8*4,0,tftlcd_data.width,tftlcd_data.height,16,"RST");
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
