#include <adc_temp.h>
#include <sysTick.h>


//�ڲ����ţ����趨��GPIO
void ADC_Temp_Init()
{
	ADC_InitTypeDef   ADC_InitStructure;  // ADC �ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	ADC_TempSensorVrefintCmd(ENABLE);
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;  //����ģʽ
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
* ���ã���ȡch��ADת��ֵ
* ch: ͨ�����
* times����ȡ����
**********************************************************************************/
u16 ADC_Temp_Data(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʵ��ʱ�䣨����ʱ����ô�㣿��
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

//��ȡ�ڲ��¶�ֵ
int ADC_Temp()
{
	u16 value;
	double temp;
	value=ADC_Temp_Data(ADC_Channel_16,10);
	temp=(float)value*(3.3/4096);  //��ѹֵ  12λ
	return ((1.43-temp)/0.043+25)*100;  //�Ŵ�һ�ٱ����Ա�����λС��
}
