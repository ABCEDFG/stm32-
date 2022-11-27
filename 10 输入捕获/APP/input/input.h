#ifndef _input_H
#define	_input_H

#include <system.h>

extern u8  sta;
extern u16 val;

void Input_Init(u16 pre,u16 psc);
u32 Input_Scan(void);


#endif
