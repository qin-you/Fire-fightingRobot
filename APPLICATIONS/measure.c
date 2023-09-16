#include "main.h"

//全局结构distance用于保存超声波探测到的距离，有三个成员。
Distance distance={0,0,0};

/**
  * @brief 测量左墙的距离
  * @param None
  * @retval None
  * @details trig-->PC.13  echo-->PA.2  TIM5_CH3
  *          毫米级测距，采用限幅滤波与卡尔曼滤波
  */
void Get_Distance_Left(void)
{
  u16 dis=0,temp=0;

  TIM5CH3_CAPTURE_STA=0;
  TIM_Cmd(TIM5,ENABLE);
  GPIO_SetBits(GPIOC,GPIO_Pin_13);
  delay_us(20);
  GPIO_ResetBits(GPIOC,GPIO_Pin_13);
  while((!(TIM5CH3_CAPTURE_STA&0X80))&&(SysTick->VAL>4500));
  if(TIM5CH3_CAPTURE_STA&0X80)
  {
    //LED_TOGGLE();
    temp = TIM5CH3_CAPTURE_VAL;             //得到总的高电平时间
    dis = (int)(temp * 170 / 1000+0.5);
    //limitfilter(&dis,distance.front);
    kalmanfilter_l(&dis);
    dis/=10;
    distance.left=dis;
  }
  else
  {
    TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Rising);       //设置为上升沿捕获
		distance.left=200;
  }
  TIM_Cmd(TIM5,DISABLE);                   //失能定时器3
  TIM5CH3_CAPTURE_STA=0;                   //开启下一次捕获
	__Sensordata.dis_left=distance.left;
}

/**
  * @brief 测量右墙的距离
  * @param None
  * @retval None
  * @details trig-->PC.12  echo-->PB.0  TIM3_CH3
  *          毫米级测距，采用限幅滤波与卡尔曼滤波
  */
void Get_Distance_Right(void)
{
  u16 dis=0,temp=0;

  TIM3CH3_CAPTURE_STA=0;
  TIM_Cmd(TIM3,ENABLE);
  GPIO_SetBits(GPIOC,GPIO_Pin_12);
  delay_us(20);
  GPIO_ResetBits(GPIOC,GPIO_Pin_12);
  while((!(TIM3CH3_CAPTURE_STA&0X80))&&(SysTick->VAL>4500));
  if(TIM3CH3_CAPTURE_STA&0X80)
  {
   // LED_TOGGLE();
    temp = TIM3CH3_CAPTURE_VAL;             //得到总的高电平时间
    dis = (int)(temp * 170 / 1000+0.5);
    //limitfilter(&dis,distance.front);
    kalmanfilter_r(&dis);
    dis/=10;
    distance.right=dis;
  }
  else
  {
		distance.right=200;
    TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising);       //设置为上升沿捕获
  }
  TIM_Cmd(TIM3,DISABLE);                   //失能定时器3
  TIM3CH3_CAPTURE_STA=0;                   //开启下一次捕获
	__Sensordata.dis_right=distance.right;
}

void Get_Distance_Front(void)
{
  u16 dis=0,temp=0;

  TIM2CH2_CAPTURE_STA=0;
  TIM_Cmd(TIM2,ENABLE);                     //使能定时器2
  GPIO_SetBits(GPIOC,GPIO_Pin_11);
  delay_us(20);
  GPIO_ResetBits(GPIOC,GPIO_Pin_11);
  while((!(TIM2CH2_CAPTURE_STA&0X80))&&(SysTick->VAL>4500));
  if(TIM2CH2_CAPTURE_STA&0X80)
  {
    LED_TOGGLE();
    temp = TIM2CH2_CAPTURE_VAL;             //得到总的高电平时间
    dis = (int)(temp * 170 / 1000+0.5);
    //limitfilter(&dis,distance.front);
    kalmanfilter_f(&dis);
    dis/=10;
    distance.front=dis;
  }
  else
  {
		distance.front=200;
    TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);       //设置为上升沿捕获
  }
  TIM_Cmd(TIM2,DISABLE);                   //失能定时器2
  TIM2CH2_CAPTURE_STA=0;                   //开启下一次捕获
	__Sensordata.dis_front=distance.front;
}

