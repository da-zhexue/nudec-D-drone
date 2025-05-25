#ifndef _TIMER_H
#define _TIMER_H
#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"
#include <stdio.h>

void TIMx_Init(TIM_TypeDef *TIMx,u16 psc,u16 arr);
extern uint16_t time2_usart1;

#endif
