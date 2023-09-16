#include "main.h"

/**
  * @brief 调节右电机PWM波
  * @param speed，范围从-100~+100
  * @retval None
  * details -100反转最大速度，0停转，+100正转最大速度
  *         PWM占空比由TIM4配置得出
  */
void MotorRight(int16_t speed)
{
  if(speed>100)  speed=100;
  if(speed<-100) speed=-100;
  __Motordata.motor_right=speed;
  speed=750-speed;
  TIM_SetCompare1(TIM4,speed);
}

/**
  * @brief 调节右电机PWM波
  * @param speed，范围从-100~+100
  * @retval None
  * details -100反转最大速度，0停转，+100正转最大速度
  *         PWM占空比由TIM4配置得出
  */
void MotorLeft(int16_t speed)
{
  if(speed>100)  speed=100;
  if(speed<-100) speed=-100;
  __Motordata.motor_left=speed;
  speed=750+speed;
  TIM_SetCompare2(TIM4,speed);
}

void FanMotor_Init()
{
  GPIO_InitTypeDef GPIO_InitStrue;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  GPIO_InitStrue.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitStrue.GPIO_Pin=GPIO_Pin_15;
  GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOD,&GPIO_InitStrue);
}

void FanMotor_Open()
{
  GPIO_SetBits(GPIOD,GPIO_Pin_15);
}

void FanMotor_Close()
{
  GPIO_ResetBits(GPIOD,GPIO_Pin_15);
}

////////////////////用延时左右转90度函数转完即停//////////////////////////////////
void turn_left_withdelay()
{
  MotorLeft(-50);
  MotorRight(50);
  delay_ms(465) ;
  MotorLeft(0)  ;
  MotorRight(0) ;
}

void turn_right_withdelay()
{
  MotorLeft(50)  ;
  MotorRight(-50);
  delay_ms(470)  ;
  MotorLeft(0)   ;
	MotorRight(0);
}
/////////////////////////////////////////////////////////////////////////////



void turn_left(void)
{
  left_encoder_count=0;
  right_encoder_count=0;
  MotorRight(50);
  MotorLeft(-50);
  while(right_encoder_count<12);				//0.7327cm/脉是encoder的分辨率 25*0.7327=18.32cm
  MotorLeft(0);
  MotorRight(0);
}

void turn_right(void)
{
  left_encoder_count=0;
  right_encoder_count=0;
  MotorRight(-50);
  MotorLeft(50);
  while(left_encoder_count<12);
  MotorLeft(0);
  MotorRight(0);
}

/*************转45度，转完就停车 风扇震动会使encoder产生脉冲*********/
void turn_left45()
{
	MotorLeft(0);
	MotorRight(0);
	
  MotorRight(20);
  MotorLeft(0);
  delay_ms(970);
  MotorRight(0);
}

void turn_left45_return()
{
  MotorLeft(0);
  MotorRight(-20);
  delay_ms(1020);
  MotorRight(0);
}


void turn_right45()
{
  MotorRight(0);
	MotorLeft(0);
	
  MotorLeft(20);
  delay_ms(920);
  
	MotorLeft(0);
}

void turn_right45_return()
{
  MotorLeft(-20);
  MotorRight(0);
  delay_ms(950);
  MotorLeft(0);
}

