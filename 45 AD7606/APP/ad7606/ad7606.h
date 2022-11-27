#ifndef _AD7606_H
#define _AD7606_H


/* AD7606 模式选择 */
#define  AD7606_FLAG   2  // 0--16 普通 IO 口，  1--16 并行 IO 口，  2--串行模式


#include "system.h"


#define  AD7606_CS_RCC	 RCC_APB2Periph_GPIOC
#define  AD7606_CS_PORT  GPIOC  
#define  AD7606_CS_PIN 	 GPIO_Pin_0
#define  AD7606_CS       PCout(0)

#define  AD7606_RD_RCC	 RCC_APB2Periph_GPIOC
#define  AD7606_RD_PORT  GPIOC   
#define  AD7606_RD_PIN 	 GPIO_Pin_1
#define  AD7606_RD       PCout(1)

#define  AD7606_RESET_RCC	 RCC_APB2Periph_GPIOC
#define  AD7606_RESET_PORT   GPIOC   
#define  AD7606_RESET_PIN 	 GPIO_Pin_2
#define  AD7606_RESET        PCout(2)

#define  AD7606_COA_RCC	  RCC_APB2Periph_GPIOC
#define  AD7606_COA_PORT  GPIOC   
#define  AD7606_COA_PIN   GPIO_Pin_3
#define  AD7606_COA       PCout(3)

#define  AD7606_COB_RCC	  RCC_APB2Periph_GPIOC
#define  AD7606_COB_PORT  GPIOC   
#define  AD7606_COB_PIN   GPIO_Pin_4
#define  AD7606_COB       PCout(4)

#define  AD7606_OSl0_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_OSl0_PORT  GPIOC   
#define  AD7606_OSl0_PIN   GPIO_Pin_5
#define  AD7606_OSl0       PCout(5)

#define  AD7606_OSl1_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_OSl1_PORT  GPIOC   
#define  AD7606_OSl1_PIN   GPIO_Pin_6
#define  AD7606_OSl1       PCout(66)

#define  AD7606_OSl2_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_OSl2_PORT  GPIOC   
#define  AD7606_OSl2_PIN   GPIO_Pin_7
#define  AD7606_OSl2       PCout(7)

#define  AD7606_BUSY_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_BUSY_PORT  GPIOC   
#define  AD7606_BUSY_PIN   GPIO_Pin_8
#define  AD7606_BUSY       PCin(8)

#define  AD7606_FRD_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_FRD_PORT  GPIOC   
#define  AD7606_FRD_PIN   GPIO_Pin_9
#define  AD7606_FRD       PCin(9)

#define  AD7606_STBY_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_STBY_PORT  GPIOC   
#define  AD7606_STBY_PIN   GPIO_Pin_10
#define  AD7606_STBY       PCin(10)


/* 16DB 引脚， 分两种模式，可由宏 AD7606_FLAG 更改 */
#if  AD7606_FLAG == 0


#define  AD7606_DB0_RCC   RCC_APB2Periph_GPIOA
#define  AD7606_DB0_PORT  GPIOA   
#define  AD7606_DB0_PIN   GPIO_Pin_10
#define  AD7606_DB0       PAin(10)

#define  AD7606_DB1_RCC   RCC_APB2Periph_GPIOA
#define  AD7606_DB1_PORT  GPIOA   
#define  AD7606_DB1_PIN   GPIO_Pin_9
#define  AD7606_DB1       PAin(9)

#define  AD7606_DB2_RCC   RCC_APB2Periph_GPIOA
#define  AD7606_DB2_PORT  GPIOA   
#define  AD7606_DB2_PIN   GPIO_Pin_8
#define  AD7606_DB2       PAin(8)

#define  AD7606_DB3_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_DB3_PORT  GPIOC   
#define  AD7606_DB3_PIN   GPIO_Pin_9
#define  AD7606_DB3       PCin(9)

#define  AD7606_DB4_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_DB4_PORT  GPIOC   
#define  AD7606_DB4_PIN   GPIO_Pin_8
#define  AD7606_DB4       PCin(8)

#define  AD7606_DB5_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_DB5_PORT  GPIOC   
#define  AD7606_DB5_PIN   GPIO_Pin_7
#define  AD7606_DB5       PCin(7)

#define  AD7606_DB6_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_DB6_PORT  GPIOC   
#define  AD7606_DB6_PIN   GPIO_Pin_6
#define  AD7606_DB6       PCin(6)

#define  AD7606_DB7_RCC   RCC_APB2Periph_GPIOD
#define  AD7606_DB7_PORT  GPIOD   
#define  AD7606_DB7_PIN   GPIO_Pin_15
#define  AD7606_DB7       PDin(15)

#define  AD7606_DB8_RCC   RCC_APB2Periph_GPIOD
#define  AD7606_DB8_PORT  GPIOD   
#define  AD7606_DB8_PIN   GPIO_Pin_14
#define  AD7606_DB8       PDin(14)

#define  AD7606_DB9_RCC   RCC_APB2Periph_GPIOD
#define  AD7606_DB9_PORT  GPIOD   
#define  AD7606_DB9_PIN   GPIO_Pin_13
#define  AD7606_DB9       PDin(13)

#define  AD7606_DB10_RCC   RCC_APB2Periph_GPIOD
#define  AD7606_DB10_PORT  GPIOD   
#define  AD7606_DB10_PIN   GPIO_Pin_12
#define  AD7606_DB10       PDin(12)

#define  AD7606_DB11_RCC   RCC_APB2Periph_GPIOD
#define  AD7606_DB11_PORT  GPIOD   
#define  AD7606_DB11_PIN   GPIO_Pin_11
#define  AD7606_DB11       PDin(11)

#define  AD7606_DB12_RCC   RCC_APB2Periph_GPIOD
#define  AD7606_DB12_PORT  GPIOD   
#define  AD7606_DB12_PIN   GPIO_Pin_10
#define  AD7606_DB12       PDin(10)

#define  AD7606_DB13_RCC   RCC_APB2Periph_GPIOD
#define  AD7606_DB13_PORT  GPIOD   
#define  AD7606_DB13_PIN   GPIO_Pin_9
#define  AD7606_DB13       PDin(9)

#define  AD7606_DB14_RCC   RCC_APB2Periph_GPIOD
#define  AD7606_DB14_PORT  GPIOD   
#define  AD7606_DB14_PIN   GPIO_Pin_8
#define  AD7606_DB14       PDin(8)

#define  AD7606_DB15_RCC   RCC_APB2Periph_GPIOB
#define  AD7606_DB15_PORT  GPIOB   
#define  AD7606_DB15_PIN   GPIO_Pin_15
#define  AD7606_DB15       PBin(15)
	
	
#elif AD7606_FLAG == 1  // 并行 IO 口


#define  AD7606_DB_RCC    RCC_APB2Periph_GPIOB
#define  AD7606_DB_PORT	  GPIOB


#elif AD7606_FLAG == 2  // 串行模式
	
	
#define  AD7606_DB_RCC   RCC_APB2Periph_GPIOC
#define  AD7606_DB_PORT  GPIOC   
#define  AD7606_DB_PIN   GPIO_Pin_11
#define  AD7606_DB       PCin(11)


#endif


void AD7606_Init(void);
void AD7606_Reset(void);  // 复位
void AD7606_Start(void);  // 开始转换
u16 AD7606_ReadData(void);  // 读取 DB 数据
void AD7606_Read(s16 *data);  // 读取 AD7606 数据



















void AD7606_Rate(u8 rate);  // 过采样倍率




#endif
