/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��Timer.c
 * ��	��	��STC 60S2 ��ʱ����ʱ����ʱ�亯��	11.0592MHz
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0	
 * Ӳ������	��
 * �� 	��	��V0.1.160217
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.2.17
 * ���༭	��2016.2.17
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "Timer.h"
/****************************�궨��***********************************************/

/****************************��������***********************************************/
u8 Timer_reg=0;
u32 Timer1_cnt = 0;			//��С��λ1ms,���ʱ2^32��
u32 Timer2_cnt = 0;
u32 Timer3_cnt = 0;
u32 Timer4_cnt = 0;

/********************************************************************************
 * ��������ReadSoftTimer
 * ����  ����ȡ�����ʱ���Ĵ���
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
u8 ReadSoftTimer(u8 timerx)
{
	switch(timerx)
	{
		case TIMER_1	:{return (Timer_reg&TIMEUP_1);}
		case TIMER_2	:{return (Timer_reg&TIMEUP_2);}
		case TIMER_3	:{return (Timer_reg&TIMEUP_3);}	
		case TIMER_4	:{return (Timer_reg&TIMEUP_4);}	
		default	:break;					
	}
	return 0;
}

/********************************************************************************
 * ��������
 * ����  ��
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void SetSoftTimer(u8 timerx,long val)
{
	switch(timerx)
	{
		case TIMER_1	:{
			Timer_reg &=~0x01;Timer1_cnt = val;		//��ʱ������㣬��ʱ����ֵ
			}break;
		case TIMER_2	:{
			Timer_reg &=~0x02;Timer2_cnt = val;			
			}break;
		case TIMER_3	:{
			Timer_reg &=~0x04;Timer3_cnt = val;			
			}break;
		case TIMER_4	:{
			Timer_reg &=~0x08;Timer4_cnt = val;			
			}break;
		default	:break;			
	}
}

void SoftTimer()
{
	if(Timer1_cnt)	Timer1_cnt--;
	else 		Timer_reg |= 0x01;
	if(Timer2_cnt)	Timer2_cnt--;
	else 		Timer_reg |= 0x02;	
	if(Timer3_cnt)	Timer3_cnt--;
	else 		Timer_reg |= 0x04;
	if(Timer4_cnt)	Timer4_cnt--;
	else 		Timer_reg |= 0x08;
}
	
/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

