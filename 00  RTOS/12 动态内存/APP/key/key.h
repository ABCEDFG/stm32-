 #ifndef _key_H
 #define _key_H

#include "system.h"


#define k_up PAin(0)
#define k0   PEin(4)
#define k1   PEin(3)

//#define k_up  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
//#define k0    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
//#define K1    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

#define K_up 1;
#define K0   2;
#define K1   3;

void Key_Init(void);
u8 Key_Scan(u8 mode);

#endif
