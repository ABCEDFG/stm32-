#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "sd_sdio.h"
#include "ff.h"
#include "fatfs_app.h"
#include "malloc.h"



int main()
{
	u8 i = 0; 
	u32 total, free;
	FIL f1;
	UINT w;
	BYTE buffer[] = "�ļ���д����";
	FRESULT res;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // �ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	
	while(SD_Init())
	{
		printf("SD ����\r\n");
		delay_ms(500);
	}
	printf("SD Card OK!\r\n");	// ��� SD ���ɹ� 	
	
	FATFS_Init();				// Ϊ fatfs ��ر��������ڴ�				 
  	f_mount(fs[0],"0:",1); 		// ����SD��
	
	/* SD ������ */
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:  printf("Card Type:SDSC V1.1\r\n");  break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:  printf("Card Type:SDSC V2.0\r\n");  break;
		case SDIO_HIGH_CAPACITY_SD_CARD:  printf("Card Type:SDHC V2.0\r\n");  break;
		case SDIO_MULTIMEDIA_CARD:        printf("Card Type:MMC Card\r\n");   break;	
	}	
	
	/* FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH */
	if(res == 0X0D)
	{
		printf("��ʽ�� FLASH...\r\n");
		res = f_mkfs("1:",1,4096);  // ��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res == 0)
		{
			f_setlabel((const TCHAR *)"1:PRECHIN");	 // ����Flash���̵�����Ϊ��PRECHIN
			printf("��ʽ����ɣ�\r\n");
		}
		else 
		{
			printf("��ʽ��ʧ�ܣ�\r\n");
		}
		delay_ms(1000);
	}
	
	/* �õ�SD������������ʣ������ */
	while(fatfs_getfree("0:",&total,&free))	
	{
		printf("SD�� Fatfs ����!");
		delay_ms(200);
		LED2=!LED2;
	}
	
	printf("FATFS OK!\r\n");
	printf("SD ������:   %d MB\r\n", total);
	printf("SD ��������: %d MB\r\n", free);
	
	/* д�ļ� */
	res = f_open(&f1, "text.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(res == FR_OK)
	{
		printf("д�룺���ļ��ɹ���\r\n");
		
		res =f_write(&f1,buffer,sizeof(buffer),&w);
		
		if(res == FR_OK)
		{
			 printf("�ļ�д��ɹ���д������ %d �ֽ�\r\n",w);
		}
		else
		{
			 printf("�ļ�д��ʧ�ܣ������룺%d\r\n",res);
		}
		f_close(&f1);
	}
	else
	{
		printf("д�룺�ļ���ʧ�ܣ�\r\n");
	}
	
	res = f_open(&f1, "text.txt", FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK)
	{
		printf("��ȡ���ļ��򿪳ɹ���\r\n");
		res = f_read(&f1, buffer, sizeof(buffer), &w); 
		if(res == FR_OK)
		{
			printf("�ļ���ȡ�ɹ�!\r\n");
			printf("�����ֽ����ݣ�%d\r\n",w);
			printf("��ȡ�õ��ļ�����Ϊ: %s \r\n", buffer);
		}
		else
		{
			 printf("�ļ���ȡʧ��, ������Ϊ��%d\r\n", res);
		}
		f_close(&f1);
	}
	else
	{
		printf("��ȡ���ļ���ʧ�ܣ�");
	}
	f_mount(NULL,"0:",1);  // ȡ�������ļ�ϵͳ
	
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
