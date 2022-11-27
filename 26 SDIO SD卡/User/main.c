#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "sd_sdio.h"


int main()
{
	u8 i=0; 
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	while(SD_Init())
	{
		printf("SD 错误！\r\n");
		delay_ms(500);
	}
	printf("SD Card OK!\r\n");	// 检测SD卡成功 	
	
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:  printf("Card Type:SDSC V1.1\r\n");  break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:  printf("Card Type:SDSC V2.0\r\n");  break;
		case SDIO_HIGH_CAPACITY_SD_CARD:  printf("Card Type:SDHC V2.0\r\n");  break;
		case SDIO_MULTIMEDIA_CARD:        printf("Card Type:MMC Card\r\n");   break;	
	}	
	if(SDCardInfo.CardCapacity>>20 > 1024)
	{
		printf("SD 容量: %.2f GB\r\n",(SDCardInfo.CardCapacity>>20) / 1024.0);  // 显示 SD 卡的容量
	}
	else
	{
		printf("SD 容量: %lld MB\r\n",SDCardInfo.CardCapacity>>20);  // 显示 SD 卡的容量
	}
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
