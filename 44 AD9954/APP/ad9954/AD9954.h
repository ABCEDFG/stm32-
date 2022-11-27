#ifndef _AD9954_H
#define _AD9954_H

#include "system.h"


/* 相关引脚 */
#define   AD9954_CS_RCC		RCC_APB2Periph_GPIOA
#define   AD9954_CS_PORT 	GPIOA   
#define   AD9954_CS_PIN 	GPIO_Pin_3
#define   AD9954_CS 	    PAout(3)

#define   AD9954_SCLK_RCC	RCC_APB2Periph_GPIOA
#define   AD9954_SCLK_PORT 	GPIOA   
#define   AD9954_SCLK_PIN 	GPIO_Pin_4
#define   AD9954_SCLK       PAout(4)

#define   AD9954_SDIO_RCC	RCC_APB2Periph_GPIOA
#define   AD9954_SDIO_PORT 	GPIOA   
#define   AD9954_SDIO_PIN 	GPIO_Pin_5
#define   AD9954_SDIO       PAout(5)

#define   AD9954_OSK_RCC	RCC_APB2Periph_GPIOC
#define   AD9954_OSK_PORT 	GPIOC   
#define   AD9954_OSK_PIN 	GPIO_Pin_0
#define   AD9954_OSK 	    PCout(0)

#define   AD9954_PS0_RCC	RCC_APB2Periph_GPIOA
#define   AD9954_PS0_PORT 	GPIOA   
#define   AD9954_PS0_PIN 	GPIO_Pin_2
#define   AD9954_PS0 		PAout(2)

#define   AD9954_PS1_RCC	RCC_APB2Periph_GPIOB
#define   AD9954_PS1_PORT 	GPIOB
#define   AD9954_PS1_PIN 	GPIO_Pin_10
#define   AD9954_PS1 		PBout(10)

#define   AD9954_UPD_RCC	RCC_APB2Periph_GPIOA
#define   AD9954_UPD_PORT 	GPIOA   
#define   AD9954_UPD_PIN 	GPIO_Pin_7
#define   AD9954_UPD 	    PAout(7)

#define   AD9954_SDO_RCC	RCC_APB2Periph_GPIOA
#define   AD9954_SDO_PORT 	GPIOA   
#define   AD9954_SDO_PIN 	GPIO_Pin_8
#define   AD9954_SDO	    PAin(8)

#define   AD9954_IOSY_RCC	RCC_APB2Periph_GPIOB
#define   AD9954_IOSY_PORT 	GPIOB   
#define   AD9954_IOSY_PIN 	GPIO_Pin_1
#define   AD9954_IOSY       PBout(1)

#define   AD9954_RES_RCC	RCC_APB2Periph_GPIOA
#define   AD9954_RES_PORT 	GPIOA   
#define   AD9954_RES_PIN 	GPIO_Pin_6
#define   AD9954_RES 	    PAout(6)

#define   AD9954_PWR_RCC	RCC_APB2Periph_GPIOB
#define   AD9954_PWR_PORT 	GPIOB   
#define   AD9954_PWR_PIN 	GPIO_Pin_0
#define   AD9954_PWR 	    PBout(0)


#define  CFR1	 0X00			
#define  CFR2    0X01
#define  ASF     0X02
#define  ARR     0X03
#define  FTW0    0X04
#define  POW0    0X05
#define  FTW1    0X06
#define  NLSCW   0X07
#define  PLSCW   0X08
#define  RSCW0   0X07
#define  RSCW1   0X08
#define  RSCW2   0X09
#define  RSCW3   0X0A
#define  RAM     0X0B

#define No_Dwell 0X80


typedef enum 
{
	DownScan  =   0,
	UpScan,
	DoubleScan
}ScanMode;


void AD9954_Init(void);
void AD9954_GPIO_Init(void);  // AD9954 初始化
void AD9954_Reset(void);  // AD9954 复位
void AD9954_Send_Byte(u8 data);  // 向 AD9954 发送一个字节
u8 AD9954_Read_Byte(void);  // 从 AD9954 读取一个字节
u32 AD9954_Get_Ftw(double f_real);  // 根据真实值得到频率（需要传给 AD9954 的值）
void AD9954_Upadte(void);  // 更新 AD9954 内部寄存器
void AD9954_Set_Fre(double f);  // 设置 AD9954 的输出频率
u32 AD9954_Read_Vau1(void);  // 读寄存器
void AD9954_Generate_PSK(float f, u16 *phase);

void AD9954_PSK(float f, float phase1, float phase2, float phase3, float phase4);
void AD9954_Generate_FM(u32 *fre);  // FM 调制信号输出
void AD9954_ASK_Init(void);
void AD9954_AM_Init(void);
void AD9954_Write_ASF(u16 factor);  // 改变 DAC 输出幅度
void AD9954_Linear_Sweep(double  MinFreq, double MaxFreq, double UpStepFreq, u8 UpStepTime, 
						 double DownStepFreq, u8 DownStepTime, ScanMode Mode);  // AD9954 扫频模式输出
void AD9954_PS0_WriteBit(BitAction flag);  // 改变 AD9954 扫频方向
void AD9954_FSK(double f1, double f2, double f3, double f4);  // 四项 FSK 信号输出
void AD9954_Line_Sweep(float f1, float f2);  // 线性扫面输出模式

	

#endif

