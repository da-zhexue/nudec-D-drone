#include "bsp_pwm.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;

int yaw_flag = 0 ;
int pitch_flag = 0;
void pwm_on(void)//pitch电机接pe9,yaw电机接pe11
{
		HAL_TIM_Base_Start(&htim1);
		
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,50);
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 90);
		
		yaw_flag = 0;
    pitch_flag = 0;
}

void pwm_yaw(void)
{   
		yaw_flag++;
		if(yaw_flag%2 == 1)
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 245);
		else
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,53);
		
}	

void pwm_pitch(void)
{
		pitch_flag++;
		if(pitch_flag%2 == 1)
				__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 190);
		else
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 85);
}	
