#include "main.h"

/**
  * @brief LED驱动函数
  * @param None
  * @retval None
  * @details LED-->PD.7
  */
void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;                  //定义结构体变量
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //使能PD端口时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;              //PD.7 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;      //IO口速度为10MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);                 //根据设定参数初始化GPIOD
  GPIO_ResetBits(GPIOD,GPIO_Pin_7);                        //PD.2 输出高,LED亮

#if DRIVER_CHECK
printf("led init successful\r\n");
#endif
}

/**
  * @brief 点亮LED灯
  * @param None
  * @retval None
  * @details LED-->PD.7
  */
void RED_ON(void)
{
  GPIO_SetBits(GPIOD,GPIO_Pin_7);
}

/**
  * @brief 熄灭LED灯
  * @param None
  * @retval None
  * @details LED-->PD.7
  */
void RED_OFF(void)
{
  GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}

/**
  * @brief 切换LED灯状态，亮变灭，灭变亮
  * @param None
  * @retval None
  * @details LED-->PD.7
  */
void LED_TOGGLE(void)
{
  if(!GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_7))           //PD.7输出低
  {
    GPIO_SetBits(GPIOD,GPIO_Pin_7);                       //置高PD.7电平
  }
  else
  {
    GPIO_ResetBits(GPIOD,GPIO_Pin_7);                     //置低PD.7电平
  }
}
