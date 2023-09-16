#include "main.h"

/**
  * @brief LED��������
  * @param None
  * @retval None
  * @details LED-->PD.7
  */
void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;                  //����ṹ�����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //ʹ��PD�˿�ʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;              //PD.7 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;      //IO���ٶ�Ϊ10MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);                 //�����趨������ʼ��GPIOD
  GPIO_ResetBits(GPIOD,GPIO_Pin_7);                        //PD.2 �����,LED��

#if DRIVER_CHECK
printf("led init successful\r\n");
#endif
}

/**
  * @brief ����LED��
  * @param None
  * @retval None
  * @details LED-->PD.7
  */
void RED_ON(void)
{
  GPIO_SetBits(GPIOD,GPIO_Pin_7);
}

/**
  * @brief Ϩ��LED��
  * @param None
  * @retval None
  * @details LED-->PD.7
  */
void RED_OFF(void)
{
  GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}

/**
  * @brief �л�LED��״̬�������������
  * @param None
  * @retval None
  * @details LED-->PD.7
  */
void LED_TOGGLE(void)
{
  if(!GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_7))           //PD.7�����
  {
    GPIO_SetBits(GPIOD,GPIO_Pin_7);                       //�ø�PD.7��ƽ
  }
  else
  {
    GPIO_ResetBits(GPIOD,GPIO_Pin_7);                     //�õ�PD.7��ƽ
  }
}
