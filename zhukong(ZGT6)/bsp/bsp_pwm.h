#ifndef BSP_PWM_H
#define BSP_PWM_H
#include "tim.h"


extern int yaw_flag ;
extern int pitch_flag ;

void pwm_on(void);
void pwm_yaw(void);
void pwm_pitch(void);

#endif
