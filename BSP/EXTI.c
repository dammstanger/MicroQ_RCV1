/******************** (C) COPYRIGHT 2015 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��EXTI.c
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
#include "EXTI.h"
#include "led.h"
#include "24l01.h"

/****************************�궨��***********************************************/

/****************************��������*********************************************/

/****************************��������*********************************************/

/****************************��������*********************************************/

///********************************************************************************
// * ��������EXTI_PC5_24L01()
// * ����  ����������24L01��IRQ�ⲿ�ж�
// * ����  ��-		    	
// * ����  ��-
// * ����  ���ⲿ
// ********************************************************************************/
//void EXTI_PC5_24L01_Init()
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ��IRQ���ڵ��ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0�� 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;			//�����ȼ�10
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure); 	

//}




/********************************************************************************
 * ��������EXTI_PA11_24L01_Init()
 * ����  ����������24L01��IRQ�ⲿ�ж�
 * ����  ��-		    	
 * ����  ��-
 * ����  ���ⲿ
 ********************************************************************************/
void EXTI_PA11_24L01_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);

	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);										//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;				//ʹ��IRQ���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			//��ռ���ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 	

}


//�ⲿ�ж�0������� 
void EXTI15_10_IRQHandler(void)
{	
	NRF24L01_IRQ_Handle();	
	EXTI_ClearITPendingBit(EXTI_Line11); //���LINE11�ϵ��жϱ�־λ  
}


		
/******************* (C) COPYRIGHT 2015 DammStanger *****END OF FILE************/
