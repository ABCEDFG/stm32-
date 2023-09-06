#include "spi.h"



void SPIx_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPI_NSS_PORT_RCC | SPI_SCK_PORT_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI_NSS_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
	GPIO_Init(SPI_NSS_PORT, &GPIO_InitStructure); 	   
	
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
	GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure); 	
	
#ifdef SPI_SINGLE_LINE_MODE_FLAG
	RCC_APB2PeriphClockCmd(SPI_DATA_PORT_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI_DATA_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
	GPIO_Init(SPI_DATA_PORT, &GPIO_InitStructure);
#else
	RCC_APB2PeriphClockCmd(SPI_MISO_PORT_RCC | SPI_MOSI_PORT_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
	GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure); 	
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入	 
	GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure); 	
#endif

	SPI_NSS = 1;
	SPI_SCK = 0;
	
}

/**
 * @brief 模拟 SPI 延时函数
 * @param count 延时时长，具体值未测
 */
void SPI_Delay(u32 count)
{
	u32 i = 0;
	u8 x = 12;
	
	for (i = 0; i < count; i++)
	{
		x = 12;
		while (x--);
	}
}


/**
 * @brief 单线 SPI 写模式
 */
void SPI_Mode_Write()
{
#ifdef SPI_SINGLE_LINE_MODE_FLAG
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SPI_DATA_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
	GPIO_Init(SPI_DATA_PORT, &GPIO_InitStructure);
#endif
}


/**
 * @brief 单线 SPI 读模式
 */
void SPI_Mode_Read()
{
#ifdef SPI_SINGLE_LINE_MODE_FLAG
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SPI_DATA_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
	GPIO_Init(SPI_DATA_PORT, &GPIO_InitStructure);
#endif
}


/**
 * @brief SPI 写函数（1 个字节）
 * @param data 要写的字节
 */
void SPI_Write_Byte(u8 data)
{
	u8 i = 0;
	u8 temp = 0;

	SPI_Mode_Write();
	for (i = 0; i < 8; i++)
	{
		temp = ((data & 0x80) == 0x80) ? 1 : 0; 
        data <<= 1;                      
        SPI_SCK = 0;
        SPI_DATA_W = temp;                
        SPI_Delay(SPI_DELAY_DUR);                 
        SPI_SCK = 1;        
        SPI_Delay(SPI_DELAY_DUR);  
	}
	SPI_SCK = 0;
}


/**
 * @brief SPI 读函数（1 个字节）
 * @return 读取的数据
 */
u8 SPI_Read_Byte()
{
	u8 i = 0;
    u8 read_data = 0xFF;
    
	SPI_Mode_Read();
	for(i = 0; i < 8; i++) 
	{
        read_data = read_data << 1;  
        SPI_SCK = 1;                  
        SPI_Delay(SPI_DELAY_DUR);
        SPI_SCK = 0;
        SPI_Delay(SPI_DELAY_DUR);
        if(SPI_DATA_R == 1) 
		{ 
           read_data = read_data + 1;
        }
    }
    SPI_SCK = 0;                      
	
    return read_data;
}


/**
 * @brief 模拟 SPI 读写函数（1个字节）
 * @param tdata 要写的数据
 * @return 读取的数据
 */
u8 SPI_ReadWrite_Byte(u8 tdata)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	uint8_t rdata = 0xff;
	
	for (i = 0; i < 8; i++)
	{
		temp = ((tdata & 0x80) == 0x80) ? 1 : 0;
		tdata <<= 1;
		rdata <<= 1;
		SPI_SCK = 0; 
		SPI_DATA_W = temp;  
		SPI_Delay(SPI_DELAY_DUR);
		SPI_SCK = 1;
		SPI_Delay(SPI_DELAY_DUR);
		if(SPI_DATA_R == 1) 
		{ 
           rdata = rdata + 1;
        }
	}
	SPI_SCK = 0; 
	
	return rdata;
}


/**
 * @brief SPI 写数据函数（多字节）
 * @param data 要写的数据首地址
 * @param len 数据长度
 */
void SPI_Write_Data(u8 *data, u16 len)
{
	while (len--)
	{
		SPI_Write_Byte(*data);
		++data;
	}
}


/**
 * @brief SPI 读数据函数（多字节）
 * @param data 存储读出数据空间的首地址
 * @param len 数据长度
 */
void SPI_Read_Data(u8 *data, u16 len)
{
	while (len--)
	{
		*data = SPI_Read_Byte();
		++data;
	}
}	


/**
 * @brief SPI 读写函数（多字节），读写 len 个字节的数据
 * @param tdata 写数据的首地址
 * @param rdata 存储读出数据空间的首地址
 * @param len 数据长度
 */
void SPI_ReadWrite_Data(u8 *tdata, u8 *rdata, u16 len)
{
	while (len--)
	{
		*rdata = SPI_ReadWrite_Byte(*tdata);
		++rdata;
		++tdata;
	}
}












