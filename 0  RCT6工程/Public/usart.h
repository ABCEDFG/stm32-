#ifndef _usart_H
#define _usart_H

#include "system.h" 
#include "stdio.h" 


#define DEBUG_FLAG
#ifdef  DEBUG_FLAG
#define DEBUG_PRINT(format, ...) printf("DEBUG: %s  %s  %d:  " format "\r\n",__FILE__, __func__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(x, ...)
#endif


void Usart1_Init(u32 bound);


#endif


