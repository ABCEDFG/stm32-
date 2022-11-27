#ifndef _hwjs_H
#define _hwjs_H

#include <system.h>

//定义全局变量
extern u32 hw_rec;
extern u8 hw_rec_flag;
extern u8 frame_cnt;

void Hwjs_Init(void);
u8 Hw_Jssj(void);


#endif
