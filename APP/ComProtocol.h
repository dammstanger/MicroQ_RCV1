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
 * ���༭	��2015.6.6
 **-------------------------------------------------------------------------------

**********************************************************************************************/
#ifndef __COMPROTOCOL_H__
#define __COMPROTOCOL_H__


//#include "Project_cfg.h"
#include "stm32f10x.h"
#include "24l01.h"
#include "RCencode.h"

extern uint8_t RevFinish;

void Send_2401Dug_Pkg(u8 datype,int dat1,int dat2,int dat3,int dat4);//����2401���ݰ�
void Send_2401RC_Pkg(RC_DATA rcdat);
#endif
