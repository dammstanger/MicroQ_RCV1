/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：RCencode.h
 * 描	述	：外部中断初始化和中断服务
 *                    
 * 实验平台	：FCU V1.0
 * 硬件连接	：
 * 版 	本	：V1.0.150411
 * 从属关系	：FCU_UCOSII150602_UCOSII2.91
 * 库版本	：ST3.0.0
 * 创建时间	：2014.4.11
 * 最后编辑	：2015.4.11
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

#ifndef __RCENCODE_H_
#define __RCENCODE_H_

/****************************包含头文件*******************************************/
#include "stm32f10x.h"
//#include "Project_cfg.h"
/****************************宏定义***********************************************/

//KEY FUNC 
#define RC_KEY_MODE_OFF		1000
#define RC_KEY_MODE_ON		2000
#define RC_KEY_FUNC_NONE	1000
#define RC_KEY_FUNC_1		1100
#define RC_KEY_FUNC_2		1200
#define RC_KEY_FUNC_CALI	1500
#define RC_KEY_FUNC_DIVPAIR	2000

#define RC_CALIB_ORIN		3			//摇杆初始位置采集
#define RC_CALIB_RANGE		2			//最大最小值及比例系数
#define RC_CALIB_DONE		1			//校准完成标志
#define RC_CALIB_RDY		0			//校准准备好
#define RC_NEED_CALIB		0xff		//校准未完成
/****************************类型定义*********************************************/
typedef struct 
{
	u16 ROLL;
	u16 PITCH;
	u16 THROTTLE;
	u16 YAW;
	u16 MODE;
	u16 FUNC;
}RC_DATA;

typedef struct 
{
	float ROLL;
	float PITCH;
	float THROTTLE;
	float YAW;
	float MODE;
	float FUNC;
}RC_DATA_f;

typedef union 
{
	RC_DATA_f RC_AD_ratio_f;
	u16 RC_AD_ratio_chr[12];
}RC_AD_RATIO;

/****************************变量声明*********************************************/
extern RC_DATA RC_dat;
extern __IO u16 *p_ADCval;
extern RC_AD_RATIO RC_AD_ratio;
extern u8 RC_Calibr_sta;
extern u16 RC_key_mod ;
extern u16 RC_key_func;
/****************************变量定义*********************************************/

/****************************函数声明*********************************************/
u8 RC_Adc_Calib(u16 *pval);
void RC_RCdat_pkg(u16 *pval);
void RC_Para_LoadandCheck(void);
#endif
/******************* (C) COPYRIGHT 2015 DammStanger *****END OF FILE************/
