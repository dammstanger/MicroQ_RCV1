/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��ComProtocol.c
 * ��	��	���ⲿ�жϳ�ʼ�����жϷ���
 *                    
 * ʵ��ƽ̨	��FCU V1.0
 * Ӳ������	��
 * �� 	��	��V1.0.1506.3
 * ������ϵ	��FCU_UCOSII150603_UCOSII2.91
 * ��汾	��ST3.0.0
 * ����ʱ��	��2014.4.11
 * ���༭	��2015.6.10
 **-------------------------------------------------------------------------------

**********************************************************************************************/



/****************************����ͷ�ļ�*******************************************/
#include "ComProtocol.h"

uint8_t BUF_R[32] = {0};
uint8_t BUF_T[32] = {0};
uint8_t RevFinish = 0;


void Send_2401Dug_Pkg(u8 datype,int dat1,int dat2,int dat3,int dat4)//�������ݰ�
{
	s16 i;
	i=(s16)dat1;
	BUF_T[0] = datype;
	BUF_T[1] = (u8)(i>>8);
	BUF_T[2] = (u8)(i&0xff);
	
	i=(s16)dat2;
	BUF_T[3] = (u8)(i>>8);
	BUF_T[4] = (u8)(i&0xff);
	
	i=(s16)dat3;
	BUF_T[5] = (u8)(i>>8);
	BUF_T[6] = (u8)(i&0xff);
	
	i=(s16)dat4;
	BUF_T[7] = (u8)(i>>8);
	BUF_T[8] = (u8)(i&0xff);
	BUF_T[9] = 0xaa;
	
	NRF24L01_Tx();					
	NRF24L01_TxPacket(BUF_T,10);
//	NRF24L01_Rx();	
}


void Send_2401RC_Pkg(RC_DATA rcdat)
{
	BUF_T[0] = 0;					//��λ��ǰ
	BUF_T[1] = 0x80;
	BUF_T[2] = (u8)rcdat.PITCH;
	BUF_T[3] = rcdat.PITCH>>8;
	BUF_T[4] = (u8)rcdat.YAW;
	BUF_T[5] = rcdat.YAW>>8;
	BUF_T[6] = (u8)rcdat.ROLL;
	BUF_T[7] = rcdat.ROLL>>8;
	BUF_T[8] = (u8)rcdat.THROTTLE;
	BUF_T[9] = rcdat.THROTTLE>>8;
	BUF_T[10] = (u8)rcdat.MODE;
	BUF_T[11] = rcdat.MODE>>8;
	BUF_T[12] = (u8)rcdat.FUNC;
	BUF_T[13] = rcdat.FUNC>>8;
	
	NRF24L01_Tx();					
	NRF24L01_TxPacket(BUF_T,32);
//	NRF24L01_Rx();
}	
