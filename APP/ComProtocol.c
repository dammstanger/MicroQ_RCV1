/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：ComProtocol.c
 * 描	述	：外部中断初始化和中断服务
 *                    
 * 实验平台	：FCU V1.0
 * 硬件连接	：
 * 版 	本	：V1.0.1506.3
 * 从属关系	：FCU_UCOSII150603_UCOSII2.91
 * 库版本	：ST3.0.0
 * 创建时间	：2014.4.11
 * 最后编辑	：2015.6.10
 **-------------------------------------------------------------------------------

**********************************************************************************************/



/****************************包含头文件*******************************************/
#include "ComProtocol.h"

uint8_t BUF_R[32] = {0};
uint8_t BUF_T[32] = {0};
uint8_t RevFinish = 0;


void Send_2401Dug_Pkg(u8 datype,int dat1,int dat2,int dat3,int dat4)//发送数据包
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
	BUF_T[0] = 0;					//低位在前
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
