#include "main.h"
vu32 left_encoder_count;
vu32 right_encoder_count;

void EXTIX_Init(void)
{
 
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //使能复用功能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOC,&GPIO_InitStructure);


  //GPIOC.3 中断线以及中断初始化配置  下降沿触发  left
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
  EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  
  //GPIOC.4 中断线以及中断初始化配置  下降沿触发  right
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
  EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;              //left
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;  //抢占优先级2， 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;         //子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 


  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;              //right
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;  //抢占优先级2 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;         //子优先
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

//left
void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3)!=RESET){
    left_encoder_count++;
    __Sensordata.wheel_left=left_encoder_count/30;
    EXTI_ClearITPendingBit(EXTI_Line3); //清除LINE3上的中断标志位  
  }
}
 
//right
void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4)!=RESET){
    right_encoder_count++;
    __Sensordata.wheel_right=right_encoder_count/30;
    EXTI_ClearITPendingBit(EXTI_Line4);   //清除LINE0上的中断标志位
  }
}

