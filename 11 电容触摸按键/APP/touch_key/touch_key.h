#ifndef _touch_key_H
#define	_touch_key_H

#include <system.h>



extern u16 touch_val;

void Touch_Key_Init(u16 pre,u16 psc);
void Touch_Reset(void);
u16 Touch_Get_Val(void);
u8 Touch_Init(u8 psc);
u16 Touch_Get_MaxVal(u8 n);
u8 Touch_Key_Scan(u8 mode);


#endif

