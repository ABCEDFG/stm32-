#include <adc.h>
#include <SysTick.h>

void ADCx_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure; //���� GPIO �ṹ�����
	ADC_InitTypeDef   ADC_InitStructure;  // ADC �ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;  //ADC
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	//ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;  //����ģʽ  һ��ͨ��
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;  //��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  //�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //�Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = 1;  //1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);  //ADC��ʼ��
	
	ADC_Cmd(ADC1,ENABLE);  //����ADת����
	
	ADC_ResetCalibration(ADC1);  //����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));  //��ȡADC����У׼�Ĵ�����״̬
	
	ADC_StartCalibration(ADC1);  //��ʼָ��ADC��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));  //��ȡָ��ADC��У׼����

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //ʹ�ܻ���ʧ��ָ����ADC�����ת����������
} 

/*********************************************************************************
* ���ã���ȡת�����ƽ��ֵ
* ch: ͨ�����
* times����ȡ����
**********************************************************************************/
u16 ADC_Data(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʵ��ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADCͨ����239.5�����ڣ���߲���ʱ�������߾�ȷ��
	
	for(t=0;t<times;t++)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);  //ʹ��ָ����ADC1���ת����������
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));  //�ȴ�ת������
		temp_val+=ADC_GetConversionValue(ADC1);
		delay_ms(5);
	}
	return temp_val/times;
}

