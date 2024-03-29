#include <adc.h>
#include <SysTick.h>

void ADCx_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure; //定义 GPIO 结构体变量
	ADC_InitTypeDef   ADC_InitStructure;  // ADC 结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;  //ADC
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	//模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;  //独立模式  一个通道
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;  //非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  //关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //右对齐	
	ADC_InitStructure.ADC_NbrOfChannel = 1;  //1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);  //ADC初始化
	
	ADC_Cmd(ADC1,ENABLE);  //开启AD转换器
	
	ADC_ResetCalibration(ADC1);  //重置指定的ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));  //获取ADC重置校准寄存器的状态
	
	ADC_StartCalibration(ADC1);  //开始指定ADC的校准状态
	while(ADC_GetCalibrationStatus(ADC1));  //获取指定ADC的校准程序

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //使能或者失能指定的ADC的软件转换启动功能
} 

/*********************************************************************************
* 作用：获取转换结果平均值
* ch: 通道编号
* times：获取次数
**********************************************************************************/
u16 ADC_Data(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	//设置指定ADC的规则主通道，一个序列，采样实际时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADC通道，239.5个周期，提高采样时间可以提高精确度
	
	for(t=0;t<times;t++)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);  //使能指定的ADC1软件转换启动功能
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));  //等待转换结束
		temp_val+=ADC_GetConversionValue(ADC1);
		delay_ms(5);
	}
	return temp_val/times;
}

