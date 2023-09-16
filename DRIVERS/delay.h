#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void NVIC_SETPRIMASK(void);
void NVIC_RESETPRIMASK(void);
void SysTick_Init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif
