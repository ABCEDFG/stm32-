#ifndef _can_H
#define _can_H

#include <system.h>

void CANx_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
 
u8 CAN_Send_Msg(u8* msg,u8 len);						//发送数据

u8 CAN_Receive_Msg(u8 *buf);							//接收数据

#endif
