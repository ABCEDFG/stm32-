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


//���ֱ�
const u8* kbd_tbl[9]={"��","2","3","4","5","6","7","8","9",};
//�ַ���
const u8* kbs_tbl[9]={"DEL","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz",};

u16 kbdxsize;	//������̰������
u16 kbdysize;	//������̰����߶�

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


//����״̬����
//x,y:��������
//key:��ֵ��0~8��
//sta:״̬��0���ɿ���1�����£�
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
	static u8 key_x=0;//0,û���κΰ������£�1~9��1~9�Ű�������
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
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

//��ʾ���.
//index:0,��ʾû��һ��ƥ��Ľ��.���֮ǰ����ʾ
//   ����,������	
void py_show_result(u8 index)
{	
	LCD_ShowNum(30+144,125,index,1,16); 		//��ʾ��ǰ������
	LCD_Fill(30+40,125,30+40+48,130+16,WHITE);	//���֮ǰ����ʾ
 	LCD_Fill(30+40,145,tftlcd_data.width,145+48,WHITE);//���֮ǰ����ʾ    
	if(index)
	{
 		LCD_ShowFontString(30+40,125,200,16,t9.pymb[index-1]->py,16,0); 	//��ʾƴ��
		LCD_ShowFontString(30+40,145,tftlcd_data.width-70,48,t9.pymb[index-1]->pymb,16,0);//��ʾ��Ӧ�ĺ���
		printf("\r\nƴ��:%s\r\n",t9.pymb[index-1]->py);	//�������ƴ��
		printf("���:%s\r\n",t9.pymb[index-1]->pymb);	//����������
	}
}


int main()
{
	u8 i = 0; 
	u8 key = 0;
	u8 inputstr[7];		//�������6���ַ�+������
	u8 inputlen;		//���볤��
	u8 result_num;
	u8 cur_index;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // �ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	TFTLCD_Init();
	Key_Init();
	EN25QXX_Init();				//��ʼ��EN25Q128	
	TP_Init();
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	
	
	while(SD_Init())
	{
		printf("SD ����\r\n");
		delay_ms(500);
	}
	printf("SD Card OK!\r\n");	// ��� SD ���ɹ� 	
	
	FATFS_Init();				// Ϊ fatfs ��ر��������ڴ�				 
  	f_mount(fs[0],"0:",1); 		// ���� SD ��
	f_mount(fs[1],"1:",1); 		// ���� SPI FLASH

	RESTART:
	FRONT_COLOR=RED; 
	while(font_init()) 		        //����ֿ�
	{  
		LCD_ShowString(30,5,tftlcd_data.width,tftlcd_data.height,16,"Font Error!   ");
		delay_ms(500);
	} 
	LCD_ShowFontString(30,45,tftlcd_data.width,tftlcd_data.height,"ƴ�����뷨",16,0);
	LCD_ShowFontString(30,65,tftlcd_data.width,tftlcd_data.height,"KEY0:���",16,0);
	LCD_ShowFontString(30,85,tftlcd_data.width,tftlcd_data.height,"KEY_UP:�Ϸ�  KEY1:�·�",16,0);
	LCD_ShowFontString(30,105,tftlcd_data.width,tftlcd_data.height,"����:        ƥ��:  ",16,0);
	LCD_ShowFontString(30,125,tftlcd_data.width,tftlcd_data.height,"ƴ��:        ��ǰ:  ",16,0);
	LCD_ShowFontString(30,145,tftlcd_data.width,tftlcd_data.height,"���:",16,0);
	
	kbdxsize=86;
	kbdysize=43;
	
	PY_Input_UI(30,195);
	
	memset(inputstr,0,7);	//ȫ������
	inputlen=0;				//���볤��Ϊ0
	result_num=0;			//��ƥ��������
	cur_index=0;
	
	while(1)
	{
		key=PY_Input_Keynum(30,195);
		if(key)
		{
			if(key==1)//ɾ��
			{
				if(inputlen)inputlen--;
				inputstr[inputlen]='\0';//��ӽ�����
			}else 
			{
				inputstr[inputlen]=key+'0';//�����ַ�
				if(inputlen<7)inputlen++;
			}
			if(inputstr[0]!=NULL)
			{
				key=t9.getpymb(inputstr);	//�õ�ƥ��Ľ����
				if(key)//�в���ƥ��/��ȫƥ��Ľ��
				{
					result_num=key&0X7F;	//��ƥ����
					cur_index=1;			//��ǰΪ��һ������ 
					if(key&0X80)		   	//�ǲ���ƥ��
					{
						inputlen=key&0X7F;	//��Чƥ��λ��
						inputstr[inputlen]='\0';//��ƥ���λ��ȥ��
						if(inputlen>1)result_num=t9.getpymb(inputstr);//���»�ȡ��ȫƥ���ַ���
					}  
				}else 						//û���κ�ƥ��
				{				   	
					inputlen--;
					inputstr[inputlen]='\0';
				}
			}else
			{
				cur_index=0;
				result_num=0;
			}
			LCD_Fill(30+40,105,30+40+48,110+16,WHITE);	//���֮ǰ����ʾ
			LCD_ShowNum(30+144,105,result_num,1,16); 	//��ʾƥ��Ľ����
			LCD_ShowFontString(30+40,105,200,16,inputstr,16,0);	//��ʾ��Ч�����ִ�		 
	 		py_show_result(cur_index);					//��ʾ��cur_index��ƥ����
		}	 
		key=Key_Scan(0);
		if(result_num)	//����ƥ��Ľ��	
		{	
			switch(key)
			{
				case 1://�Ϸ�
					if(cur_index<result_num)cur_index++;
					else cur_index=1;
					py_show_result(cur_index);	//��ʾ��cur_index��ƥ����
					break;
 				case 3://�·�
	   				if(cur_index>1)cur_index--;
					else cur_index=result_num;
					py_show_result(cur_index);	//��ʾ��cur_index��ƥ����
					break;
				case 2://�������
 					LCD_Fill(30+40,145,tftlcd_data.width-1,145+48,WHITE);	//���֮ǰ����ʾ    
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
