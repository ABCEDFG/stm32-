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
	BYTE buffer[] = "文件读写测试";
	FRESULT res;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	
	while(SD_Init())
	{
		printf("SD 错误！\r\n");
		delay_ms(500);
	}
	printf("SD Card OK!\r\n");	// 检测 SD 卡成功 	
	
	FATFS_Init();				// 为 fatfs 相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 		// 挂载SD卡
	
	/* SD 卡类型 */
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:  printf("Card Type:SDSC V1.1\r\n");  break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:  printf("Card Type:SDSC V2.0\r\n");  break;
		case SDIO_HIGH_CAPACITY_SD_CARD:  printf("Card Type:SDHC V2.0\r\n");  break;
		case SDIO_MULTIMEDIA_CARD:        printf("Card Type:MMC Card\r\n");   break;	
	}	
	
	/* FLASH磁盘,FAT文件系统错误,重新格式化FLASH */
	if(res == 0X0D)
	{
		printf("格式化 FLASH...\r\n");
		res = f_mkfs("1:",1,4096);  // 格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res == 0)
		{
			f_setlabel((const TCHAR *)"1:PRECHIN");	 // 设置Flash磁盘的名字为：PRECHIN
			printf("格式化完成！\r\n");
		}
		else 
		{
			printf("格式化失败！\r\n");
		}
		delay_ms(1000);
	}
	
	/* 得到SD卡的总容量和剩余容量 */
	while(fatfs_getfree("0:",&total,&free))	
	{
		printf("SD卡 Fatfs 错误!");
		delay_ms(200);
		LED2=!LED2;
	}
	
	printf("FATFS OK!\r\n");
	printf("SD 总容量:   %d MB\r\n", total);
	printf("SD 可用容量: %d MB\r\n", free);
	
	/* 写文件 */
	res = f_open(&f1, "text.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(res == FR_OK)
	{
		printf("写入：打开文件成功！\r\n");
		
		res =f_write(&f1,buffer,sizeof(buffer),&w);
		
		if(res == FR_OK)
		{
			 printf("文件写入成功，写入数据 %d 字节\r\n",w);
		}
		else
		{
			 printf("文件写入失败，错误码：%d\r\n",res);
		}
		f_close(&f1);
	}
	else
	{
		printf("写入：文件打开失败！\r\n");
	}
	
	res = f_open(&f1, "text.txt", FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK)
	{
		printf("读取：文件打开成功！\r\n");
		res = f_read(&f1, buffer, sizeof(buffer), &w); 
		if(res == FR_OK)
		{
			printf("文件读取成功!\r\n");
			printf("读到字节数据：%d\r\n",w);
			printf("读取得的文件数据为: %s \r\n", buffer);
		}
		else
		{
			 printf("文件读取失败, 错误码为：%d\r\n", res);
		}
		f_close(&f1);
	}
	else
	{
		printf("读取：文件打开失败！");
	}
	f_mount(NULL,"0:",1);  // 取消挂载文件系统
	
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
