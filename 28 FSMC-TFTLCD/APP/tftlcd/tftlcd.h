#ifndef _TFTLCD_H
#define _TFTLCD_H

#include <system.h>

#define LCD_LED PBout(0)   //LCD���� PB0
#define TFTLCD_DIR	0	//0������  1������  Ĭ������

//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A10��Ϊ�������������� 
//ע������16λ����ʱSTM32�ڲ�������һλ����!			    
#define TFTLCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)



//������ɫ
#define WHITE         	 0xFFFF //��ɫ
#define BLACK         	 0x0000	//��ɫ 
#define BLUE         	 0x001F //��ɫ
#define BRED             0XF81F //��ɫ��������
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF //��ɫ
#define RED           	 0xF800 //��ɫ
#define MAGENTA       	 0xF81F //���ɫ
#define GREEN         	 0x07E0 //��ɫ
#define CYAN          	 0x7FFF //��ɫ
#define YELLOW        	 0xFFE0 //��ɫ
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)



//TFTLCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;            //LCD ����
}_tftlcd_data;

//TFTLCD��ַ�ṹ��
typedef struct
{
	u16 LCD_CMD;
	u16 LCD_DATA;
}TFTLCD_TypeDef;

//LCD����
extern _tftlcd_data tftlcd_data;	//����LCD��Ҫ����

//LCD��ǰ����ɫ�ͱ���ɫ	   
extern u16  FRONT_COLOR;//ǰ����ɫ Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


void LCD_WriteCmd(u16 cmd);
void LCD_WriteData(u16 data);
void LCD_WriteCmdData(u16 cmd,u16 data);
void LCD_WriteData_Color(u16 color);

void TFTLCD_Init(void); //��ʼ��
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);//���ô���
void LCD_Display_Dir(u8 dir);//������Ļ��ʾ����
void LCD_Clear(u16 Color);//����
void LCD_Fill(u16 xState,u16 yState,u16 xEnd,u16 yEnd,u16 color);//��䵥ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);//��ָ�����������ָ����ɫ��
void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_DrawFRONT_COLOR(u16 x,u16 y,u16 color);//ָ����ɫ����
u16 LCD_ReadPoint(u16 x,u16 y);//����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);//����
void LCD_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//ָ����ɫ����
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color);//��ʮ�ֱ��
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);//������
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);//��Բ
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);//��ʾ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);//��ʾ�ַ���
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn);//��ʾ����					   						   																			
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic);//��ʾͼƬ

#endif
