#include "main.h"

/**
  * @brief �����ҵ��PWM��
  * @param speed����Χ��-100~+100
  * @retval None
  * details -100��ת����ٶȣ�0ͣת��+100��ת����ٶ�
  *         PWMռ�ձ���TIM4���õó�
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
  * @brief �����ҵ��PWM��
  * @param speed����Χ��-100~+100
  * @retval None
  * details -100��ת����ٶȣ�0ͣת��+100��ת����ٶ�
  *         PWMռ�ձ���TIM4���õó�
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

////////////////////����ʱ����ת90�Ⱥ���ת�꼴ͣ//////////////////////////////////
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
  while(right_encoder_count<12);				//0.7327cm/����encoder�ķֱ��� 25*0.7327=18.32cm
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

/*************ת45�ȣ�ת���ͣ�� �����𶯻�ʹencoder��������*********/
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

