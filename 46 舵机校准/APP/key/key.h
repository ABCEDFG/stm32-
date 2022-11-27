 #ifndef _key_H
 #define _key_H

#include "system.h"


#define k_up PAin(0)
#define k0   PCin(1)
#define k1   PCin(13)

//#define k_up  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
//#define k0    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
//#define K1    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

//#define K_up 1;
//#define K0   2;
//#define K1   3;

void Key_Init();
u8 Key(u8 mode);

#endif
