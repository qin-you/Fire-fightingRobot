#ifndef MAIN_H
#define MAIN_H

#define DRIVER_CHECK 0
#define TASKONCE_CHECK 0                                //如果只想检查一次，置为0
#define TASKDUPLICATE_CHECK 0
#define IRQONCE_CHECK 0                                 //如果只想检查一次，置为0
#define IRQDUPLICATE_CHECK 0


#include "stdio.h"
#include "stm32f10x.h"
#include "delay.h"
#include "encoder.h"
#include "led.h"
#include "timer.h"
#include "usart1.h" 
#include "dutytask.h"
#include "driver.h"
#include "pid.h"
#include "measure.h"
#include "system.h"
#include "datatransfer.h"
#include "motor.h"
#include "filter.h"
#include "parameters.h"
#include "adc.h"
#include "judge_fire.h"
#include "fire_fighting.h"
#include "search.h"

#endif
