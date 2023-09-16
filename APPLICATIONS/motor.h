#ifndef MOTOR_H
#define MOTOR_H
#include "main.h"

void MotorRight(int16_t speed);
void MotorLeft(int16_t speed);
void FanMotor_Init(void);
void FanMotor_Open(void);
void FanMotor_Close(void);

void turn_left45(void);           //use delay_ms()
void turn_left45_return(void);
void turn_right45(void);
void turn_right45_return(void);

void turn_left(void);
void turn_right(void);            //use encoder

void turn_right_withdelay(void);  //use delay_ms()
void turn_left_withdelay(void);
#endif

