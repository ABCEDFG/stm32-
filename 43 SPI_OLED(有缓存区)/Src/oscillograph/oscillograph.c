#include "oscillograph.h"
#include "oled.h"
#include "time.h"


u32 Count_Hz(u16 *adcx)
{
	u16 i;
	u16 temp;
	u16 adcx1, adcx2;
	
	temp = adcx[0];
	for(i = 0; i < SAMPLE_NUM; i++)
	{
		if(adcx[i] != temp)
		{
			adcx1 = i;  break;
		}
	}
	
	if(adcx1 == i)
	{
		i++;
		for(; i < SAMPLE_NUM; i++)
		{
			if(adcx[i] == temp)
			{
				adcx2 = i;
				
				return (time_hz / (adcx2 - adcx1));
			}
		}
	}
	
	return 0;
}


