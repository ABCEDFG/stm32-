#ifndef __SPI_H
#define __SPI_H


#include "system.h"

// #define SPI_SINGLE_LINE_MODE_FLAG  // 单线模式标志


#define SPI_NSS_PORT 			GPIOB   
#define SPI_NSS_PIN 			GPIO_Pin_12
#define SPI_NSS_PORT_RCC		RCC_APB2Periph_GPIOB
#define SPI_NSS  				PBout(12)

#define SPI_SCK_PORT 			GPIOB   
#define SPI_SCK_PIN 			GPIO_Pin_13
#define SPI_SCK_PORT_RCC		RCC_APB2Periph_GPIOB
#define SPI_SCK  				PBout(13)

#ifdef  SPI_SINGLE_LINE_MODE_FLAG
#define SPI_DATA_PORT 			GPIOB   
#define SPI_DATA_PIN 			GPIO_Pin_15
#define SPI_DATA_PORT_RCC		RCC_APB2Periph_GPIOB
#define SPI_DATA_W  			PBout(15)
#define SPI_DATA_R   			PBin(15)
#else
#define SPI_MOSI_PORT 			GPIOB   
#define SPI_MOSI_PIN 			GPIO_Pin_15
#define SPI_MOSI_PORT_RCC		RCC_APB2Periph_GPIOB
#define SPI_MOSI  				PBout(15)
#define SPI_DATA_W				SPI_MOSI

#define SPI_MISO_PORT 			GPIOB   
#define SPI_MISO_PIN 			GPIO_Pin_14
#define SPI_MISO_PORT_RCC		RCC_APB2Periph_GPIOB
#define SPI_MISO  				PBin(14)
#define SPI_DATA_R              SPI_MISO
#endif


#define SPI_DELAY_DUR  10

void SPIx_Init(void);
void SPI_Write_Byte(u8 data);
u8 SPI_Read_Byte(void);
u8 SPI_ReadWrite_Byte(u8 tdata);

void SPI_Write_Data(u8 *data, u16 len);
void SPI_Read_Data(u8 *data, u16 len);

#endif
