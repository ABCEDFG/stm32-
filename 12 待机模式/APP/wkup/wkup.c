#include <wkup.h>



void Wkup_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);	//ʹ��PWR����ʱ��
	
	PWR_ClearFlag(PWR_FLAG_WU);	//���wake-up��־
	
	PWR_WakeUpPinCmd(ENABLE);	//ʹ�ܻ��ѹܽ�(WK_UP������) ʹ�ܻ���ʧ�ܻ��ѹ̹ܽ���
	
	PWR_EnterSTANDBYMode();	//�������ģʽ
	
	
//	PWR_EnterSTOPMode(PWR_Regulator_ON,     // PWR_Regulator_ON �򿪵�ѹת����;  PWR_Regulator_LowPower   ��ѹת��������͹���ģʽ 
//					  PWR_STOPEntry_WFI);   // PWR_STOPEntry_WFI  �����жϻ���;  PWR_STOPEntry_WFE        �����¼�����
}
 
