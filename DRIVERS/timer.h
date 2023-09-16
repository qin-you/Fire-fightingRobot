#ifndef __MOTOR_PWM_H
#define __MOTOR_PWM_H
#include "main.h"

#define ARR_PWM 9999
#define PSC_PWM 143
#define ARR_WAVE 49999
#define PSC_WAVE 71

extern u8  TIM2CH2_CAPTURE_STA;            //保存捕获状态
extern u16 TIM2CH2_CAPTURE_VAL;            //保存捕获值
extern u8  TIM3CH3_CAPTURE_STA;            //保存捕获状态
extern u16 TIM3CH3_CAPTURE_VAL;            //保存捕获值
extern u8  TIM5CH3_CAPTURE_STA;            //保存捕获状态
extern u16 TIM5CH3_CAPTURE_VAL;            //保存捕获值

void PWM_Init(void);
void Wave_Init(void);

#endif
