//202141222
#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include "stdio.h"

#define  USART2_DEBUG		0



void USART1_Configuration(void);             //���ڳ�ʼ������
void USART2_Configuration(void);                        //���ڳ�ʼ������

void USART1_RX_IntteruptEnable(USART_TypeDef* USARTx);		//�����ж�ʹ��
int fputc(int ch, FILE *f);    //fputc�ض���
int fgetc(FILE *f); //fgetc�ض���

#endif
