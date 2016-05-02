/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��RCencode.c
 * ��	��	���ⲿ�жϳ�ʼ�����жϷ���
 *                    
 * ʵ��ƽ̨	��FCU V1.0
 * Ӳ������	��
 * �� 	��	��V1.0.150412
 * ������ϵ	��FCU_UCOSII150602_UCOSII2.91
 * ��汾	��ST3.0.0
 * ����ʱ��	��2014.4.11
 * ���༭	��2015.4.12
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/



/****************************����ͷ�ļ�*******************************************/
#include "RCencode.h"
#include "adc.h"
#include "led.h"
#include "key.h"
#include "Systick.h"
/****************************�궨��***********************************************/
//ͨ��ӳ��
#define ADC_ROLL		2
#define ADC_PITCH		3
#define ADC_THRO		1
#define ADC_YAW			0

#define ADC_ROLL_MAX	40		



/****************************��������*********************************************/

/****************************��������*********************************************/
RC_DATA adc_orig ={2080,2030,3955,2012,0,0};			
RC_DATA_f ratio_adc_RC ={4.032f,4.095f,3.955f,4.095f,0.0f,0.0f};
RC_DATA RC_dat={1500,1500,1000,1500,1000,1000};
__IO u16 *p_ADCval=ADC_ConvertedValue;
/****************************��������*********************************************/


/********************************************************************************
 * ��������RC_Adc_Calib()
 * ����  ��ADCͨ����λֵ���������ֵУ׼
 * ����  ��-		    	
 * ����  ��-
 * ����  ���ⲿ
 ********************************************************************************/
u8 RC_Adc_Calib(u16 *pval)
{
	static u8 cnt = 0;
	static u32 add[4] = {0};

	if(cnt++<50)
	{
		add[0] += pval[ADC_ROLL];
		add[1] += pval[ADC_PITCH];
		add[2] += pval[ADC_THRO];
		add[3] += pval[ADC_YAW];	
		return 0;
	}
	else
	{
		cnt = 0;
		adc_orig.ROLL = add[0]/50;
		adc_orig.PITCH = add[1]/50;
		adc_orig.THROTTLE= add[2]/50;
		adc_orig.YAW = add[3]/50;
		add[0] = 0,add[1] = 0,add[2] = 0,add[3] = 0;
		return 1;
	}
}
	


/********************************************************************************
 * ��������RC_Adc_to_RCdat()
 * ����  ��ADCͨ��ֵ��RCͨ����ת��
 * ����  ��-		    	
 * ����  ��-
 * ����  ���ⲿ
 ********************************************************************************/
void RC_Adc_to_RCdat(u16 *pval)
{
	RC_dat.ROLL= 1500 + (((s16)adc_orig.ROLL-(s16)pval[ADC_ROLL])/ratio_adc_RC.ROLL);
	RC_dat.PITCH= 1500 + (((s16)adc_orig.PITCH-(s16)pval[ADC_PITCH])/ratio_adc_RC.PITCH);
	RC_dat.THROTTLE= 1000 + (((s16)adc_orig.THROTTLE-(s16)pval[ADC_THRO])/ratio_adc_RC.THROTTLE);
	RC_dat.YAW= 1500 + (((s16)pval[ADC_YAW]-(s16)adc_orig.YAW)/ratio_adc_RC.YAW);
}


void RC_RCdat_pkg(u16 *pval)
{
	u8 val;
	RC_Adc_to_RCdat(pval);
	if(!KEY_MODE_Val)
	{
		Delay_ms(10);
		val = KEY_MODE_Val;
		if(!val)
		{
			RC_dat.MODE = 2000;
		}
	}
	else
	{
		RC_dat.MODE = 1000;
	}
	if(!KEY_FUNC_Val)
	{
		Delay_ms(10);
		val = KEY_FUNC_Val;
		if(!val)
		{
			RC_dat.FUNC = 2000;
		}
	}
	else
	{
		RC_dat.FUNC = 1000;
	}
}

		
/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/
