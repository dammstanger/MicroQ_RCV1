
/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��main.c
 * ��	��	��TIM2 1ͨ�� ���벶�� ʵ��PPM�źŽ���
 *                    
 * ʵ��ƽ̨	��Ұ��STM32������
 * Ӳ������	��
 * �� 	��	��
 * ������ϵ	��
 * ��汾	��ST3.0.0
 * ����ʱ��	��2015.6.21
 * ���༭	��2015.6.21
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "stm32f10x.h"
#include "usart.h"
#include "SysTick.h"
#include "led.h"
#include "24l01.h"
#include "EXTI.h"
#include "ComProtocol.h"
#include "adc.h"
#include "key.h"
#include "Timer.h"
#include "flash.h"
/****************************�궨��***********************************************/
#define LED_NORMAL			0
#define LED_NOCALIB			6
#define LED_RDYCALIB		7
#define LED_CALIB_STP1		1
#define LED_CALIB_STP2		2
#define LED_CALIB_ERR		4
#define LED_DIV_PAIR		5

#define KEY_NONE			0
#define KEY_CALIB_AREA		1			//�������ڵĹ��ܵ�ʱ������
#define KEY_PAIR_AREA		2
/****************************��������*********************************************/

/****************************��������*********************************************/
typedef enum {
	NORMAL=0,
	NEED_CALIB,
	CALIB_RDY,
	CALIB_STP1,
	CALIB_STP2,
	DIV_PAIR
}SYSMODE;

SYSMODE g_sysmode=NORMAL;		

u8 g_ledmode = LED_NORMAL;
u8 g_keytime = KEY_NONE;
/****************************��������*********************************************/
void Key_Scan(void);
void LED_Task(u8 mode);
void Sys_State_Machine(void);
/********************************************************************************
 * ��������Delay()
 * ����  �������ʱ
 * ����  ��-		    	
 * ����  ��inttype
 * ����  ��-
 ********************************************************************************/
void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
}


/********************************************************************************
 * ��������Delay()
 * ����  �������ʱ
 * ����  ��-		    	
 * ����  ��inttype
 * ����  ��-
 ********************************************************************************/
void BSP_Init()
{
	KEY_GPIO_Config();
	LED_GPIO_Config();
	LED1(ON);
	LED2(ON);
	LED3(ON);
	LED4(ON);
	Delay(0x0055ffff);
	LED1(OFF);
	LED2(OFF);
	LED3(OFF);
	LED4(OFF);
	Delay(0x0055ffff);
	USART1_Configuration();
	printf("system start...\r\n");
	ADC1_Init();
	NRF24L01_Init(MODEL_TX, 40);		//�ϵ�ȴ�100ms
	while(NRF24L01_Check()==ERROR)
	{
		Delay(0x0015ffff);
		printf("24L01check fail...\r\n");
	}
	Delay(0x0035ffff);
}

/********************************************************************************
 * ��������main()
 * ����  ��������
 * ����  ��-		    	
 * ����  ��inttype
 * ����  ��-
 ********************************************************************************/
int main(void)
{
	/* config the sysclock to 72M */    

	SystemInit();
	BSP_Init();
	RC_Para_LoadandCheck();

	SysTick_Init();
	
	SetSoftTimer(TIMER_1,20);			//��λms
	while(1)
	{	
		//======================����ɨ��==============================
		Key_Scan();
		//======================����==================================		
		if(ReadSoftTimer(TIMER_1))
		{
			//��ʱ����װ
			SetSoftTimer(TIMER_1,20);
			//ADC���ݴ��
			RC_RCdat_pkg(ADC_ConvertedValue);
			//RCУ׼
			if(RC_Calibr_sta==RC_CALIB_ORIN||RC_Calibr_sta==RC_CALIB_RANGE)
			{
				RC_Adc_Calib(ADC_ConvertedValue);
			}
			
			Send_2401RC_Pkg(RC_dat);
			
//			printf("send data: %d,%d,%d,%d.\r\n",RC_dat.ROLL,RC_dat.PITCH,RC_dat.THROTTLE,RC_dat.YAW);
//			printf("send data: %f,%f,%f,%f.\r\n",RC_AD_ratio.RC_AD_ratio_f.ROLL,RC_AD_ratio.RC_AD_ratio_f.PITCH,RC_AD_ratio.RC_AD_ratio_f.THROTTLE,RC_AD_ratio.RC_AD_ratio_f.YAW);
		}
		//״̬��
		Sys_State_Machine();
		//LED��ʾ����
		LED_Task(g_ledmode);

//		Delay(0x0003ffff);
	}
}


void Key_Scan()
{
	u8 k_mode,k_func;
	static u8 k_mode_last = 0;
	static u8 cnt_mode = 0;
	static u16 cnt_func = 0;

	if(ReadSoftTimer(TIMER_3))			//
	{
		//��װ��ʱ��
		SetSoftTimer(TIMER_3,10);		//ɨ������10ms����������
		//������������
		k_mode = KEY_MODE_Val;
		k_func = KEY_FUNC_Val;
		//����
		if(k_mode!=k_mode_last)			//k_mode�ǿ��أ��Կ��صĴ���
		{
			if(cnt_mode==0)				//��һ�ν���������
			{
				cnt_mode = 1;
			}
			else{						//�ڶ��ν�����ȡֵ
				cnt_mode = 0;
				if(!k_mode)
					RC_key_mod = RC_KEY_MODE_ON;
				else RC_key_mod = RC_KEY_MODE_OFF;
				k_mode_last = k_mode;
			}
		}
		//�������¼�ʱ
		if(!k_func)						//k_func�ǰ���������Ϊ0���Կ��صĴ���
		{
			cnt_func++;
		}
		else{
			//����ʱ������
			if(cnt_func>2&&cnt_func<200)					//С��3sΪ��ͨ����ģʽ
			{
				RC_key_func = RC_KEY_FUNC_1;
			}
			else if(cnt_func>=200&&cnt_func<400)			//3-6sΪУ׼ģʽ
			{
				RC_key_func = RC_KEY_FUNC_CALI;
				RC_Calibr_sta = RC_CALIB_ORIN;
//				printf("key cmd start calib...\r\n");
			}
			else if(cnt_func>=400)
			{
				RC_key_func = RC_KEY_FUNC_DIVPAIR;			
			}
			else 
				RC_key_func = RC_KEY_FUNC_NONE;
			cnt_func = 0;
			g_keytime = KEY_NONE;							//�ɿ�ʱ���
		}
		//����ָʾģʽ
		if(cnt_func>=200&&cnt_func<400)						//3-6sΪУ׼ģʽ
		{
			g_keytime = KEY_CALIB_AREA;
//			printf("key in calib area...\r\n");
		}
		else if(cnt_func>=400)
		{
			g_keytime = KEY_PAIR_AREA;
		}
		else g_keytime = KEY_NONE;
	}
}

void Sys_State_Machine()
{

	if(g_keytime==KEY_CALIB_AREA)
	{
		g_sysmode = CALIB_RDY;
		g_ledmode = LED_RDYCALIB;	
	}
	else if(RC_Calibr_sta==RC_CALIB_ORIN)
	{
		g_sysmode = CALIB_STP1;
		g_ledmode = LED_CALIB_STP1;	
	}
	else if(RC_Calibr_sta==RC_CALIB_RANGE)
	{
		g_sysmode = CALIB_STP2;
		g_ledmode = LED_CALIB_STP2;	
	}
	else if(g_sysmode==CALIB_STP2&&RC_Calibr_sta==RC_CALIB_DONE)
	{
		g_sysmode = NORMAL;
		g_ledmode = LED_NORMAL;	
	}
	else if(RC_Calibr_sta==RC_NEED_CALIB)
	{
		g_sysmode = NEED_CALIB;
		g_ledmode = LED_NOCALIB;	
	}
	else
	{
		g_sysmode = NORMAL;
		g_ledmode = LED_NORMAL;	
	}
	
}


void LED_Task(u8 mode)
{
	static u8 cnt = 0;
	
	switch(mode)
	{
		case LED_NOCALIB :{
			if(ReadSoftTimer(TIMER_4))	
			{
				SetSoftTimer(TIMER_4,300);
				if(cnt)
				{
					cnt = 0;
					LED1(ON);
					LED2(ON);
					LED3(ON);
					LED4(ON);
				}
				else{
					cnt = 1;
					LED1(OFF);
					LED2(OFF);
					LED3(OFF);
					LED4(OFF);			
				}
//				printf("mode:need calib...\r\n");
			}
		}break;
		case LED_RDYCALIB :{
			if(ReadSoftTimer(TIMER_4))	
			{
				SetSoftTimer(TIMER_4,50);
				switch(cnt)
				{
					case 0:LED1(ON);LED2(OFF);LED3(OFF);LED4(OFF);cnt++;break;
					case 1:LED1(OFF);LED2(ON);LED3(OFF);LED4(OFF);cnt++;break;
					case 2:LED1(OFF);LED2(OFF);LED3(ON);LED4(OFF);cnt++;break;
					default:LED1(OFF);LED2(OFF);LED3(OFF);LED4(ON);cnt=0;break;	
				}	
//				printf("mode:ready to calib...\r\n");				
			}
		}break;
		case LED_CALIB_STP1 :{
			if(ReadSoftTimer(TIMER_4))	
			{
				SetSoftTimer(TIMER_4,10);
				LED1(ON);
				LED2(ON);
				LED3(OFF);
				LED4(OFF);	
//				printf("mode:calib step1...\r\n");				
			}
		}break;
		case LED_CALIB_STP2 :{
			if(ReadSoftTimer(TIMER_4))	
			{
				SetSoftTimer(TIMER_4,100);
				LED1(ON);
				LED2(ON);
				if(cnt)
				{
					cnt = 0;
					LED3(OFF);
					LED4(ON);
				}
				else{
					cnt = 1;
					LED3(ON);
					LED4(OFF);			
				}
//				printf("mode:calib step2...\r\n");				
			}
		}break;
		case LED_CALIB_ERR :{}break;
		case LED_DIV_PAIR :{}break;
		default:{	
			if(ReadSoftTimer(TIMER_4))	
			{
				SetSoftTimer(TIMER_4,10);
				LED3(OFF);
				LED4(OFF);	
				cnt++;
				if(cnt>=3)
				{
					if(cnt==4)cnt = 0;
					LED1(ON);
					LED2(ON);
				}
				else{
					LED1(OFF);
					LED2(OFF);
				}
			}
		}break;
	}
}

