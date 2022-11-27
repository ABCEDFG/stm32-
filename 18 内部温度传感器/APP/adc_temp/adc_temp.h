#ifndef _adc_temp_H
#define _adc_temp_H

#include <system.h>

void ADC_Temp_Init(void);
u16 ADC_Temp_Data(u8 ch,u8 times);
int ADC_Temp(void);

#endif
