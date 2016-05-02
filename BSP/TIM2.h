
#ifndef __TIMER_PWMIN_H_
#define	__TIMER_PWMIN_H_

#include "stm32f10x.h"
#include 	"misc.h"
#include	"stm32f10x_tim.h"


void TIM2_NVIC_Configuration(void);
void TIM2_PWMIN_Configuration(void);
void TIM2_PWMIN_GPIOInit(void);
void TIM2_ISR(void);

#endif

