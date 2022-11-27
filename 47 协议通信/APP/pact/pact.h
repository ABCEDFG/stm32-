#ifndef _pact_H
#define _pact_H

#include <system.h>

#define BYET0(x) (*((u8 *)(&x)))    // 0~7   字节
#define BYET1(x) (*((u8 *)(&x)+1))  // 8~15  字节
#define BYET2(x) (*((u8 *)(&x)+2))  // 15~23 字节
#define BYET3(x) (*((u8 *)(&x)+3))  // 24~31 字节

extern u16 pact_a;
extern s16 pact_b;
extern s32 pact_c;

void Pact_Send(u16 a,s16 b,s32 c);
void Pact_Receive(u8 data);

#endif

