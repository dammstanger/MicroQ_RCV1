/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：RCencode.c
 * 描	述	：外部中断初始化和中断服务
 *                    
 * 实验平台	：FCU V1.0
 * 硬件连接	：
 * 版 	本	：V1.0.150412
 * 从属关系	：FCU_UCOSII150602_UCOSII2.91
 * 库版本	：ST3.0.0
 * 创建时间	：2014.4.11
 * 最后编辑	：2015.4.12
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/



/****************************包含头文件*******************************************/
#include "RCencode.h"
#include "adc.h"
#include "led.h"
#include "key.h"
#include "Systick.h"
#include "flash.h"
/****************************宏定义***********************************************/
//通道映射
#define ADC_ROLL		2
#define ADC_PITCH		3
#define ADC_THRO		1
#define ADC_YAW			0

#define ADC_ROLL_MAX	40		


/****************************变量声明*********************************************/
// = {4.032f,4.095f,3.955f,4.095f,0.0f,0.0f};
/****************************变量定义*********************************************/
RC_DATA adc_orig ={2080,2030,3955,2012,0,0};			
//RC_DATA_f ratio_adc_RC ={4.032f,4.095f,3.955f,4.095f,0.0f,0.0f};
RC_DATA RC_dat={1500,1500,1000,1500,1000,1000};

RC_AD_RATIO RC_AD_ratio = {4.032f,4.095f,3.955f,4.095f,0.0f,0.0f};

u8 RC_Calibr_sta = 0;					//RC摇杆校准状态
u16 RC_key_mod = RC_KEY_MODE_OFF;
u16 RC_key_func= RC_KEY_FUNC_NONE;
__IO u16 *p_ADCval=ADC_ConvertedValue;
/****************************函数声明*********************************************/
void RC_Para_Save(RC_AD_RATIO ratio,RC_DATA oringe);


#define ADDCNT		100
/********************************************************************************
 * 函数名：RC_Adc_Calib()
 * 描述  ：ADC通道中位值，油门最低值校准
 * 输入  ：-		    	
 * 返回  ：-完成返回1，否则返回0
 * 调用  ：外部
 ********************************************************************************/
u8 RC_Adc_Calib(u16 *pval)
{
	static u16 cnt = 0;
	static u32 add[4] = {0};
	static u16 Max[4] = {0};				//用于记录出现的最大值
	static u16 Min[4] = {4095};				//用于记录出现的最小值

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
				cnt = 0;															//以备下次校准
				RC_AD_ratio.RC_AD_ratio_f.ROLL = (Max[0] - Min[0])/1000.0;			//计算比例系数
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
 * 函数名：RC_Para_Save()
 * 描述  ：RC参数存入Flash
 * 输入  ：-		    	
 * 返回  ：-
 * 调用  ：内部
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
 * 函数名：RC_Para_LoadandCheck()
 * 描述  ：加载Flash中的RC参数
 * 输入  ：-		    	
 * 返回  ：
 * 调用  ：外部
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
		RC_Calibr_sta = RC_NEED_CALIB;			//摇杆未校准
	}
}
	


/********************************************************************************
 * 函数名：RC_Adc_to_RCdat()
 * 描述  ：ADC通道值到RC通道的转换，将0-4095 转换到1000-2000
 * 输入  ：-		    	
 * 返回  ：-
 * 调用  ：外部
 ********************************************************************************/
void RC_Adc_to_RCdat(u16 *pval)
{
	static u16 datlast[4] ={0};
	if(pval[ADC_ROLL]<=4095)	datlast[ADC_ROLL] = pval[ADC_ROLL];			//去除异常值，保留上一次的值
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
