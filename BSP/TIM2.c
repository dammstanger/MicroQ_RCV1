
#include "TIM2.h"
/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_PWMIN_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*TIM_Period--0xffff   TIM_Prescaler--71 -->中断周期为1us*/
void TIM2_PWMIN_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM2_ICInitStructure;
	
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=0xffff;		 				/* 自动重装载寄存器周期的值(计数值) 10 */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* 时钟预分频数 72M/72=1us*/
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* 采样分频 不分频*/
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//----------------------------TIM2-ch1--------------------------------------
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//下降沿捕获
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM2_ICInitStructure.TIM_ICFilter = 0;					//不滤波
	TIM_ICInit(TIM2,&TIM2_ICInitStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_FLAG_CC1);				/* 清除溢出中断标志 */
    
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);

}

//TIM2 负责RC信号输入捕捉
void TIM2_ISR(void)
{	
	u8 event = 0;											//中断事件变量
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)			//定时器溢出
		event = 1;
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1)!=RESET) 			//捕获到电压沿开始
		event = 2;
	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_FLAG_CC1); 		//标志位必须有软件清除	
}


