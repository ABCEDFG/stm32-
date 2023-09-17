#ifndef _usart_H
#define _usart_H

#include "system.h" 
#include "stdio.h" 


#define DEBUG_FLAG
#ifdef  DEBUG_FLAG
#define DEBBUG_PRINT(x, ...)  printf(x, ##__VA_ARGS__)
#else
#define DEBBUG_PRINT(x, ...)
#endif


void Usart1_Init(u32 bound);


#endif


