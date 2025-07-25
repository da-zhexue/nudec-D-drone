#include "bsp_pwm.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

int turn = 0;

void pwm_on(void)
{
		HAL_TIM_Base_Start(&htim3);
		HAL_TIM_Base_Start(&htim4);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 1000);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 1000);
		
		turn = 0;
}

void pwm_turn(void)
{
		turn++;
		if(turn %2 == 0)
		{
				__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 1000);
		}
		else
		{
				__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 2000);
		}
}	
