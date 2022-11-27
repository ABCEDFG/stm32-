#include <wkup.h>



void Wkup_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);	//使能PWR外社时钟
	
	PWR_ClearFlag(PWR_FLAG_WU);	//清除wake-up标志
	
	PWR_WakeUpPinCmd(ENABLE);	//使能唤醒管角(WK_UP键唤醒) 使能或者失能唤醒管教功能
	
	PWR_EnterSTANDBYMode();	//进入待机模式
	
	
//	PWR_EnterSTOPMode(PWR_Regulator_ON,     // PWR_Regulator_ON 打开电压转换器;  PWR_Regulator_LowPower   电压转换器进入低功耗模式 
//					  PWR_STOPEntry_WFI);   // PWR_STOPEntry_WFI  任意中断唤醒;  PWR_STOPEntry_WFE        唤醒事件唤醒
}
 
