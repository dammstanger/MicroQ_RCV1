/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��RCencode.h
 * ��	��	���ⲿ�жϳ�ʼ�����жϷ���
 *                    
 * ʵ��ƽ̨	��FCU V1.0
 * Ӳ������	��
 * �� 	��	��V1.0.150411
 * ������ϵ	��FCU_UCOSII150602_UCOSII2.91
 * ��汾	��ST3.0.0
 * ����ʱ��	��2014.4.11
 * ���༭	��2015.4.11
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

#ifndef __RCENCODE_H_
#define __RCENCODE_H_

/****************************����ͷ�ļ�*******************************************/
#include "stm32f10x.h"
//#include "Project_cfg.h"
/****************************�궨��***********************************************/

//KEY FUNC 
#define RC_KEY_MODE_OFF		1000
#define RC_KEY_MODE_ON		2000
#define RC_KEY_FUNC_NONE	1000
#define RC_KEY_FUNC_1		1100
#define RC_KEY_FUNC_2		1200
#define RC_KEY_FUNC_CALI	1500
#define RC_KEY_FUNC_DIVPAIR	2000

#define RC_CALIB_ORIN		3			//ҡ�˳�ʼλ�òɼ�
#define RC_CALIB_RANGE		2			//�����Сֵ������ϵ��
#define RC_CALIB_DONE		1			//У׼��ɱ�־
#define RC_CALIB_RDY		0			//У׼׼����
#define RC_NEED_CALIB		0xff		//У׼δ���
/****************************���Ͷ���*********************************************/
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

/****************************��������*********************************************/
extern RC_DATA RC_dat;
extern __IO u16 *p_ADCval;
extern RC_AD_RATIO RC_AD_ratio;
extern u8 RC_Calibr_sta;
extern u16 RC_key_mod ;
extern u16 RC_key_func;
/****************************��������*********************************************/

/****************************��������*********************************************/
u8 RC_Adc_Calib(u16 *pval);
void RC_RCdat_pkg(u16 *pval);
void RC_Para_LoadandCheck(void);
#endif
/******************* (C) COPYRIGHT 2015 DammStanger *****END OF FILE************/
