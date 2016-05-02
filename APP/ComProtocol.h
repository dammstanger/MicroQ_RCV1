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
 * 最后编辑	：2015.6.6
 **-------------------------------------------------------------------------------

**********************************************************************************************/
#ifndef __COMPROTOCOL_H__
#define __COMPROTOCOL_H__


//#include "Project_cfg.h"
#include "stm32f10x.h"
#include "24l01.h"
#include "RCencode.h"

extern uint8_t RevFinish;

void Send_2401Dug_Pkg(u8 datype,int dat1,int dat2,int dat3,int dat4);//发送2401数据包
void Send_2401RC_Pkg(RC_DATA rcdat);
#endif
