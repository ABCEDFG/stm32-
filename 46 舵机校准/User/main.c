#include <SysTick.h>
#include <LED.h>
#include <pwm.h>
#include <key.h>

int main()
{
	u16 i=0;
	u8 flag = 1;
	u8 x = 0;
	u8 key;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();	//led初始化
	Tim3_Pwm_Init(20000,72-1);	//2k
	
	Key_Init();
	
//	while(1)
//	{
//		
//		TIM_SetCompare2(TIM3,1000);	
//		delay_ms(1500);
//		delay_ms(1500);
//		TIM_SetCompare2(TIM3,2000);	
//		delay_ms(1500);
//		delay_ms(1500);
//		if(Key(0)==1)
//		{
//			break;
//		}
//	}
//	TIM_SetCompare2(TIM3,1500);	
//	TIM_SetCompare2(TIM3,1500);	
	while(1)
	{
		key = Key(0);
		
		switch(key)
		{
			case 0:  break;
			case 2:  flag = 1;  break;
			case 3:  flag = 2;  break;
			default: break;
		}
		if(i > 100)
		{
			i = 0;
			if(flag == 1)
			{
				led0 = !led0;
				if(x == 0)
				{
					x = 1;
					TIM_SetCompare2(TIM3,  1000);
				}
				else
				{
					x = 0;
					TIM_SetCompare2(TIM3, 2000);
				}
			}
			else if(flag == 2)
			{
				led0 = 0;
				TIM_SetCompare2(TIM3,  1500);
			}
		}
		
		i++;
		delay_ms(10);
	}
}

