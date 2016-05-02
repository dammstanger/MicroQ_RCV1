//202141222
#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include "stdio.h"

#define  USART2_DEBUG		0



void USART1_Configuration(void);             //串口初始化函数
void USART2_Configuration(void);                        //串口初始化函数

void USART1_RX_IntteruptEnable(USART_TypeDef* USARTx);		//接收中断使能
int fputc(int ch, FILE *f);    //fputc重定向
int fgetc(FILE *f); //fgetc重定向

#endif
