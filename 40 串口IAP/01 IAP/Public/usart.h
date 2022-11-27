#ifndef _usart_H
#define _usart_H

#include "system.h" 
#include <stdio.h>


#define USART1_REC_LEN		55*1024 //�����������ֽ��� 55K

extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���
extern u16 USART1_RX_CNT;				//���յ��ֽ���


void Usart1_Init(u32 bound);


#endif


