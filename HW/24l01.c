/****************************************Copyright (c)****************************************************
**                                       �����Ƽ���ѧ
**                                     
**                                         ����Э��
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           nrf24l01.c
** Last modified Date: 
** Last Version: 
** Description:         NRF24L01����Դ�ļ�
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


/*********************************************************************************************************
	����ͷ�ļ�
*********************************************************************************************************/
#include "24l01.h"
#include "spi.h"
#include "EXTI.h"
#include "usart.h"

/*********************************************************************************************************
	��������
*********************************************************************************************************/
u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���յ�ַ

/*********************************************************************************************************
	������Ҫʹ�õ��ڲ�����
*********************************************************************************************************/
void NRF24L01_PortInit(void);


/*********************************************************************************************************
** Function name:       NRF24L01_PortInit
** Descriptions:        NRF24L01�˿�����
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void NRF24L01_PortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  //ʹ��PORTA��ʱ�� 
  //ʹ��PORTB��ʱ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	SPI_CE_L();
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	SPI_CSN_H();
		
	SPI1_Init();    //��ʼ��SPI
//	SPI_CE_L(); 	//ʹ��24L01
//	SPI_CSN_H();	//SPIƬѡȡ��	
//	
	/*
	 *	����IRQ�ⲿ�ж�
	 */
	 EXTI_PA11_24L01_Init();
}


/*********************************************************************************************************
** Function name:       NRF24L01_Write_Reg
** Descriptions:        NRF24L01д�Ĵ���
** input parameters:    reg:	�Ĵ�����ַ
**						value:	ֵ
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	SPI_CSN_L();					  /* ѡͨ���� */
	status = SPI1_ReadWriteByte(reg);  /* д�Ĵ�����ַ */
	SPI1_ReadWriteByte(value);		  /* д���� */
	SPI_CSN_H();					  /* ��ֹ������ */
	return 	status;
}


/*********************************************************************************************************
** Function name:       NRF24L01_Read_Reg
** Descriptions:        NRF24L01д�Ĵ���
** input parameters:    reg:	�Ĵ�����ַ
** output parameters:   none
** Returned value:      ������ֵ
*********************************************************************************************************/
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	SPI_CSN_L();					  /* ѡͨ���� */
	SPI1_ReadWriteByte(reg);			  /* д�Ĵ�����ַ */
	reg_val = SPI1_ReadWriteByte(0);	  /* ��ȡ�üĴ����������� */
	SPI_CSN_H();					  /* ��ֹ������ */
    return 	reg_val;
}


/*********************************************************************************************************
** Function name:       NRF24L01_Write_Buf
** Descriptions:        NRF24L01д������
** input parameters:    reg:	�������׵�ַ
**						value:	�����׵�ַ
**						uchars:	���ݳ���
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	SPI_CSN_L();				        /* ѡͨ���� */
	status = SPI1_ReadWriteByte(reg);	/* д�Ĵ�����ַ */
	for(i=0; i<uchars; i++)
	{
		SPI1_ReadWriteByte(pBuf[i]);		/* д���� */
	}
	SPI_CSN_H();						/* ��ֹ������ */
    return 	status;	
}


/*********************************************************************************************************
** Function name:       NRF24L01_Read_Buf
** Descriptions:        NRF24L01��������
** input parameters:    reg:	�������׵�ַ
**						value:	�����׵�ַ
**						uchars:	���ݳ���
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	SPI_CSN_L();						/* ѡͨ���� */
	status = SPI1_ReadWriteByte(reg);	/* д�Ĵ�����ַ */
	for(i=0; i<uchars; i++)
	{
		pBuf[i] = SPI1_ReadWriteByte(0); /* ��ȡ�������� */ 	
	}
	SPI_CSN_H();						/* ��ֹ������ */
    return 	status;
}


/*********************************************************************************************************
** Function name:       NRF24L01_TxPacket
** Descriptions:        NRF24L01�������ݰ�
** input parameters:    tx_buf:	�����׵�ַ
**						len:	���ݳ���
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void NRF24L01_TxPacket(uint8_t * tx_buf, uint8_t len)
{	
	SPI_CE_L();		 //StandBy Iģʽ	
	
//	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	NRF24L01_Write_Buf(WR_TX_PLOAD, tx_buf, len); 			 // װ������	
	SPI_CE_H();		 //�ø�CE���������ݷ���
}


void NRF24L01_TxPacket_AP(uint8_t * tx_buf, uint8_t len)
{	
	SPI_CE_L();		 //StandBy Iģʽ	
	NRF24L01_Write_Buf(0xa8, tx_buf, len); 			 // װ������
	SPI_CE_H();		 //�ø�CE
}


/*********************************************************************************************************
** Function name:       NRF24L01_RxPacket
** Descriptions:        NRF24L01�������ݰ�
** input parameters:    tx_buf:	�����׵�ַ
** output parameters:   none
** Returned value:      0��������ɣ��������������
*********************************************************************************************************/
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	sta=NRF24L01_Read_Reg(NRFRegSTATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+NRFRegSTATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}
	return 1;//û�յ��κ�����
}


/*********************************************************************************************************
** Function name:       Nrf24l01_Check
** Descriptions:        NRF24L01�Լ�
** input parameters:    none
** output parameters:   none
** Returned value:      0:ERROR		others:SUCCESS
*********************************************************************************************************/
u8 NRF24L01_Check(void)
{ 
	u8 buf1[5]; 
	u8 i; 
	/*д��5���ֽڵĵ�ַ. */ 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5); 
	/*����д��ĵ�ַ */ 
	NRF24L01_Read_Buf(TX_ADDR,buf1,5); 
	/*�Ƚ�*/ 
	printf("%s",buf1);
	for(i=0;i<5;i++) 
	{ 
		if(buf1[i]!=TX_ADDRESS[i]) 
			break; 
	} 
	if(i==5)
		return SUCCESS ; //MCU��NRF�ɹ����� 
	else
		return ERROR ; //MCU��NRF���������� 
}


/*********************************************************************************************************
** Function name:       NRF24L01_Init
** Descriptions:        NRF24L01��ʼ��
** input parameters:    model:	ģʽѡ��
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
void NRF24L01_Init(u8 model, u8 ch)
{
	NRF24L01_PortInit();
	
	SPI_CE_L();
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);	//дRX�ڵ��ַ 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH); 		//дTX�ڵ��ַ  
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x00);							//��ֹ����ͨ�����Զ�Ӧ�� 
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);						//ʹ��ͨ��0�Ľ��յ�ַ 
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x00);						//��ֹ�Զ��ط�
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,ch);								//����RFͨ��ΪCHANAL
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); 						//����TX�������,0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg(NRF_WRITE_REG+NRFRegSTATUS,MAX_TX|TX_OK|RX_OK); 		//����ж�
/////////////////////////////////////////////////////////
	if(model==1)			 //RX
	{
		NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//ѡ��ͨ��0����Ч���ݿ�� 
		NRF24L01_Write_Reg(FLUSH_TX,0xff);
		NRF24L01_Write_Reg(FLUSH_RX,0xff);
		NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x3f);   		 // IRQ�շ�����жϿ���,16λCRC,������
	}
	else if(model==2)		//TX
	{
		NRF24L01_Write_Reg(FLUSH_RX,0xff);
		NRF24L01_Write_Reg(FLUSH_TX,0xff);
		NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);	 	//ѡ��ͨ��0����Ч���ݿ�� 
		NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 	// IRQ������жϿ���,16λCRC,������  
	}																	//bit4=0����ط��жϷ�ӳ��IRO��
																	 	//bit5=0��������жϷ�ӳ��IRO��
																		//bit6=0�����жϷ�ӳ��IRO��
	
	else if(model==3)		//RX2
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);
		NRF24L01_Write_Reg(FLUSH_RX,0xff);
		NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x3f);   		 // IRQ�շ�����жϿ���,16λCRC,������
		
		SPI1_ReadWriteByte(0x50);
		SPI1_ReadWriteByte(0x73);
		NRF24L01_Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		NRF24L01_Write_Reg(NRF_WRITE_REG+0x1d,0x07);
	}
	else								//TX2
	{
		NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����жϿ���,16λCRC,������
		NRF24L01_Write_Reg(FLUSH_TX,0xff);
		NRF24L01_Write_Reg(FLUSH_RX,0xff);
		
		SPI1_ReadWriteByte(0x50);
		SPI1_ReadWriteByte(0x73);
		NRF24L01_Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		NRF24L01_Write_Reg(NRF_WRITE_REG+0x1d,0x07);
	}
	SPI_CE_H();
}


void NRF24L01_Tx(void)
{
	SPI_CE_L();
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ������жϿ���,16λCRC,������
	SPI_CE_H();
}



void NRF24L01_Rx(void)
{
	u8 sta = 0;
	while(!(sta&TX_OK))							//�ȴ��������
	{
		sta=NRF24L01_Read_Reg(NRFRegSTATUS);  	//��ȡ״̬�Ĵ�����ֵ
	}
	SPI_CE_L();
	NRF24L01_Write_Reg(NRF_WRITE_REG+NRFRegSTATUS,sta); 			//���TX_DS��MAX_RT�жϱ�־
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x3f);   		 	// IRQ������жϿ���,16λCRC,������
	SPI_CE_H();
}



void NRF24L01_IRQ_Handle(void)
{
//	u8 sta = 0;
//	sta=NRF24L01_Read_Reg(NRFRegSTATUS);  	//��ȡ״̬�Ĵ�����ֵ
//	NRF24L01_Write_Reg(NRF_WRITE_REG+NRFRegSTATUS,sta); 			//���TX_DS��MAX_RT�жϱ�־
}

