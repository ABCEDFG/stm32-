#include "ad9954.h"
#include "systick.h"


/* 模块时钟相关 */
#define  AD9954_FOSC  20  // 晶振频率（外部晶振频率）
#define  AD9954_PLL_MULTIPLIER  19  // PLL 倍频数（4--20）
#define  AD9954_FS  (AD9954_FOSC * AD9954_PLL_MULTIPLIER)  // 系统时钟频率


double f_num = 11.3025455157895;  // 频率相关  具体作用？？？？？？？？？？？？？？？？？？？？？


/*
*	AD9954 的 GPIO 初始化
*	因为 AD9954 的最大传输速度为 25MHz，而 GPIO 速度 < 25MHz, 
*	故本程序都无需加延时（us 级）
*/
void AD9954_GPIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(AD9954_CS_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_SCLK_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_SDIO_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_OSK_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_PS0_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_PS1_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_UPD_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_SDO_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_IOSY_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_RES_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(AD9954_PWR_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_OSK_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_OSK_PORT, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = AD9954_CS_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_CS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_SCLK_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_SCLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_SDIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_SDIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_PS0_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_PS0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_UPD_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_UPD_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_RES_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_RES_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_PS1_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_PS1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_IOSY_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_IOSY_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_PWR_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_PWR_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9954_SDO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	  // 上拉输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // 2MHz		 
	GPIO_Init(AD9954_SDO_PORT, &GPIO_InitStructure);
	
	AD9954_IOSY = 0;
	AD9954_OSK = 0;
	AD9954_PWR = 0;
}


/*
*	复位 AD9954
*	不复位也可以
*/
void AD9954_Reset()
{
	AD9954_CS = 0;
	AD9954_RES = 0;
	AD9954_RES = 1;
	delay_ms(100);
	AD9954_RES = 0;
	AD9954_CS = 0;
	AD9954_SCLK = 0;
	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_UPD = 0;
	AD9954_CS = 1;
}


/*
*	向 AD9954 发送一个字节
*	AD9954的传输速度最大为25M，所以不加延时也可以
*	data: 需发送的字节
*/
void AD9954_Send_Byte(u8 data)
{
	u8 i;
	
	for (i = 0; i < 8; i++)
	{
		AD9954_SCLK = 0;
		if (data & 0x80)
		{
			AD9954_SDIO = 1;
		}
		else
		{
			AD9954_SDIO = 0;
		}
		AD9954_SCLK = 1;
		data <<= 1;
	}
}


/* 从 AD9954 读取一个字节 */
u8 AD9954_Read_Byte()
{
	u8 i;
	u8 data = 0;
	
	for(i = 0; i < 8; i++)
	{
		AD9954_SCLK = 0;
		data |= AD9954_SDO;
		AD9954_SCLK = 1;
		data <<= 1;  // 此行是否需要放到前面  ？？？？？？？？？？？？？？？？？？？？？？？？？？
	}
	
	return data;
}


/* AD9954 初始化 */
void AD9954_Init()
{
	AD9954_GPIO_Init();
	AD9954_Reset();  // 复位 AD9954
	delay_ms(300);
	AD9954_CS = 0;
	
	/* 单频模式 */
	AD9954_Send_Byte(CFR1);  // 地址 0 写操作
	AD9954_Send_Byte(0x02);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	//AD9954_Send_Byte(0x40);  // 比较器断电
	AD9954_Send_Byte(0x00);
	
	AD9954_Send_Byte(CFR2);  // 地址 1 写操作
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	
#if AD9954_FS > 400
	#error "系统频率超过芯片最大值"  
#elseif AD9954_FS >= 250
	AD9954_Send_Byte(AD9954_PLL_MULTIPLIER << 3 | 0x04 | 0x03);  
#else
	AD9954_Send_Byte(AD9954_PLL_MULTIPLIER << 3); 
#endif
	
	//AD9954_Send_Byte(0x24);  // 四倍频，打开 VCO 控制高位，系统时钟倍频后为 400MHz
	AD9954_CS = 1;
}


/*
*	根据真实值得到频率（需要传给 AD9954 的值）
*	f_real：真实频率值
*/
u32 AD9954_Get_Ftw(double f_real)
{
	return (u32)(f_real * f_num);
}


/*
*	产生一个更新信号，更新 AD9954 内部寄存器
*	可以不加任何延时
*/
void AD9954_Upadte()
{
	AD9954_UPD = 0;
	AD9954_UPD = 1;
	//delay_us(200);
	AD9954_UPD = 0;
}


/* 
*	设置 AD9954 的频率输出
*	因为使用的浮点数计算，转换过程中会出现误差，
*	通过调整 f_num 的值	可以精确到 0.1 Hz以内
*	f：真实频率值
*/
void AD9954_Set_Fre(double f)
{
	u32 f_data;
	
	AD9954_CS = 0;
	f_data = AD9954_Get_Ftw(f);  // 得到计算值
	AD9954_Send_Byte(FTW0);  // FTW0 地址
	AD9954_Send_Byte((u8)(f_data >> 24));//频率控制字
	AD9954_Send_Byte((u8)(f_data >> 16));
	AD9954_Send_Byte((u8)(f_data >> 8));
	AD9954_Send_Byte((u8)f_data);
	AD9954_CS = 1;
	AD9954_Upadte();
}


/* 读寄存器 CFR1 */
u32 AD9954_Read_Vau1()
{
	u8 i;
	u32 data = 0;
	
	AD9954_CS = 1;     // 这里信号是不是写错了 ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
	AD9954_Send_Byte(CFR1);
	for(i = 0; i < 4; i++)
	{
		data <<= 8;
		data += AD9954_Read_Byte();
	}
	AD9954_CS = 1;    // 这里信号是不是写错了 ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
	
	return data;
}


/* 读寄存器 CFR2 */
u32 AD9954_Read_Vau2()
{
	u8 i;
	u32 data = 0;
	
	AD9954_CS = 0;  // 这里信号是不是写错了 ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
	AD9954_Send_Byte(CFR2);
	for(i = 0; i < 3; i++)
	{
		data <<= 8;
		data += AD9954_Read_Byte();
	}
	AD9954_CS = 1;  // 这里信号是不是写错了 ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
	
	return data;
}


// 这函数有什么用 ?????????????????????????????????????????????????????????????????????????????????????????????????????????????????
void AD9954_PSK(float f, float phase1, float phase2, float phase3, float phase4)
{
	u16 date;
	
	AD9954_CS = 0;
	AD9954_UPD = 0;
	delay_us(1);
	AD9954_Send_Byte(0x00);  // 地址0 写操作
	AD9954_Send_Byte(0x00);  // 打开 RAM 控制位驱动 FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	//
	AD9954_Send_Byte(0x01);  // 地址1写操作
	AD9954_Send_Byte(0x00);  
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0xA4);  // 4 倍频，打开 VCO 控制高位，系统时钟倍频后为 400M
	AD9954_Set_Fre(f);  // 载波频率
	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_Send_Byte(0x07);
	AD9954_Send_Byte(0x01);  //ramp rate=0x0010
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);  // final address:0x000
	AD9954_Send_Byte(0x00);  // start address:0x000;
	AD9954_Send_Byte(0x00);  // RAM0 工作于模式 0,不停留位没有激活
	AD9954_Upadte();
	date = 45.51*phase1;
	date = date << 2;
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte((u8)(date >> 8));  // 频率控制字
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);

	AD9954_PS0 = 1;//ram1
	AD9954_PS1 = 0;

	AD9954_Send_Byte(0x08);
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x01);  // final address:0x0001
	AD9954_Send_Byte(0x04);  // start address:0x0001
	AD9954_Send_Byte(0x00);  // RAM1 工作于模式 0,不停留位没有激活
	AD9954_Upadte();
	date = 45.51*phase2;
	date = date << 2;
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte((u8)(date >> 8));  // 频率控制字
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);

	AD9954_PS0 = 0;  // ram2
	AD9954_PS1 = 1;

	AD9954_Send_Byte(0x09);
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x02);  // final address:0x0002
	AD9954_Send_Byte(0x08);  // start address:0x0002
	AD9954_Send_Byte(0x00);
	AD9954_Upadte();
	date = 45.51*phase3;
	date = date << 2;
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte((u8)(date >> 8));  // 频率控制字
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_PS0 = 1;//ram3
	AD9954_PS1 = 1;
	AD9954_Send_Byte(0x0a);
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x03);  // final address:0x0003
	AD9954_Send_Byte(0x0c);  // start address:0x0003
	AD9954_Send_Byte(0x00);
	AD9954_Upadte();
	date = 45.51*phase4;
	date = date << 2;
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte((u8)(date >> 8));  // 频率控制字
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);

	AD9954_Send_Byte(0x00);  // 地址0写操作
	AD9954_Send_Byte(0xa0);  // 打开RAM控制位驱动FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	PSK 调制信号输出
*	通过 RAM 端来控制信号的相位，从而实现 PSK 信号的调制
*   这里使用一个 RAM 端的循环模式，PSK 信号为 8 位数据位。
*   RAM 的更新速度由 ram rate 定时寄存器控制，寄存器为 16 位，实现对系统提供的 100M 时钟 1~65535 分频，
*	写0是无效的，当跟输出 PSK 载波的频率是 ram rate 跟新频率的整数倍时，才能保证每次的相位改变一致
*	f：      PSK 载波的频率
*	*phase： 写入相位累加器的相位值，180--相位反转，0--不改变相位，
*			 PSK 信号为 8 位，需为 8 位的数值
*/
void AD9954_Generate_PSK(float f, u16 *phase)
{
	u8 i;
	u16 date;
	AD9954_CS = 0;
	AD9954_UPD = 0;

	AD9954_Send_Byte(0x00);  // 地址 0 写操作
	AD9954_Send_Byte(0x00);  // 打开 RAM 控制位驱动 FTW
	AD9954_Send_Byte(0x00); 
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);

	AD9954_Send_Byte(0x01);  // 地址 1 写操作
	AD9954_Send_Byte(0x00);  //
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0xA4);  // 4 倍频，打开 VCO 控制高位，系统时钟倍频后为 400M
	AD9954_Set_Fre(f);  // 载波频率

	AD9954_Send_Byte(0x04);  // FTW0 地址
	AD9954_Send_Byte(0x00);  // 频率控制字
	AD9954_Send_Byte(0x10);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Upadte();

	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_Send_Byte(0x07);
	AD9954_Send_Byte(0x00);  // ramp rate=0x0400
	AD9954_Send_Byte(0x04);
	AD9954_Send_Byte(0x07);  // final address:0x007
	AD9954_Send_Byte(0x00);  // start address:0x000;
	AD9954_Send_Byte(0x80);  // RAM0 工作于模式 4,不停留位没有激活
	AD9954_Upadte();

	AD9954_Send_Byte(0x0b);
	for (i = 0;i<8;i++)
	{
		date = 45.51*phase[i];
		date = date << 2;
		AD9954_Send_Byte((u8)(date >> 8));  // 频率控制字
		AD9954_Send_Byte((u8)date);
		AD9954_Send_Byte(0x00);
		AD9954_Send_Byte(0x00);
	}
	AD9954_Send_Byte(0x00);  // 地址 0 写操作
	AD9954_Send_Byte(0xc0);  // 打开 RAM 控制位驱动 FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	FM 调制信号输出
*	通过 RAM 端来控制信号的频率，从而实现 FM 信号的调制
*	这里使用一个 RAM 段的循环模式，PSK 信号为 8 位数据位
*	RAM 的更新速度由 ram rate 定时寄存器控制，寄存器位 16 位，实现对系统提供的 100M 时钟 1~65535 分频，
*	ram rate 的更新速度为调制信号的频率，这里如 1KHz, 写入 ram rate 的值为 0x0C35,
*	*fre：FM 信号频率值，这里采用的是 32 点频率采样，调用此函数之前需先设置好频率表的值，其按正弦规律改变
*	for(i = 0; i < 32; i++)
*	{
*		fre[i] = Fc + Fshift * sin(wt);
*	}
*/
void AD9954_Generate_FM(u32 *fre)
{
	u8 i;
	u32 date;
	
	AD9954_CS = 0;
	AD9954_UPD = 0;

	AD9954_Send_Byte(0x00);  // 地址 0 写操作
	AD9954_Send_Byte(0x00);  // 打开 RAM 控制位驱动FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);

	AD9954_Send_Byte(0x01);  // 地址 1 写操作
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x24);  // 4 倍频，打开 VCO 控制高位，系统时钟倍频后为 400M

	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_Send_Byte(0x07);
	AD9954_Send_Byte(0x35);  // ramp rate=32kHz
	AD9954_Send_Byte(0x0C);
	AD9954_Send_Byte(0x1F);  // final address:0x000
	AD9954_Send_Byte(0x00);  // start address:0x000;
	AD9954_Send_Byte(0x80);  // RAM0 工作于模式 0,不停留位没有激活
	AD9954_Upadte();

	AD9954_Send_Byte(0x0b);
	for (i = 0; i < 32; i++)
	{
		date = 10.73741 * fre[i];
		AD9954_Send_Byte((u8)(date >> 24));  // 频率控制字
		AD9954_Send_Byte((u8)(date >> 16));
		AD9954_Send_Byte((u8)(date >> 8));
		AD9954_Send_Byte((u8)date);
	}
	AD9954_Send_Byte(0x00);  // 地址 0 写操作
	AD9954_Send_Byte(0x80);  //打开 RAM 控制位驱动 FTW
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	ASK 调制信号输出初始化
*	ASK 调制，需要先设置好的载波频率，然后改变 DAC 比例因子 scale factor 其为 14 位，
*	通过设置为最大和 0 两种值实现 ASK 信号的调制，调用 Write_ASF(u16 factor) 改变
*/
void AD9954_ASK_Init()
{
	AD9954_CS = 0;
	AD9954_Send_Byte(0x00);  // 地址0写操作
	AD9954_Send_Byte(0x02);  // 手动打开OSK打开
	AD9954_Send_Byte(0x00);

	AD9954_Send_Byte(0x20);  // 当受到UPDATE信号，自动清楚相位累加器		//
//	AD9954_Send_Byte(0x00);  // 不改变任何
	AD9954_Send_Byte(0x40);

	// 控制相位偏移，因默认输出是余弦波，所以需控制相位累加器的累加相位为270
	AD9954_Send_Byte(0X05);
	AD9954_Send_Byte(0X30);
	AD9954_Send_Byte(0X00);

	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	AM 调制信号输出初始化
*	AM 调制，需要先设置好载波频率，然后改变 DAC 比例因子 scale factor 其为 14 位，
*	AM 的幅度值按正弦规律变换，可以实现 AM，最高可采用 32 点采样
*	调用 AD9954_Write_ASF(u16 factor) 来改变幅值 
*/
void AD9954_AM_Init()
{
	AD9954_CS = 0;
	AD9954_Send_Byte(0x00);  // 地址 0 写操作
	AD9954_Send_Byte(0x02);  // 手动打开 OSK 打开
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);  // 不改变任何
	AD9954_Send_Byte(0x40);
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	改变 scale factor 数值，改变 DAC 输出幅度
*	factor：要改成的数值
*	范围： 0 ~ 0x3fff
*/
void AD9954_Write_ASF(u16 factor)
{
	AD9954_CS = 0;
	AD9954_Send_Byte(0x02);  // 幅度	
	AD9954_Send_Byte(factor >> 8);
	AD9954_Send_Byte(factor);
	AD9954_CS = 1;
	
	AD9954_Upadte();
}


/*
*	AD9954 扫频模式输出
*	MinFreq：扫频下限频率，
*	MaxFreq：扫频上限频率,
*	UpStepFreq：向上扫频步进，
*	UpStepTime：向上扫频的跳频时间,
*	DownStepFreq：向下扫频步进，
*	DownStepTime：向下扫频的跳频时间,
*	Mode：扫频模式  
*		0--向下扫频模式，
*		1--向上扫频模式，
*		2--双边扫频
*	与频率相关的参数的单位都是 MHz
*	在双边扫描是，AD9954_PS0 = 1，即为向向上扫频，
*	但配置了向下扫频的寄存器，可直接通过 AD9954_PS0_WriteBit() 改变扫频方向
*/
void AD9954_Linear_Sweep(double  MinFreq, double MaxFreq, double UpStepFreq, u8 UpStepTime, 
						 double DownStepFreq, u8 DownStepTime, ScanMode Mode)
{
	u32 FTW_Vau;
    
	AD9954_PS0 = 0;
    AD9954_PS1 = 0;
    AD9954_CS = 0;
    AD9954_Send_Byte(CFR1);
    AD9954_Send_Byte(0x02);
    AD9954_Send_Byte(0x20);  // Linear_Sweep Enable
    AD9954_Send_Byte(0);
	
    if (Mode & No_Dwell)
        AD9954_Send_Byte(0x44);  // Comp Power-Down & Linear Sweep No Dwell
    else
        AD9954_Send_Byte(0x40);  // Comp Power-Down
    
    /* 写入FTW0----最小频率 */
    FTW_Vau =  AD9954_Get_Ftw(MinFreq);
    AD9954_Send_Byte(FTW0);
    AD9954_Send_Byte(FTW_Vau >> 24);
    AD9954_Send_Byte(FTW_Vau >> 16);
    AD9954_Send_Byte(FTW_Vau >> 8);
    AD9954_Send_Byte(FTW_Vau);
    
    /* 写入FTW1----最大频率 */
    FTW_Vau = AD9954_Get_Ftw(MaxFreq);
    AD9954_Send_Byte(FTW1);
    AD9954_Send_Byte(FTW_Vau>>24);
    AD9954_Send_Byte(FTW_Vau>>16);
    AD9954_Send_Byte(FTW_Vau>>8);
    AD9954_Send_Byte(FTW_Vau);
    
    Mode &= 0x7F;
    
    if(Mode != UpScan)
    {
        /* 写入NLSCW----下降频率步进和跳频时间 */
        FTW_Vau = AD9954_Get_Ftw(DownStepFreq);
        AD9954_Send_Byte(NLSCW);
        AD9954_Send_Byte(DownStepTime); // 跳频时间（DownStepTime个？？周期）     
        AD9954_Send_Byte(FTW_Vau>>24);  // 频率步进
        AD9954_Send_Byte(FTW_Vau>>16);
        AD9954_Send_Byte(FTW_Vau>>8);
        AD9954_Send_Byte(FTW_Vau);
        AD9954_PS0 = 0;  // 下降扫频
    }
    if (Mode != DownScan)
    {
        /* 写入PLSCW----上升频率步进和跳频时间 */
        FTW_Vau = AD9954_Get_Ftw(UpStepFreq);
        AD9954_Send_Byte(PLSCW);
        AD9954_Send_Byte(UpStepTime);  // 跳频时间（0XFF个？？周期）
        AD9954_Send_Byte(FTW_Vau>>24); // 频率步进
        AD9954_Send_Byte(FTW_Vau>>16);
        AD9954_Send_Byte(FTW_Vau>>8);
        AD9954_Send_Byte(FTW_Vau);
        AD9954_PS0 = 1;  // 上升扫频
    }
    AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	通过改变 PS0 状态，改变 AD9954 扫频方向
*	flag：PS0 状态
*/
void AD9954_PS0_WriteBit(BitAction flag)
{
	AD9954_PS0 = flag;
}


/*
*	四相 FSK 信号输出
*	在四个 RAM 区设置一个频率值，通过改变 PS0 和 PS1 的电平选择对应的 RAM 端输出相应的频率值来实现 FSK，
*	也可实现二项的 FSK，通过设置定时器中断控制 PS0, PS1 管脚的电平就可以将二进制的编码转化为 FSK 信号输出
*/
void AD9954_FSK(double f1, double f2, double f3, double f4)
{
	u32 FTW_Vau;
	AD9954_CS = 0;

	AD9954_Send_Byte(0x00);  // 地址 0 写操作   
	AD9954_Send_Byte(0x00);  // 打开 RAM 控制位驱动 FTW   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x40);   

	AD9954_Send_Byte(0x01);  // 地址 1 写操作   
	AD9954_Send_Byte(0x00);  //   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0xA4);  // 8 倍频，打开 VCO 控制高位，系统时钟倍频后为 400M   
	AD9954_PS0 = 0;   
	AD9954_PS1 = 0;   
	AD9954_Send_Byte(0x07);   
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x00);  // final address:0x000   
	AD9954_Send_Byte(0x00);  // start address:0x000;   
	AD9954_Send_Byte(0x00);  // RAM0工作于模式0,不停留位没有激活   
	AD9954_Upadte();
	
	FTW_Vau = AD9954_Get_Ftw(f1);
	AD9954_Send_Byte(0x0b);
	AD9954_Send_Byte(FTW_Vau>>24);  // 频率步进
	AD9954_Send_Byte(FTW_Vau>>16);
	AD9954_Send_Byte(FTW_Vau>>8);
	AD9954_Send_Byte(FTW_Vau);

	AD9954_PS0 = 1;  // ram1   
	AD9954_PS1 = 0;   

	AD9954_Send_Byte(0x08);   
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x01);  // final address:0x0001   
	AD9954_Send_Byte(0x04);  // start address:0x0001   
	AD9954_Send_Byte(0x00);  // RAM1 工作于模式 0,不停留位没有激活   
	AD9954_Upadte();
	
	FTW_Vau = AD9954_Get_Ftw(f2);   
	AD9954_Send_Byte(0x0b);   
	AD9954_Send_Byte(FTW_Vau>>24);  // 频率步进
	AD9954_Send_Byte(FTW_Vau>>16);
	AD9954_Send_Byte(FTW_Vau>>8);
	AD9954_Send_Byte(FTW_Vau);  

	AD9954_PS0 = 0;  // ram2   
	AD9954_PS1 = 1;   

	AD9954_Send_Byte(0x09);   
	AD9954_Send_Byte(0x01);  // ramp rate=0x0010   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x02);  // final address:0x0002   
	AD9954_Send_Byte(0x08);  // start address:0x0002   
	AD9954_Send_Byte(0x00);   
	AD9954_Upadte();
	
	FTW_Vau = AD9954_Get_Ftw(f3);   
	AD9954_Send_Byte(0x0b);   
	AD9954_Send_Byte(FTW_Vau>>24);  // 频率步进
	AD9954_Send_Byte(FTW_Vau>>16);
	AD9954_Send_Byte(FTW_Vau>>8);
	AD9954_Send_Byte(FTW_Vau); 
	AD9954_PS0 = 1;  // ram3   
	AD9954_PS1 = 1;   
	AD9954_Send_Byte(0x0a);   
	AD9954_Send_Byte(0x01); // ramp rate=0x0010   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x03); // final address:0x0003   
	AD9954_Send_Byte(0x0c); // start address:0x0003   
	AD9954_Send_Byte(0x00);     
	AD9954_Upadte();
	
	FTW_Vau = AD9954_Get_Ftw(f4);   
	AD9954_Send_Byte(0x0b);   
	AD9954_Send_Byte(FTW_Vau>>24);  // 频率步进
	AD9954_Send_Byte(FTW_Vau>>16);
	AD9954_Send_Byte(FTW_Vau>>8);
	AD9954_Send_Byte(FTW_Vau); 

	AD9954_Send_Byte(0x00); // 地址 0 写操作   
	AD9954_Send_Byte(0x80); // 打开 RAM 控制位驱动 FTW   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x00);   
	AD9954_Send_Byte(0x40);   
	AD9954_CS = 1;
	AD9954_Upadte();
}


/*
*	线性扫面输出模式
*	从 f1 扫到 f2
*	f1: 起始频率
*	f2: 终止频率
*/
void AD9954_Line_Sweep(float f1, float f2)
{
	u32 date;
	
	AD9954_SCLK = 0;
	AD9954_RES = 0;
	AD9954_UPD = 0;
	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_CS = 0;

	/* 先性扫描模式 */
	AD9954_Send_Byte(0x00);  // 地址 0 写操作
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x20);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x40);
	

	AD9954_Send_Byte(0x01);  // 地址 1 写操作
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
//	AD9954_Send_Byte(0x24);  // 4 倍频，打开 VCO 控制高位，系统时钟倍频后为 400M
	AD9954_Send_Byte(AD9954_PLL_MULTIPLIER << 3 | 0x04);
	
	date = 10.7374 * f1;
	AD9954_Send_Byte(0x04);  // FTW0 地址
	AD9954_Send_Byte((u8)(date>>24));  // 频率控制字
	AD9954_Send_Byte((u8)(date>>16));
	AD9954_Send_Byte((u8)(date>>8));
	AD9954_Send_Byte((u8)date);
	
	date = 10.7374 * f2;
	AD9954_Send_Byte(0x06);  // FTW1 地址
	AD9954_Send_Byte((u8)(date >> 24));  // 频率控制字
	AD9954_Send_Byte((u8)(date >> 16));
	AD9954_Send_Byte((u8)(date >> 8));
	AD9954_Send_Byte((u8)date);
	AD9954_Send_Byte(0x07);  // NLSCW
	AD9954_Send_Byte(0x01);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x1b);
	AD9954_Send_Byte(0xf4);
	AD9954_Send_Byte(0x08);  // RLSCW
	AD9954_Send_Byte(0x01);
	AD9954_Send_Byte(0x00);
	AD9954_Send_Byte(0x01);
	AD9954_Send_Byte(0xa3);
	AD9954_Send_Byte(0x6f);
	
	AD9954_CS=1;
	AD9954_Upadte();
}


















