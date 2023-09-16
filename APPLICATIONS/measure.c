#include "main.h"

//ȫ�ֽṹdistance���ڱ��泬����̽�⵽�ľ��룬��������Ա��
Distance distance={0,0,0};

/**
  * @brief ������ǽ�ľ���
  * @param None
  * @retval None
  * @details trig-->PC.13  echo-->PA.2  TIM5_CH3
  *          ���׼���࣬�����޷��˲��뿨�����˲�
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
    temp = TIM5CH3_CAPTURE_VAL;             //�õ��ܵĸߵ�ƽʱ��
    dis = (int)(temp * 170 / 1000+0.5);
    //limitfilter(&dis,distance.front);
    kalmanfilter_l(&dis);
    dis/=10;
    distance.left=dis;
  }
  else
  {
    TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Rising);       //����Ϊ�����ز���
		distance.left=200;
  }
  TIM_Cmd(TIM5,DISABLE);                   //ʧ�ܶ�ʱ��3
  TIM5CH3_CAPTURE_STA=0;                   //������һ�β���
	__Sensordata.dis_left=distance.left;
}

/**
  * @brief ������ǽ�ľ���
  * @param None
  * @retval None
  * @details trig-->PC.12  echo-->PB.0  TIM3_CH3
  *          ���׼���࣬�����޷��˲��뿨�����˲�
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
    temp = TIM3CH3_CAPTURE_VAL;             //�õ��ܵĸߵ�ƽʱ��
    dis = (int)(temp * 170 / 1000+0.5);
    //limitfilter(&dis,distance.front);
    kalmanfilter_r(&dis);
    dis/=10;
    distance.right=dis;
  }
  else
  {
		distance.right=200;
    TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising);       //����Ϊ�����ز���
  }
  TIM_Cmd(TIM3,DISABLE);                   //ʧ�ܶ�ʱ��3
  TIM3CH3_CAPTURE_STA=0;                   //������һ�β���
	__Sensordata.dis_right=distance.right;
}

void Get_Distance_Front(void)
{
  u16 dis=0,temp=0;

  TIM2CH2_CAPTURE_STA=0;
  TIM_Cmd(TIM2,ENABLE);                     //ʹ�ܶ�ʱ��2
  GPIO_SetBits(GPIOC,GPIO_Pin_11);
  delay_us(20);
  GPIO_ResetBits(GPIOC,GPIO_Pin_11);
  while((!(TIM2CH2_CAPTURE_STA&0X80))&&(SysTick->VAL>4500));
  if(TIM2CH2_CAPTURE_STA&0X80)
  {
    LED_TOGGLE();
    temp = TIM2CH2_CAPTURE_VAL;             //�õ��ܵĸߵ�ƽʱ��
    dis = (int)(temp * 170 / 1000+0.5);
    //limitfilter(&dis,distance.front);
    kalmanfilter_f(&dis);
    dis/=10;
    distance.front=dis;
  }
  else
  {
		distance.front=200;
    TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);       //����Ϊ�����ز���
  }
  TIM_Cmd(TIM2,DISABLE);                   //ʧ�ܶ�ʱ��2
  TIM2CH2_CAPTURE_STA=0;                   //������һ�β���
	__Sensordata.dis_front=distance.front;
}

