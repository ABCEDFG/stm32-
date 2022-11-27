#include <dac.h>
#include <usart.h>

void DAC1_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);  //使能DAC时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //DAC_1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  //模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;  //不使用触发功能 TEN1=0
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;  //不使用波形发生
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;  //屏蔽、幅值设置
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;  //关闭输出缓存 BOFF1=1
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);
	
	DAC_SetChannel1Data(DAC_Align_12b_R,0);  //12位右对齐
	
	DAC_Cmd(DAC_Channel_1,ENABLE);  //使能DAC1
}
