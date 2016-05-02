
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
/****************************�궨��***********************************************/

/****************************��������*********************************************/

/****************************��������*********************************************/

/****************************��������*********************************************/



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
	NRF24L01_Init(MODEL_TX, 40);
	while(NRF24L01_Check()==ERROR)
	{
		Delay(0x0015ffff);
		printf("24L01check fail...\r\n");
	}

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
	u8 flg=0;	
	u8 sta;

	SystemInit();
	BSP_Init();
	SysTick_Init();

	sta = RCC_GetSYSCLKSource();
	if(sta==0x08)
		printf("used PLL as system clk.\r\n");
	SetSoftTimer(TIMER_1,50);
	SetSoftTimer(TIMER_2,20);
	while(1)
	{	
		if(ReadSoftTimer(TIMER_1))
		{
			if(flg)
			{
				flg = 0;
				LED1(ON);
			}
			else
			{
				flg = 1;
				LED1(OFF);
			}
			Send_2401RC_Pkg(RC_dat);
			SetSoftTimer(TIMER_1,50);
		}
		if(ReadSoftTimer(TIMER_2))
		{
			RC_RCdat_pkg(ADC_ConvertedValue);
			SetSoftTimer(TIMER_2,20);
		}
//		Delay(0x0003ffff);
//		printf("send data: %d,%d.\r\n",123,456);
//		Send_2401Dug_Pkg(0xff,p_ADCval[0],p_ADCval[1],p_ADCval[2],p_ADCval[3]);

	}

}

