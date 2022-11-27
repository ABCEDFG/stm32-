#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"



int main()
{
	u8 i=0;
	short x0,y0,z0;
	short x1,y1,z1;
	short x2,y2,z2,n0;
	float x,y,z;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	
	MPU6050_Init();
	while(mpu_dmp_init())
	{
		printf("MPU6050错误！ /r/n");
		delay_ms(200);
		LED2=!LED2;
	}
	
	while(1)
	{
		if(mpu_dmp_get_data(&x,&y,&z)==0)
		{
			n0=MPU6050_Get_Temperature();
			MPU6050_Get_Accelerometer(&x1,&y1,&z1);
			MPU6050_Get_Gyroscope(&x0,&y0,&z0);
			x2=x*10;
			y2=y*10;
			z2=z*10;
			if(x2<0)
			{
				x2=-x2;
			}
			if(y2<0)
			{
				y2=-y2;
			}
			printf("A");
			printf("Pitch：%lf   Roll：%lf   Yaw：%lf \r\n",x,y,z);
		}
		
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
