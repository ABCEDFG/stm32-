#include "OLED.h"
#include "oledfont.h"  
#include "systick.h"
#include <math.h>


/*
*	该库使用 128 * 64 / 8 的 RAM 做为缓存，以提供画图功能
*	所有显示函数均为对缓存区的的操作
*	如需显示到 OLED 上，必须使用函数将 OLED_Fill() 
*	将缓存区的数据传输到 OLED 上 
*/


u8 OLED_DATAS[8][128]={0};  // 用于缓存显示屏的画面


void OLED_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(OLED_RCC, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN|OLED_RST_PIN|OLED_DC_PIN|OLED_CLK_PIN|OLED_SDA_PIN;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //速度50MHz
 	GPIO_Init(OLED_PORT, &GPIO_InitStructure);	  
 	GPIO_SetBits(OLED_PORT, OLED_CS_PIN|OLED_RST_PIN|OLED_DC_PIN|OLED_CLK_PIN|OLED_SDA_PIN);
	
	
	delay_ms(100);
	OLED_RST=1;
	delay_ms(100);
	OLED_RST=0;
	delay_ms(100);
	OLED_RST=1;
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--关闭oled面板
	OLED_WR_Byte(0x00,OLED_CMD);//---设置低列地址
	OLED_WR_Byte(0x10,OLED_CMD);//---设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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
	OLED_Clear();  // 清屏
	OLED_Set_Pos(0,0); 	
}


//向SSD1106写入一个字节
//dat: 要写入的数据/命令
//cmd: 数据/命令标志 0,表示命令;  1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)  
	  OLED_DC=1;  // 写命令
	else 
	  OLED_DC=0;  // 写数据	  
	OLED_CS=0;  // 片选
	
	for(i=0;i<8;i++)  // 高位到低位
	{			  
		OLED_CLK=0;
		if(dat&0x80)
		   OLED_SDA=1;
		else 
		   OLED_SDA=0;
		OLED_CLK=1;
		dat<<=1;   
	}				 		  
	OLED_CS=1;  // 关片选
	OLED_DC=1; 	  
}


/* 刷新屏幕画面 */	  
void OLED_Fill()  
{  
	u8 i,j;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
		
		for(j=0;j<128;j++)
		{
			OLED_WR_Byte(OLED_DATAS[i][j],OLED_DATA); 
		}
	} 
}


/* 清屏函数 */	  
void OLED_Clear()  // 效率好像不行，待优化
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


/* 设置位置 */
void OLED_Set_Pos(u8 x, u8 y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}


// 开启OLED显示 
// 0: 关闭显示    1: 开启显示
void OLED_Display(char x)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
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


/* 在指定的位置画点 */
void OLED_Spot(u8 x,u8 y)
{      	
	u8 y1;
	
	y1 = y & 0x07;
	y >>= 3;
	OLED_DATAS[y][x] |= 1 << y1;
}


/* 交换两个变量的值 */
void Swap(u8 *a, u8 *b)
{
	u8 temp;
	
	temp=*a;
	*a=*b;
	*b=temp;
}


/* 在指定的两点之间画线 */
void OLED_Line(u8 x1,u8 y1,u8 x2,u8 y2)
{      	
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	
	delta_x = x2 - x1;  // 计算坐标增量 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	if(delta_x > 0)
		incx = 1;  // 设置单步方向 
	else if(delta_x == 0)
		incx = 0;  // 垂直线 
	else 
	{
		incx = -1;
		delta_x = -delta_x;
	} 
	
	if(delta_y > 0)
		incy = 1; 
	else if(delta_y == 0)
		incy=0;//水平线 
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if( delta_x>delta_y)
		distance = delta_x; //选取基本增量坐标轴 
	else 
		distance = delta_y; 
	for(t = 0; t <= distance + 1; t++)//画线输出 
	{  
		OLED_Spot(uRow, uCol);//画点 
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
*	在指定的三点画三角形
*	(x1,y1)、(x1,y1)、(x1,y1):  三个点的坐标
*	flag:   0: 不填充    1: 填充
*	三角形填充还有 bug, 填充不全
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
		
		delta_x = x2 - x1;  // 计算坐标增量 
		delta_y = y2 - y1; 
		uRow = x1; 
		uCol = y1; 
		
		if(delta_x > 0)
			incx = 1;  // 设置单步方向 
		else if(delta_x == 0)
			incx = 0;  // 垂直线 
		else 
		{
			incx = -1;
			delta_x = -delta_x;
		} 
		
		if(delta_y > 0)
			incy = 1; 
		else if(delta_y == 0)
			incy=0;//水平线 
		else
		{
			incy = -1;
			delta_y = -delta_y;
		} 
		
		if( delta_x>delta_y)
			distance = delta_x; //选取基本增量坐标轴 
		else 
			distance = delta_y; 
		for(t = 0; t <= distance + 1; t++)//画线输出 
		{  
			//OLED_Spot(uRow, uCol);//画点 
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
*	画矩形形
*   x, y:  起始坐标
*   w, h:  宽和高
*	flag:  0: 不填充    1: 填充
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
*	画园形
*   x, y:  起始坐标
*   r:     半径
*	flag:  0: 不填充    1: 填充
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
		di = 3 -(r << 1);             //判断下个点位置的标志
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
			//使用Bresenham算法画圆     
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
*	在指定位置显示一个字符
* 	x:0~127    y:0~63		
* 	z: 字体   8: 8*6 	 16: 16*8
* 	chr: 字符
*/
void OLED_ShowChar(u8 x,u8 y,u8 z,u8 chr)
{      	
	u8 i,y1;
	
	y1=y&0x07;
	y>>=3;
	
	chr-=32;  //将 ASCII码 转换成 oledfont.h 文件使用的字符集
	if(z==8)  //判断字体大小
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
			OLED_DATAS[y][i+x]|=F8X16[chr][i]<<y1;  // 上8行像素
			OLED_DATAS[y+1][i+x]|=F8X16[chr][i]>>(8-y1);
			
			OLED_DATAS[y+1][i+x]|=F8X16[chr][i+8]<<y1;  // 下8行像素
			OLED_DATAS[y+2][i+x]|=F8X16[chr][i+8]>>(8-y1);
		}
	}
}


//在指定位置显示一个字符串
// x:0~120    y:0~63		
// z: 字体   8: 8*6 	 16: 16*8
// chr: 字符串
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


//在指定位置显示 数字
// x:0~120    y:0~63		
// z: 字体   8: 8*6 	 16: 16*8
// chr: 数字
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


//在指定位置显示一个汉字
// x:0~120    y:0~63	
// cn: 汉字在数组中的位置
// 大小：只支持 16*16
void OLED_ShowCN(u8 x,u8 y,u8 cn)
{      	
	u8 i,y1;
	
	y1=y&0x07;
	y>>=3;
	
	cn*=2;
	for(i=0;i<16;i++)
	{
		OLED_DATAS[y][i+x]|=Hzk[cn][i]<<y1;  // 上8行像素
		OLED_DATAS[y+1][i+x]|=Hzk[cn][i]>>(8-y1);
		
		OLED_DATAS[y+1][i+x]|=Hzk[cn+1][i]<<y1;  // 下8行像素
		OLED_DATAS[y+2][i+x]|=Hzk[cn+1][i]>>(8-y1);
	}		
}


// 显示图片
// 需新建一个专门存储图片的.h文件
// 貌似有bug，待完善
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

