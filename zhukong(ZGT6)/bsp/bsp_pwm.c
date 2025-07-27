#include "bsp_pwm.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;


  int yaw_flag ;
 int pitch_flag ;
void pwm_on(void)//下面电机接pe9,上面电机接pe11
{
		HAL_TIM_Base_Start(&htim1);
		
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,50);
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 90);
		
		int yaw_flag = 0;
        int pitch_flag = 0;
}

void pwm_yaw(void)
{   if(yaw_flag ==1)
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 250);
	else
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,50);
		
}	

void pwm_pitch(void)
{if(pitch_flag==1)
 __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 200);
	else
__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 90);
}	