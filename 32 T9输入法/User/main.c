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
#include "touch.h"
#include "pinyin_input.h"
#include "string.h"


//数字表
const u8* kbd_tbl[9]={"←","2","3","4","5","6","7","8","9",};
//字符表
const u8* kbs_tbl[9]={"DEL","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz",};

u16 kbdxsize;	//虚拟键盘按键宽度
u16 kbdysize;	//虚拟键盘按键高度

void PY_Input_UI(u16 x,u16 y)
{
	u16 i;
	FRONT_COLOR=RED;
	LCD_DrawRectangle(x,y,x+kbdxsize*3,y+kbdysize*3);						   
	LCD_DrawRectangle(x+kbdxsize,y,x+kbdxsize*2,y+kbdysize*3);						   
	LCD_DrawRectangle(x,y+kbdysize,x+kbdxsize*3,y+kbdysize*2);
	FRONT_COLOR=BLUE;
	for(i=0;i<9;i++)
	{
		LCD_ShowFontStringMid(x+(i%3)*kbdxsize,y+4+kbdysize*(i/3),(u8*)kbd_tbl[i],16,kbdxsize);		
		LCD_ShowFontStringMid(x+(i%3)*kbdxsize,y+kbdysize/2+kbdysize*(i/3),(u8*)kbs_tbl[i],16,kbdxsize);		
	}  		
}


//按键状态设置
//x,y:键盘坐标
//key:键值（0~8）
//sta:状态，0，松开；1，按下；
void PY_Input_Key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>8)return;
	if(sta)LCD_Fill(x+j*kbdxsize+1,y+i*kbdysize+1,x+j*kbdxsize+kbdxsize-1,y+i*kbdysize+kbdysize-1,GREEN);
	else LCD_Fill(x+j*kbdxsize+1,y+i*kbdysize+1,x+j*kbdxsize+kbdxsize-1,y+i*kbdysize+kbdysize-1,WHITE); 
	LCD_ShowFontStringMid(x+j*kbdxsize,y+4+kbdysize*i,(u8*)kbd_tbl[keyx],16,kbdxsize);		
	LCD_ShowFontStringMid(x+j*kbdxsize,y+kbdysize/2+kbdysize*i,(u8*)kbs_tbl[keyx],16,kbdxsize);	
}


u8 PY_Input_Keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,没有任何按键按下；1~9，1~9号按键按下
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
	{	
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*kbdxsize+kbdxsize)&&tp_dev.x[0]>(x+j*kbdxsize)&&tp_dev.y[0]<(y+i*kbdysize+kbdysize)&&tp_dev.y[0]>(y+i*kbdysize))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					PY_Input_Key_staset(x,y,key_x-1,0);
					key_x=key;
					PY_Input_Key_staset(x,y,key_x-1,1);
				}
				break;
			}
		}  
	}else if(key_x) 
	{
		PY_Input_Key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}

//显示结果.
//index:0,表示没有一个匹配的结果.清空之前的显示
//   其他,索引号	
void py_show_result(u8 index)
{	
	LCD_ShowNum(30+144,125,index,1,16); 		//显示当前的索引
	LCD_Fill(30+40,125,30+40+48,130+16,WHITE);	//清除之前的显示
 	LCD_Fill(30+40,145,tftlcd_data.width,145+48,WHITE);//清除之前的显示    
	if(index)
	{
 		LCD_ShowFontString(30+40,125,200,16,t9.pymb[index-1]->py,16,0); 	//显示拼音
		LCD_ShowFontString(30+40,145,tftlcd_data.width-70,48,t9.pymb[index-1]->pymb,16,0);//显示对应的汉字
		printf("\r\n拼音:%s\r\n",t9.pymb[index-1]->py);	//串口输出拼音
		printf("结果:%s\r\n",t9.pymb[index-1]->pymb);	//串口输出结果
	}
}


int main()
{
	u8 i = 0; 
	u8 key = 0;
	u8 inputstr[7];		//最大输入6个字符+结束符
	u8 inputlen;		//输入长度
	u8 result_num;
	u8 cur_index;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	TFTLCD_Init();
	Key_Init();
	EN25QXX_Init();				//初始化EN25Q128	
	TP_Init();
	my_mem_init(SRAMIN);		//初始化内部内存池
	
	
	while(SD_Init())
	{
		printf("SD 错误！\r\n");
		delay_ms(500);
	}
	printf("SD Card OK!\r\n");	// 检测 SD 卡成功 	
	
	FATFS_Init();				// 为 fatfs 相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 		// 挂载 SD 卡
	f_mount(fs[1],"1:",1); 		// 挂载 SPI FLASH

	RESTART:
	FRONT_COLOR=RED; 
	while(font_init()) 		        //检查字库
	{  
		LCD_ShowString(30,5,tftlcd_data.width,tftlcd_data.height,16,"Font Error!   ");
		delay_ms(500);
	} 
	LCD_ShowFontString(30,45,tftlcd_data.width,tftlcd_data.height,"拼音输入法",16,0);
	LCD_ShowFontString(30,65,tftlcd_data.width,tftlcd_data.height,"KEY0:清除",16,0);
	LCD_ShowFontString(30,85,tftlcd_data.width,tftlcd_data.height,"KEY_UP:上翻  KEY1:下翻",16,0);
	LCD_ShowFontString(30,105,tftlcd_data.width,tftlcd_data.height,"输入:        匹配:  ",16,0);
	LCD_ShowFontString(30,125,tftlcd_data.width,tftlcd_data.height,"拼音:        当前:  ",16,0);
	LCD_ShowFontString(30,145,tftlcd_data.width,tftlcd_data.height,"结果:",16,0);
	
	kbdxsize=86;
	kbdysize=43;
	
	PY_Input_UI(30,195);
	
	memset(inputstr,0,7);	//全部清零
	inputlen=0;				//输入长度为0
	result_num=0;			//总匹配数清零
	cur_index=0;
	
	while(1)
	{
		key=PY_Input_Keynum(30,195);
		if(key)
		{
			if(key==1)//删除
			{
				if(inputlen)inputlen--;
				inputstr[inputlen]='\0';//添加结束符
			}else 
			{
				inputstr[inputlen]=key+'0';//输入字符
				if(inputlen<7)inputlen++;
			}
			if(inputstr[0]!=NULL)
			{
				key=t9.getpymb(inputstr);	//得到匹配的结果数
				if(key)//有部分匹配/完全匹配的结果
				{
					result_num=key&0X7F;	//总匹配结果
					cur_index=1;			//当前为第一个索引 
					if(key&0X80)		   	//是部分匹配
					{
						inputlen=key&0X7F;	//有效匹配位数
						inputstr[inputlen]='\0';//不匹配的位数去掉
						if(inputlen>1)result_num=t9.getpymb(inputstr);//重新获取完全匹配字符数
					}  
				}else 						//没有任何匹配
				{				   	
					inputlen--;
					inputstr[inputlen]='\0';
				}
			}else
			{
				cur_index=0;
				result_num=0;
			}
			LCD_Fill(30+40,105,30+40+48,110+16,WHITE);	//清除之前的显示
			LCD_ShowNum(30+144,105,result_num,1,16); 	//显示匹配的结果数
			LCD_ShowFontString(30+40,105,200,16,inputstr,16,0);	//显示有效的数字串		 
	 		py_show_result(cur_index);					//显示第cur_index的匹配结果
		}	 
		key=Key_Scan(0);
		if(result_num)	//存在匹配的结果	
		{	
			switch(key)
			{
				case 1://上翻
					if(cur_index<result_num)cur_index++;
					else cur_index=1;
					py_show_result(cur_index);	//显示第cur_index的匹配结果
					break;
 				case 3://下翻
	   				if(cur_index>1)cur_index--;
					else cur_index=result_num;
					py_show_result(cur_index);	//显示第cur_index的匹配结果
					break;
				case 2://清除输入
 					LCD_Fill(30+40,145,tftlcd_data.width-1,145+48,WHITE);	//清除之前的显示    
					goto RESTART;			 		 	   
			}   	 
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
