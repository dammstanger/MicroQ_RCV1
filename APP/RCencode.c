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
#include "flash.h"
/****************************�궨��***********************************************/
//ͨ��ӳ��
#define ADC_ROLL		2
#define ADC_PITCH		3
#define ADC_THRO		1
#define ADC_YAW			0

#define ADC_ROLL_MAX	40		


/****************************��������*********************************************/
// = {4.032f,4.095f,3.955f,4.095f,0.0f,0.0f};
/****************************��������*********************************************/
RC_DATA adc_orig ={2080,2030,3955,2012,0,0};			
//RC_DATA_f ratio_adc_RC ={4.032f,4.095f,3.955f,4.095f,0.0f,0.0f};
RC_DATA RC_dat={1500,1500,1000,1500,1000,1000};

RC_AD_RATIO RC_AD_ratio = {4.032f,4.095f,3.955f,4.095f,0.0f,0.0f};

u8 RC_Calibr_sta = 0;					//RCҡ��У׼״̬
u16 RC_key_mod = RC_KEY_MODE_OFF;
u16 RC_key_func= RC_KEY_FUNC_NONE;
__IO u16 *p_ADCval=ADC_ConvertedValue;
/****************************��������*********************************************/
void RC_Para_Save(RC_AD_RATIO ratio,RC_DATA oringe);


#define ADDCNT		100
/********************************************************************************
 * ��������RC_Adc_Calib()
 * ����  ��ADCͨ����λֵ���������ֵУ׼
 * ����  ��-		    	
 * ����  ��-��ɷ���1�����򷵻�0
 * ����  ���ⲿ
 ********************************************************************************/
u8 RC_Adc_Calib(u16 *pval)
{
	static u16 cnt = 0;
	static u32 add[4] = {0};
	static u16 Max[4] = {0};				//���ڼ�¼���ֵ����ֵ
	static u16 Min[4] = {4095};				//���ڼ�¼���ֵ���Сֵ

	switch(RC_Calibr_sta)
	{
		case RC_CALIB_ORIN :{
			if(cnt++<ADDCNT)
			{
				add[0] += pval[ADC_ROLL];
				add[1] += pval[ADC_PITCH];
				add[2] += pval[ADC_THRO];
				add[3] += pval[ADC_YAW];	
			}
			
			else
			{
				cnt = 0;
				adc_orig.ROLL = add[0]/ADDCNT;
				adc_orig.PITCH = add[1]/ADDCNT;
				adc_orig.THROTTLE= add[2]/ADDCNT;
				adc_orig.YAW = add[3]/ADDCNT;
				add[0] = 0,add[1] = 0,add[2] = 0,add[3] = 0;
				RC_Calibr_sta = RC_CALIB_RANGE;
			}
		}break;
		case RC_CALIB_RANGE :{
			if(cnt++<300)
			{
				if(pval[ADC_ROLL]<=4095)
				{
					if(Max[0]<pval[ADC_ROLL])	Max[0] = pval[ADC_ROLL];
					if(Min[0]>pval[ADC_ROLL])	Min[0] = pval[ADC_ROLL];
				}
				if(pval[ADC_PITCH]<=4095)
				{
					if(Max[1]<pval[ADC_PITCH])	Max[1] = pval[ADC_PITCH];
					if(Min[1]>pval[ADC_PITCH])	Min[1] = pval[ADC_PITCH];
				}
				if(pval[ADC_THRO]<=4095)
				{
					if(Max[2]<pval[ADC_THRO])	Max[2] = pval[ADC_THRO];
					if(Min[2]>pval[ADC_THRO])	Min[2] = pval[ADC_THRO];
				}
				if(pval[ADC_YAW]<=4095)
				{
					if(Max[3]<pval[ADC_YAW])	Max[3] = pval[ADC_YAW];
					if(Min[3]>pval[ADC_YAW])	Min[3] = pval[ADC_YAW];
				}	
			}
			else{
				cnt = 0;															//�Ա��´�У׼
				RC_AD_ratio.RC_AD_ratio_f.ROLL = (Max[0] - Min[0])/1000.0;			//�������ϵ��
				RC_AD_ratio.RC_AD_ratio_f.PITCH = (Max[1] - Min[1])/1000.0;
				RC_AD_ratio.RC_AD_ratio_f.THROTTLE= (Max[2] - Min[2])/1000.0;
				RC_AD_ratio.RC_AD_ratio_f.YAW = (Max[3] - Min[3])/1000.0;
				
				RC_Para_Save(RC_AD_ratio,adc_orig);
				RC_Calibr_sta = RC_CALIB_DONE;
			} 			
		}break;
		default:break;
	}
	if(RC_Calibr_sta==RC_CALIB_DONE)
		return 1;
	else return 0;
	
}



/********************************************************************************
 * ��������RC_Para_Save()
 * ����  ��RC��������Flash
 * ����  ��-		    	
 * ����  ��-
 * ����  ���ڲ�
 ********************************************************************************/
void RC_Para_Save(RC_AD_RATIO ratio,RC_DATA oringe)
{
	u8 i;
	FlashWbuf[0] = 1;
	for(i=1;i<9;i++)
		FlashWbuf[i] = ratio.RC_AD_ratio_chr[i-1];
	FlashWbuf[9] = oringe.ROLL;
	FlashWbuf[10] = oringe.PITCH;
	FlashWbuf[11] = oringe.THROTTLE;
	FlashWbuf[12] = oringe.YAW;
	
	STMFLASH_Write(FLASH_ROOM1_ADDR,FlashWbuf,13);
}

/********************************************************************************
 * ��������RC_Para_LoadandCheck()
 * ����  ������Flash�е�RC����
 * ����  ��-		    	
 * ����  ��
 * ����  ���ⲿ
 ********************************************************************************/
void RC_Para_LoadandCheck()
{
	u8 i;
	STMFLASH_Read(FLASH_ROOM1_ADDR,FlashRbuf,13);
	RC_Calibr_sta = FlashRbuf[0];
	if(RC_Calibr_sta==RC_CALIB_DONE)
	{
		for(i=1;i<9;i++)
			RC_AD_ratio.RC_AD_ratio_chr[i-1] = FlashRbuf[i];
		adc_orig.ROLL = FlashRbuf[9];
		adc_orig.PITCH = FlashRbuf[10];
		adc_orig.THROTTLE = FlashRbuf[11];
		adc_orig.YAW = FlashRbuf[12];
	}
	else{
		RC_Calibr_sta = RC_NEED_CALIB;			//ҡ��δУ׼
	}
}
	


/********************************************************************************
 * ��������RC_Adc_to_RCdat()
 * ����  ��ADCͨ��ֵ��RCͨ����ת������0-4095 ת����1000-2000
 * ����  ��-		    	
 * ����  ��-
 * ����  ���ⲿ
 ********************************************************************************/
void RC_Adc_to_RCdat(u16 *pval)
{
	static u16 datlast[4] ={0};
	if(pval[ADC_ROLL]<=4095)	datlast[ADC_ROLL] = pval[ADC_ROLL];			//ȥ���쳣ֵ��������һ�ε�ֵ
	if(pval[ADC_PITCH]<=4095) datlast[ADC_PITCH] = pval[ADC_PITCH];			
	if(pval[ADC_THRO]<=4095)	datlast[ADC_THRO] = pval[ADC_THRO];			
	if(pval[ADC_YAW]<=4095)	datlast[ADC_YAW] = pval[ADC_YAW];			
	
	RC_dat.ROLL= 1500 + (((s16)adc_orig.ROLL-(s16)datlast[ADC_ROLL])/RC_AD_ratio.RC_AD_ratio_f.ROLL);
	RC_dat.PITCH= 1500 + (((s16)adc_orig.PITCH-(s16)datlast[ADC_PITCH])/RC_AD_ratio.RC_AD_ratio_f.PITCH);
	RC_dat.THROTTLE= 1000 + (((s16)adc_orig.THROTTLE-(s16)datlast[ADC_THRO])/RC_AD_ratio.RC_AD_ratio_f.THROTTLE);
	RC_dat.YAW= 1500 + (((s16)datlast[ADC_YAW]-(s16)adc_orig.YAW)/RC_AD_ratio.RC_AD_ratio_f.YAW);
}

void RC_RCdat_pkg(u16 *pval)
{
	RC_Adc_to_RCdat(pval);
	RC_dat.MODE = RC_key_mod;
	if(RC_key_func==RC_KEY_FUNC_1)
		RC_dat.FUNC = RC_KEY_FUNC_1;
	else
		RC_dat.FUNC = RC_KEY_FUNC_NONE;
}

		
/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/
