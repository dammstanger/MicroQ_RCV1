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
/****************************宏定义***********************************************/
//通道映射
#define ADC_ROLL		2
#define ADC_PITCH		3
#define ADC_THRO		1
#define ADC_YAW			0

#define ADC_ROLL_MAX	40		



/****************************变量声明*********************************************/

/****************************变量定义*********************************************/
RC_DATA adc_orig ={2080,2030,3955,2012,0,0};			
RC_DATA_f ratio_adc_RC ={4.032f,4.095f,3.955f,4.095f,0.0f,0.0f};
RC_DATA RC_dat={1500,1500,1000,1500,1000,1000};
__IO u16 *p_ADCval=ADC_ConvertedValue;
/****************************函数声明*********************************************/


/********************************************************************************
 * 函数名：RC_Adc_Calib()
 * 描述  ：ADC通道中位值，油门最低值校准
 * 输入  ：-		    	
 * 返回  ：-
 * 调用  ：外部
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
 * 函数名：RC_Adc_to_RCdat()
 * 描述  ：ADC通道值到RC通道的转换，将0-4095 转换到1000-2000
 * 输入  ：-		    	
 * 返回  ：-
 * 调用  ：外部
 ********************************************************************************/
void RC_Adc_to_RCdat(u16 *pval)
{
	static u16 datlast[4] ={0};
	if(pval[ADC_ROLL]<4095)	datlast[ADC_ROLL] = pval[ADC_ROLL];			//去除异常值，保留上一次的值
	if(pval[ADC_PITCH]<4095) datlast[ADC_PITCH] = pval[ADC_PITCH];			
	if(pval[ADC_THRO]<4095)	datlast[ADC_THRO] = pval[ADC_THRO];			
	if(pval[ADC_YAW]<4095)	datlast[ADC_YAW] = pval[ADC_YAW];			
	
	RC_dat.ROLL= 1500 + (((s16)adc_orig.ROLL-(s16)datlast[ADC_ROLL])/ratio_adc_RC.ROLL);
	RC_dat.PITCH= 1500 + (((s16)adc_orig.PITCH-(s16)datlast[ADC_PITCH])/ratio_adc_RC.PITCH);
	RC_dat.THROTTLE= 1000 + (((s16)adc_orig.THROTTLE-(s16)datlast[ADC_THRO])/ratio_adc_RC.THROTTLE);
	RC_dat.YAW= 1500 + (((s16)datlast[ADC_YAW]-(s16)adc_orig.YAW)/ratio_adc_RC.YAW);
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
