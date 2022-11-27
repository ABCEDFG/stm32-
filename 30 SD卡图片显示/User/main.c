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
#include "piclib.h"
#include "string.h"		
#include "math.h"


int main()
{
	u8 i = 0; 
	u8 key = 0;
	u8 res;
	DIR picdir;	 		// ͼƬĿ¼
	FILINFO *picfileinfo;// �ļ���Ϣ 
	u8 *pname;			// ��·�����ļ���
	u16 totpicnum; 		// ͼƬ�ļ�����
	u16 curindex;		// ͼƬ��ǰ����
	u8 pause=0;			// ��ͣ���
	u16 temp;
	u32 *picoffsettbl;	//ͼƬ�ļ�offset������
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // �ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	TFTLCD_Init();
	Key_Init();
	EN25QXX_Init();				//��ʼ��EN25Q128	 
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	
	FRONT_COLOR = BLACK;
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,24,"Hello World!");
	
	while(SD_Init())
	{
		printf("SD ����\r\n");
		delay_ms(500);
	}
	printf("SD Card OK!\r\n");	// ��� SD ���ɹ� 	
	
	FATFS_Init();				// Ϊ fatfs ��ر��������ڴ�				 
  	f_mount(fs[0],"0:",1); 		// ���� SD ��
	f_mount(fs[1],"1:",1); 		// ���� SPI FLASH
	
	while(font_init()) 		        //����ֿ�
	{  
		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"Font Error!   ");
		delay_ms(500);
	} 
	LCD_ShowFontString(10,50,tftlcd_data.width,tftlcd_data.height,"ͼƬ��ʾ",16,0);
	
	while(f_opendir(&picdir,"0:/PICTURE"))  // ��ͼƬ�ļ���
 	{	    
		LCD_ShowFontString(10,70,tftlcd_data.width,tftlcd_data.height,"PICTURE�ļ��д���!",16,0);
		delay_ms(200);
		LCD_Fill(10,70,tftlcd_data.width,86,WHITE);  // �����ʾ
		delay_ms(200);
	}  
	totpicnum=fatfs_get_filetype_tnum("0:/PICTURE",TYPE_PICTURE);  // �õ�����Ч�ļ���
	
	while(totpicnum==NULL)  // ͼƬ�ļ�Ϊ0		
 	{	    
		LCD_ShowFontString(10,70,tftlcd_data.width,tftlcd_data.height,"û��ͼƬ�ļ�!",16,0);
		delay_ms(200);
		LCD_Fill(10,70,tftlcd_data.width,86,WHITE);  // �����ʾ
		delay_ms(200);		
	} 
	picfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	 // �����ڴ�
 	pname=mymalloc(SRAMIN,_MAX_LFN*2+1);	// Ϊ��·�����ļ��������ڴ�
 	picoffsettbl=mymalloc(SRAMIN,4*totpicnum);	// ����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����
 	while(!picfileinfo||!pname||!picoffsettbl)	// �ڴ�������
 	{	    	
		LCD_ShowFontString(10,70,tftlcd_data.width,tftlcd_data.height,"�ڴ����ʧ��!",16,0);
		delay_ms(200);				  
		LCD_Fill(10,70,tftlcd_data.width,86,WHITE);  // �����ʾ
		delay_ms(200);				  
	}  	
	// ��¼����
    res=f_opendir(&picdir,"0:/PICTURE");  // ��Ŀ¼
	
	if(res==FR_OK)
	{
		curindex=0;  // ��ǰ����Ϊ0
		while(1) // ȫ����ѯһ��
		{
			temp=picdir.dptr;								// ��¼��ǰdptrƫ��
	        res=f_readdir(&picdir,picfileinfo);       		// ��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	// ������/��ĩβ��,�˳�	 	 
			res=f_typetell((u8*)picfileinfo->fname);	
			if((res&0XF0)==TYPE_PICTURE)  // ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				picoffsettbl[curindex]=temp;  // ��¼����
				curindex++;
			}	    
		} 
	}   
	LCD_ShowFontString(10,70,tftlcd_data.width,tftlcd_data.height,"��ʼ��ʾ...",16,0); 
	delay_ms(1500);
	piclib_init();										//��ʼ����ͼ	   	   
	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&picdir,(const TCHAR*)"0:/PICTURE"); 	//��Ŀ¼
	
	while(res==FR_OK)//�򿪳ɹ�
	{	
		dir_sdi(&picdir,picoffsettbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�
		strcpy((char*)pname,"0:/PICTURE/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)picfileinfo->fname);//���ļ������ں���
 		LCD_Clear(BLACK);
 		picfile_display(pname,0,0,tftlcd_data.width,tftlcd_data.height,1);//��ʾͼƬ    
		LCD_ShowFontString(2,2,tftlcd_data.width,16,pname,16,1); 				//��ʾͼƬ����
		i=0;
		while(1) 
		{
			key=Key_Scan(0);		//ɨ�谴��
			if(i>250)
				key=1;			//ģ��һ�ΰ���K_DOWN    
			if(i%10==0)
				LED1=!LED1;//D1��˸,��ʾ������������.
			if(key== 1)		//��һ��
			{
				if(curindex)curindex--;
				else curindex=totpicnum-1;
				break;
			}
			else if(key== 2)//��һ��
			{
				curindex++;		   	
				if(curindex>=totpicnum)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
				break;
			}
			else if(key== 3)
			{
				pause=!pause;
				LED1=!pause; 	//��ͣ��ʱ��LED1��.  
			}
			if(pause==0)
				i++;
			delay_ms(10); 
		}					    
		res=0;  
	} 							    
	myfree(SRAMIN,picfileinfo);			//�ͷ��ڴ�						   		    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,picoffsettbl);			//�ͷ��ڴ�
	
//	while(1)
//	{
//		key = Key_Scan(0);
//		
//		
//		i++;
//		if(i%50==0)
//		{
//			LED2=!LED2;
//			i=0;
//		}
//		delay_ms(10);
//	}
}
