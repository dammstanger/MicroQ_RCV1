
/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：main.c
 * 描	述	：TIM2 1通道 输入捕获 实现PPM信号解码
 *                    
 * 实验平台	：野火STM32开发板
 * 硬件连接	：
 * 版 	本	：
 * 从属关系	：
 * 库版本	：ST3.0.0
 * 创建时间	：2015.6.21
 * 最后编辑	：2015.6.21
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
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
/****************************宏定义***********************************************/

/****************************变量声明*********************************************/

/****************************变量定义*********************************************/

/****************************函数声明*********************************************/



/********************************************************************************
 * 函数名：Delay()
 * 描述  ：软件延时
 * 输入  ：-		    	
 * 返回  ：inttype
 * 调用  ：-
 ********************************************************************************/
void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
}


/********************************************************************************
 * 函数名：Delay()
 * 描述  ：软件延时
 * 输入  ：-		    	
 * 返回  ：inttype
 * 调用  ：-
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
 * 函数名：main()
 * 描述  ：主函数
 * 输入  ：-		    	
 * 返回  ：inttype
 * 调用  ：-
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
	SetSoftTimer(TIMER_1,20);
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
			SetSoftTimer(TIMER_1,20);
			printf("send data: %d,%d,%d,%d.\r\n",p_ADCval[0],p_ADCval[1],p_ADCval[2],p_ADCval[3]);
		}
		if(ReadSoftTimer(TIMER_2))
		{
			RC_RCdat_pkg(ADC_ConvertedValue);
			SetSoftTimer(TIMER_2,20);
		}
//		Delay(0x0003ffff);
	
//		Send_2401Dug_Pkg(0xff,p_ADCval[0],p_ADCval[1],p_ADCval[2],p_ADCval[3]);

	}

}

