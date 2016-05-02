//20141222
#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"


// SPI�����ٶ����� Ҳ��������Ԥ��Ƶ�Ĵ���
#define SPI_SPEED_2   	0
#define SPI_SPEED_4   	1
#define SPI_SPEED_8  	2
#define SPI_SPEED_16 	3
#define SPI_SPEED_32   	4
#define SPI_SPEED_64   	5
#define SPI_SPEED_128  	6
#define SPI_SPEED_256 	7
			  	    													  
void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

