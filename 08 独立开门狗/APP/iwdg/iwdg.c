#include <iwdg.h>


/*
*   pre取值： 0， 1， 2， 3， 4， 5， 6
*	分频大小：4， 8，16，32，64，128，256
*   rlr： 0 ~ 4095
*   T = (4 * 2^pre * rlr) / 40000 
*   单位：S
*   注意：看门狗时钟是不准确的，早点喂狗！
*/

void Iwdg_Init(u8 pre,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//开启寄存器访问
	IWDG_SetPrescaler(pre);	//设置预分频系数
	IWDG_SetReload(rlr);	//设置重载值
	IWDG_ReloadCounter();	
	IWDG_Enable();	//使能开门狗
}


void Iwdg_FeedDog()
{
	IWDG_ReloadCounter();	// 喂狗
}
