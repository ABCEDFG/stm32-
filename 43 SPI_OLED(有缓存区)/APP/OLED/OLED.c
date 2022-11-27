#include "OLED.h"
#include "oledfont.h"  
#include "systick.h"
#include <math.h>


/*
*	�ÿ�ʹ�� 128 * 64 / 8 �� RAM ��Ϊ���棬���ṩ��ͼ����
*	������ʾ������Ϊ�Ի������ĵĲ���
*	������ʾ�� OLED �ϣ�����ʹ�ú����� OLED_Fill() 
*	�������������ݴ��䵽 OLED �� 
*/


u8 OLED_DATAS[8][128]={0};  // ���ڻ�����ʾ���Ļ���


void OLED_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(OLED_RCC, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN|OLED_RST_PIN|OLED_DC_PIN|OLED_CLK_PIN|OLED_SDA_PIN;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //�ٶ�50MHz
 	GPIO_Init(OLED_PORT, &GPIO_InitStructure);	  
 	GPIO_SetBits(OLED_PORT, OLED_CS_PIN|OLED_RST_PIN|OLED_DC_PIN|OLED_CLK_PIN|OLED_SDA_PIN);
	
	
	delay_ms(100);
	OLED_RST=1;
	delay_ms(100);
	OLED_RST=0;
	delay_ms(100);
	OLED_RST=1;
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--�ر�oled���
	OLED_WR_Byte(0x00,OLED_CMD);//---���õ��е�ַ
	OLED_WR_Byte(0x10,OLED_CMD);//---���ø��е�ַ
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();  // ����
	OLED_Set_Pos(0,0); 	
}


//��SSD1106д��һ���ֽ�
//dat: Ҫд�������/����
//cmd: ����/�����־ 0,��ʾ����;  1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)  
	  OLED_DC=1;  // д����
	else 
	  OLED_DC=0;  // д����	  
	OLED_CS=0;  // Ƭѡ
	
	for(i=0;i<8;i++)  // ��λ����λ
	{			  
		OLED_CLK=0;
		if(dat&0x80)
		   OLED_SDA=1;
		else 
		   OLED_SDA=0;
		OLED_CLK=1;
		dat<<=1;   
	}				 		  
	OLED_CS=1;  // ��Ƭѡ
	OLED_DC=1; 	  
}


/* ˢ����Ļ���� */	  
void OLED_Fill()  
{  
	u8 i,j;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ
		
		for(j=0;j<128;j++)
		{
			OLED_WR_Byte(OLED_DATAS[i][j],OLED_DATA); 
		}
	} 
}


/* �������� */	  
void OLED_Clear()  // Ч�ʺ����У����Ż�
{  
	u8 i,j;	
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<128;j++)
		{
			OLED_DATAS[i][j]=0;
		}
	}
	
	OLED_Fill();
}


/* ����λ�� */
void OLED_Set_Pos(u8 x, u8 y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}


// ����OLED��ʾ 
// 0: �ر���ʾ    1: ������ʾ
void OLED_Display(char x)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	if(x==0)
	{
		OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
		OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
	}
	else
	{
		OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
		OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
	}
}


/* ��ָ����λ�û��� */
void OLED_Spot(u8 x,u8 y)
{      	
	u8 y1;
	
	y1 = y & 0x07;
	y >>= 3;
	OLED_DATAS[y][x] |= 1 << y1;
}


/* ��������������ֵ */
void Swap(u8 *a, u8 *b)
{
	u8 temp;
	
	temp=*a;
	*a=*b;
	*b=temp;
}


/* ��ָ��������֮�仭�� */
void OLED_Line(u8 x1,u8 y1,u8 x2,u8 y2)
{      	
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	
	delta_x = x2 - x1;  // ������������ 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	if(delta_x > 0)
		incx = 1;  // ���õ������� 
	else if(delta_x == 0)
		incx = 0;  // ��ֱ�� 
	else 
	{
		incx = -1;
		delta_x = -delta_x;
	} 
	
	if(delta_y > 0)
		incy = 1; 
	else if(delta_y == 0)
		incy=0;//ˮƽ�� 
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if( delta_x>delta_y)
		distance = delta_x; //ѡȡ�������������� 
	else 
		distance = delta_y; 
	for(t = 0; t <= distance + 1; t++)//������� 
	{  
		OLED_Spot(uRow, uCol);//���� 
		xerr += delta_x; 
		yerr += delta_y; 
		if(xerr > distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if(yerr > distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}
}


/* 
*	��ָ�������㻭������
*	(x1,y1)��(x1,y1)��(x1,y1):  �����������
*	flag:   0: �����    1: ���
*	��������仹�� bug, ��䲻ȫ
*/
void OLED_Triangle(u8 x1, u8 y1, u8 x2, u8 y2, u8 x3, u8 y3, u8 flag)
{
	if(flag == 0)
	{
		OLED_Line(x1, y1, x2, y2);
		OLED_Line(x1, y1, x3, y3);
		OLED_Line(x2, y2, x3, y3);
	}
	else
	{
		u16 t; 
		int xerr=0,yerr=0,delta_x,delta_y,distance; 
		int incx,incy,uRow,uCol; 
		
		delta_x = x2 - x1;  // ������������ 
		delta_y = y2 - y1; 
		uRow = x1; 
		uCol = y1; 
		
		if(delta_x > 0)
			incx = 1;  // ���õ������� 
		else if(delta_x == 0)
			incx = 0;  // ��ֱ�� 
		else 
		{
			incx = -1;
			delta_x = -delta_x;
		} 
		
		if(delta_y > 0)
			incy = 1; 
		else if(delta_y == 0)
			incy=0;//ˮƽ�� 
		else
		{
			incy = -1;
			delta_y = -delta_y;
		} 
		
		if( delta_x>delta_y)
			distance = delta_x; //ѡȡ�������������� 
		else 
			distance = delta_y; 
		for(t = 0; t <= distance + 1; t++)//������� 
		{  
			//OLED_Spot(uRow, uCol);//���� 
			OLED_Line(x3, y3, uRow, uCol);
			xerr += delta_x; 
			yerr += delta_y; 
			if(xerr > distance) 
			{ 
				xerr -= distance; 
				uRow += incx; 
			} 
			if(yerr > distance) 
			{ 
				yerr -= distance; 
				uCol += incy; 
			} 
		}
	}
}

/* 
*	��������
*   x, y:  ��ʼ����
*   w, h:  ��͸�
*	flag:  0: �����    1: ���
*/
void OLED_Rectangle(u8 x, u8 y, u8 w, u8 h, u8 flag)
{	
	u8 i;
	
	if(flag == 0)
	{
		OLED_Line(x, y, x + w, y);
		OLED_Line(x, y, x , y + h);
		OLED_Line(x + w, y, x + w , y + h);
		OLED_Line(x + w, y + h, x , y + h);
	}
	else
	{
		for(i = 0; i < w; i++)
		{
			OLED_Line(x + i, y, x + i, y + h);
		}
	}
}

/* 
*	��԰��
*   x, y:  ��ʼ����
*   r:     �뾶
*	flag:  0: �����    1: ���
*/
void OLED_Circle(u8 x, u8 y, u8 r, u8 flag)
{
	u8 xx, yy;
	int a, b;
	int di;
	
	if(flag == 0)
	{
		a = 0;
		b = r;	  
		di = 3 -(r << 1);             //�ж��¸���λ�õı�־
		while(a <= b)
		{
			OLED_Spot(x + a, y - b);             //5
			OLED_Spot(x + b, y - a);             //0           
			OLED_Spot(x + b, y + a);             //4               
			OLED_Spot(x + a, y + b);             //6 
			OLED_Spot(x - a, y + b);             //1       
			OLED_Spot(x - b, y + a);             
			OLED_Spot(x - a, y - b);             //2             
			OLED_Spot(x - b, y - a);             //7     	         
			a++;
			//ʹ��Bresenham�㷨��Բ     
			if(di < 0)  
				di += 4 * a + 6;	  
			else
			{
				di += 10 + 4 * (a - b);   
				b--;
			} 						    
		}
	}
	else
	{		
		for(xx = 0; xx <= r; xx++)
		{
			for(yy = 0; yy <= r; yy++)
			{
				if(xx * xx + yy * yy < r * r)
				{
					OLED_Spot(x + xx, y + yy);
					OLED_Spot(x + xx, y - yy);
					OLED_Spot(x - xx, y + yy);
					OLED_Spot(x - xx, y - yy);
				}
			}
		}
	}
}


/*
*	��ָ��λ����ʾһ���ַ�
* 	x:0~127    y:0~63		
* 	z: ����   8: 8*6 	 16: 16*8
* 	chr: �ַ�
*/
void OLED_ShowChar(u8 x,u8 y,u8 z,u8 chr)
{      	
	u8 i,y1;
	
	y1=y&0x07;
	y>>=3;
	
	chr-=32;  //�� ASCII�� ת���� oledfont.h �ļ�ʹ�õ��ַ���
	if(z==8)  //�ж������С
	{
		for(i=0;i<6;i++)  // 8*6
		{
			OLED_DATAS[y][i+x]=F6x8[chr][i]<<y1;
			OLED_DATAS[y+1][i+x]=F6x8[chr][i]>>(8-y1);
		}
	}
	else
	{
		for(i=0;i<8;i++)  // 16*8
		{
			OLED_DATAS[y][i+x]|=F8X16[chr][i]<<y1;  // ��8������
			OLED_DATAS[y+1][i+x]|=F8X16[chr][i]>>(8-y1);
			
			OLED_DATAS[y+1][i+x]|=F8X16[chr][i+8]<<y1;  // ��8������
			OLED_DATAS[y+2][i+x]|=F8X16[chr][i+8]>>(8-y1);
		}
	}
}


//��ָ��λ����ʾһ���ַ���
// x:0~120    y:0~63		
// z: ����   8: 8*6 	 16: 16*8
// chr: �ַ���
void OLED_ShowStr(u8 x,u8 y,u8 z,u8 *chr)
{
	u32 i=0;
	u8 w;
	
	if(z==8)
	{
		w=6;
	}
	else
	{
		w=8;
	}
	
	while(chr[i]!='\0')
	{
		OLED_ShowChar(x,y,z,chr[i]);
		x+=w;
		if(x>120)
		{
			x=0;
			y+=z;
		}
		i++;
	}
}


//��ָ��λ����ʾ ����
// x:0~120    y:0~63		
// z: ����   8: 8*6 	 16: 16*8
// chr: ����
void OLED_ShowNum(u8 x,u8 y,u8 z,u32 num)
{
	u8 i=0,w;
	u8 temp;
	u8 numx[10];
	
	
	if(z==8)
	{
		w=6;
	}
	else
	{
		w=8;
	}
	
	do
	{
		temp=num%10;
		num/=10;
		numx[i++]=temp+48;
		
		
	}while(num!=0);
	
	while(i!=0)
	{
		OLED_ShowChar(x,y,z,numx[--i]);
		x+=w;
		if(x>120)
		{
			x=0;
			y+=z;
		}
	}
}


//��ָ��λ����ʾһ������
// x:0~120    y:0~63	
// cn: �����������е�λ��
// ��С��ֻ֧�� 16*16
void OLED_ShowCN(u8 x,u8 y,u8 cn)
{      	
	u8 i,y1;
	
	y1=y&0x07;
	y>>=3;
	
	cn*=2;
	for(i=0;i<16;i++)
	{
		OLED_DATAS[y][i+x]|=Hzk[cn][i]<<y1;  // ��8������
		OLED_DATAS[y+1][i+x]|=Hzk[cn][i]>>(8-y1);
		
		OLED_DATAS[y+1][i+x]|=Hzk[cn+1][i]<<y1;  // ��8������
		OLED_DATAS[y+2][i+x]|=Hzk[cn+1][i]>>(8-y1);
	}		
}


// ��ʾͼƬ
// ���½�һ��ר�Ŵ洢ͼƬ��.h�ļ�
// ò����bug��������
void OLED_ShowBMP(u8 x,u8 y,u8 w,u8 h,u8 *bmp)
{
	u8 i,j;
	u8 k;
	
	h>>=3;
	for(i=0;i<h;i++)
	{
		k=i*w;
		for(j=0;j<w;j++)
		{
			OLED_DATAS[i][j]=bmp[k+j];
		}
	}
}

