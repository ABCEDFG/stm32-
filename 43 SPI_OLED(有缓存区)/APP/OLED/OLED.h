#ifndef _OLED_H
#define _OLED_H


#include "systick.h"


#define 	OLED_RCC  		RCC_APB2Periph_GPIOB
#define 	OLED_PORT 		GPIOB
#define 	OLED_CS_PIN   	GPIO_Pin_14
#define 	OLED_RST_PIN  	GPIO_Pin_5
#define 	OLED_DC_PIN     GPIO_Pin_12
#define 	OLED_CLK_PIN 	GPIO_Pin_13
#define 	OLED_SDA_PIN	GPIO_Pin_15


#define 	OLED_CS  	PBout(14)  // 片选信号  低电平有效  
#define 	OLED_RST 	PBout(5)   // 硬复位  电平翻转触发  (RES)
#define 	OLED_DC  	PBout(12)  // 命令/数据标志  0-命令  1-数据
#define 	OLED_CLK 	PBout(13)  // 串行时钟线  (D0)
#define 	OLED_SDA 	PBout(15)  // 串行数据线  (D1)


#define  OLED_CMD   0  // 写命令  
#define  OLED_DATA  1  // 写数据



extern u8 OLED_DATAS[8][128];


void OLED_Init(void);
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Fill(void);
void OLED_Clear(void);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_Display(char x);
void OLED_Spot(u8 x,u8 y);
void OLED_Line(u8 x1,u8 y1,u8 x2,u8 y2);
void OLED_ShowChar(u8 x,u8 y,u8 z,u8 chr);
void OLED_ShowStr(u8 x,u8 y,u8 z,u8 *chr);
void OLED_ShowCN(u8 x,u8 y,u8 cn);
void OLED_ShowNum(u8 x,u8 y,u8 z,u32 num);
void OLED_ShowBMP(u8 x,u8 y,u8 w,u8 h,u8 *bmp);
void OLED_Triangle(u8 x1, u8 y1, u8 x2, u8 y2, u8 x3, u8 y3, u8 flag);
void OLED_Rectangle(u8 x, u8 y, u8 w, u8 h, u8 flag);
void OLED_Circle(u8 x, u8 y, u8 r, u8 flag);

#endif

