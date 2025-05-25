#include "timer.h"
/****************TIM函数封装************************
**形参：TIM_TypeDef *TIMx -- 那个定时器
**			u16 psc -- 预分频系数（0~65535）
**			u16 arr -- 重装载值（0~16）
****************************************************/
uint16_t time2_usart1 = 0;


void TIMx_Init(TIM_TypeDef *TIMx,u16 psc,u16 arr)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	if(TIM2 == TIMx)
	{
		/*1.开时钟*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能	
		#ifdef TIM2_IRQ
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断
		STM32_NVIC_SetPriority(TIM2_IRQn,1,2);//设置优先级
		#endif
	}
	if(TIM3 == TIMx)
	{
		/*1.开时钟*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能	
		#ifdef TIM3_IRQ
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
		STM32_NVIC_SetPriority(TIM3_IRQn,2,2);//设置优先级
		#endif
	}
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	TIMx->CR1|=1<<0;//开启定时器	
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		TIM_Cmd(TIM2, DISABLE);//关闭定时器2
		USART1_rx.usart1_flag=1;//串口接收标志位置1
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		if(ERROR_FLAG <= 3000)
			ERROR_FLAG++;
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
}
