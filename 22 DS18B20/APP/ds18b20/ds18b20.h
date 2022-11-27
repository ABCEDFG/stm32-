#ifndef _ds18b20_H
#define _ds18b20_H

#include <system.h>


#define DS18B20_RCC   RCC_APB2Periph_GPIOG
#define DS18B20_PORT  GPIOG
#define DS18B20_PIN   GPIO_Pin_11

#define DS18B20_OUT PGout(11)  
#define DS18B20_IN	PGin(11)



void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);
void DS18B20_Reset(void);
u8 DS18B20_Check(void);
u8 DS18B20_Read_Bit(void);
u8 DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(u8 dat);
void DS18B20_Start(void);
u8 DS18B20_Init(void);
float DS18B20_Get_Temp(void);

#endif
