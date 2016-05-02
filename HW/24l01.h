/****************************************Copyright (c)****************************************************
**                                       �����Ƽ���ѧ
**                                     
**                                         ����Э��
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           24l01.h
** Last modified Date: 
** Last Version: 
** Description:         NRF24L01����ͷ�ļ�
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          ����
** Created date:        2014/09/11
** Version:             V1.0
** Descriptions:		
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#ifndef __24L01_H__
#define __24L01_H__


/*********************************************************************************************************
	����ͷ�ļ�
*********************************************************************************************************/
//#include "Project_cfg.h"
#include "stm32f10x.h"


/*********************************************************************************************************
	�����궨��--NRF24L01�ڲ��Ĵ���ָ��
*********************************************************************************************************/
#define NRF_READ_REG	0x00  	// ���Ĵ���ָ��
#define NRF_WRITE_REG	0x20 	// д�Ĵ���ָ��
#define R_RX_PL_WID   	0x60
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����


/*********************************************************************************************************
	�����궨��--NRF24L01(SPI)�ڲ��Ĵ�����ַ
*********************************************************************************************************/
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define NRFRegSTATUS    0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������


/*********************************************************************************************************
	�����궨��--NRF24L01����
*********************************************************************************************************/
#define RX_DR			6		//�жϱ�־
#define TX_DS			5
#define MAX_RT			4
#define MAX_TX  		0x10  //�ﵽ����ʹ����ж�
#define TX_OK   		0x20  //TX��������ж�
#define RX_OK   		0x40  //���յ������ж�

#define MODEL_RX		1			//��ͨ����
#define MODEL_TX		2			//��ͨ����
#define MODEL_RX2		3			//����ģʽ2,����˫����
#define MODEL_TX2		4			//����ģʽ2,����˫����

#define RX_PLOAD_WIDTH  32  	
#define TX_PLOAD_WIDTH  32 
#define TX_ADR_WIDTH    5 	 	
#define RX_ADR_WIDTH    5   	


/*********************************************************************************************************
	�ⲿ��������
*********************************************************************************************************/
//extern 	u8 	NRF24L01_RXDATA[RX_PLOAD_WIDTH];		//nrf24l01���յ�������
//extern 	u8 	NRF24L01_TXDATA[TX_PLOAD_WIDTH];		//nrf24l01��Ҫ���͵�����


/*********************************************************************************************************
	������Ҫʹ�õ��ⲿ����
*********************************************************************************************************/
extern void NRF24L01_Init(u8 model, u8 ch);				//��ʼ��,model=1/2/3/4,chΪʵ�õ�ͨ����
extern void NRF24L01_TxPacket(uint8_t * tx_buf, uint8_t len);			//�������ݰ�,����model 2/4
extern void NRF24L01_TxPacket_AP(uint8_t * tx_buf, uint8_t len);	//�������ݰ�,����model 3
extern uint8_t NRF24L01_RxPacket(u8 *rxbuf);
extern uint8_t NRF24L01_Read_Reg(uint8_t reg);
extern uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);
extern uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars);
extern uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars);
extern uint8_t NRF24L01_Check(void);
void NRF24L01_IRQ_Handle(void);
void NRF24L01_Tx(void);
void NRF24L01_Rx(void);

/*********************************************************************************************************
	�˿ڶ���
*********************************************************************************************************/
#define SPI_CE_H()   GPIO_SetBits(GPIOA, GPIO_Pin_15) 
#define SPI_CE_L()   GPIO_ResetBits(GPIOA, GPIO_Pin_15)

#define SPI_CSN_H()  GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define SPI_CSN_L()  GPIO_ResetBits(GPIOA, GPIO_Pin_4)



#endif
