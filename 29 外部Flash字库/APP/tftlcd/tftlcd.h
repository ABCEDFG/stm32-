#ifndef _TFTLCD_H
#define _TFTLCD_H

#include <system.h>

#define LCD_LED PBout(0)   //LCD背光 PB0
#define TFTLCD_DIR	0	//0：竖屏  1：横屏  默认竖屏

//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置16位总线时STM32内部会右移一位对齐!			    
#define TFTLCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)



//画笔颜色
#define WHITE         	 0xFFFF //白色
#define BLACK         	 0x0000	//黑色 
#define BLUE         	 0x001F //蓝色
#define BRED             0XF81F //红色（，，）
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF //蓝色
#define RED           	 0xF800 //红色
#define MAGENTA       	 0xF81F //洋红色
#define GREEN         	 0x07E0 //绿色
#define CYAN          	 0x7FFF //青色
#define YELLOW        	 0xFFE0 //黄色
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



//TFTLCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;            //LCD 方向
}_tftlcd_data;

//TFTLCD地址结构体
typedef struct
{
	u16 LCD_CMD;
	u16 LCD_DATA;
}TFTLCD_TypeDef;

//LCD参数
extern _tftlcd_data tftlcd_data;	//管理LCD重要参数

//LCD的前端颜色和背景色	   
extern u16  FRONT_COLOR;//前端颜色 默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色


void LCD_WriteCmd(u16 cmd);
void LCD_WriteData(u16 data);
void LCD_WriteCmdData(u16 cmd,u16 data);
void LCD_WriteData_Color(u16 color);

void TFTLCD_Init(void); //初始化
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);//设置窗口
void LCD_Display_Dir(u8 dir);//设置屏幕显示方向
void LCD_Clear(u16 Color);//清屏
void LCD_Fill(u16 xState,u16 yState,u16 xEnd,u16 yEnd,u16 color);//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);//在指定区域内填充指定颜色块
void LCD_DrawPoint(u16 x,u16 y);//画点
void LCD_DrawFRONT_COLOR(u16 x,u16 y,u16 color);//指定颜色画点
u16 LCD_ReadPoint(u16 x,u16 y);//读点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);//画线
void LCD_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//指定颜色画线
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color);//画十字标记
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);//画矩形
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);//画圆
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);//显示数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);//显示字符串
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn);//显示汉字					   						   																			
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic);//显示图片

#endif
