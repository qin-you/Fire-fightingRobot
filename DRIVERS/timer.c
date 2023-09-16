#include "main.h"

u8  TIM2CH2_CAPTURE_STA;            //���沶��״̬
u16 TIM2CH2_CAPTURE_VAL;            //���沶��ֵ
u8  TIM3CH3_CAPTURE_STA;            //���沶��״̬
u16 TIM3CH3_CAPTURE_VAL;            //���沶��ֵ
u8  TIM5CH3_CAPTURE_STA;            //���沶��״̬
u16 TIM5CH3_CAPTURE_VAL;            //���沶��ֵ

/**
  * @brief ���Ƶ��̵����PWM������
  * @param None
  * @retval None
  * @details 1.�ҵ����PD.12��������PD.13(��PB.5,PB.6��ӳ��)
  *          2.ͨ��TIM4_CH1��TIM4_CH2���PWM��,PWM��Ƶ��50Hz
  *          3.ռ�ձ� speed/ARR_PWM���������� speed/10000
  *          4.ͨ��MotorRight(TIM4,u16 speed);�� MotorLeft(TIM4,u16 speed);
  */
void PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure;  

    //�˿���ӳ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    //���ø�����Ϊ�����������,���TIM4 CH1(PD.12�������)��TIM4 CH2(PD.13�������)��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;    //TIM4_CH1(PD.12�������)��TIM4_CH2(PD.13�������)  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
      
    TIM_TimeBaseStructure.TIM_Period = ARR_PWM;                 //��������һ�������¼�װ�����Զ���װ�ؼĴ���
    TIM_TimeBaseStructure.TIM_Prescaler =PSC_PWM;               //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ  
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;           //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1  
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; //�Ƚ����ʹ��  
    TIM_OCInitStructure.TIM_Pulse = 0;                          //���ô�װ�벶��ȽϼĴ���������ֵ  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //�������:TIM����Ƚϼ��Ը�  

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);                    //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx   
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);           //CH1Ԥװ��ʹ��
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);                    //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);           //CH2Ԥװ��ʹ��  

    TIM_ARRPreloadConfig(TIM4, ENABLE);                         //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���  
    TIM_CtrlPWMOutputs(TIM4,ENABLE);                            //MOE �����ʹ��
    TIM_Cmd(TIM4, ENABLE);

#if DRIVER_CHECK
printf("pwm init successful!\r\n");
#endif
}

/**
  * @brief ��������ʼ������
  * @param None
  * @retval None
  * @details ǰ������  trig-->PC.11 echo-->PA.1 TIM2_CH2
  *          ������  trig-->PC.13 echo-->PA.2 TIM5_CH3
  *          �ҳ�����  trig-->PC.12 echo-->PB.0 TIM3_CH3
  */

void Wave_Init(void){
  
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM2_ICInitStructure;
    TIM_ICInitTypeDef  TIM3_ICInitStructure;
    TIM_ICInitTypeDef  TIM5_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        //ʹ��TIM2ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        //ʹ��TIM3ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);        //ʹ��TIM5ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);

    /*----------------------------------Front---------------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                   //trig-->PC.11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_11);                           //trig������λ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                    //echo-->PA.1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*----------------------------------Left-----------------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                   //trig-->PC.13
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);                           //trig ������λ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                    //echo-->PA.2 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*----------------------------------Right-----------------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                   //trig-->PC.12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_12);                           //trig ������λ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                    //echo-->PB.0 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*------------------------------TIM2--------------------------------------*/
    //��ʼ��TIM2ʱ��
    TIM_TimeBaseStructure.TIM_Period = ARR_WAVE;                 //�Զ���װֵ 
    TIM_TimeBaseStructure.TIM_Prescaler =PSC_WAVE;               //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);


    //��ʼ��TIM2���벶�����
    TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2;            //ѡ������� IC2ӳ�䵽TI2��
    TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //�����ز���
    TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI2��
    TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
    TIM2_ICInitStructure.TIM_ICFilter = 0x00;                    //���������˲��� ���˲�
    TIM_ICInit(TIM2, &TIM2_ICInitStructure);


    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;              //TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM2,TIM_IT_CC2|TIM_IT_Update);        //����жϱ�־λ
    TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);                        //�������ж�


    /*------------------------------TIM3--------------------------------------*/
    //��ʼ����ʱ��3 TIM3ʱ��
    TIM_TimeBaseStructure.TIM_Period = ARR_WAVE;                 //�Զ���װֵ 
    TIM_TimeBaseStructure.TIM_Prescaler =PSC_WAVE;               //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    //��ʼ��TIM3���벶�����
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;            //ѡ�������IC3ӳ�䵽TI3��
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //�����ز���
    TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ֱ��ӳ��
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
    TIM3_ICInitStructure.TIM_ICFilter = 0x00;                    //���������˲��������˲������˲�
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);


    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;              //TIM5�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_Update);
    TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);                        //�������ж�


    /*------------------------------TIM5--------------------------------------*/
    
    //��ʼ����ʱ��5 TIM5ʱ��
    TIM_TimeBaseStructure.TIM_Period = ARR_WAVE;                 //�Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler =PSC_WAVE;               //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    //��ʼ��TIM3���벶�����
    TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3;            //CC2S=01 ѡ������� IC3ӳ�䵽TI3��
    TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //�����ز���
    TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI3��
    TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
    TIM5_ICInitStructure.TIM_ICFilter = 0x00;                    //���������˲��������˲�
    TIM_ICInit(TIM5, &TIM5_ICInitStructure);


    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;              //TIM5�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC3|TIM_IT_Update);
    TIM_ITConfig(TIM5,TIM_IT_CC3,ENABLE);                        //�������ж�

}

void TIM2_IRQHandler(void)
{

  if(!(TIM2CH2_CAPTURE_STA&0X80))                        //��δ�ɹ�����
  {
    if(TIM_GetITStatus(TIM2,TIM_IT_CC2) != RESET)
    {
      if(TIM2CH2_CAPTURE_STA&0X40)                       //֮ǰ�Ѿ�����������
      {
        TIM2CH2_CAPTURE_STA|=0X80;                       //��ǳɹ�����һ����������
        TIM2CH2_CAPTURE_VAL=TIM2->CCR2;
        TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);   //����Ϊ�����ز���
      }
      else
      {
        TIM2CH2_CAPTURE_STA=0;
        TIM2CH2_CAPTURE_VAL=0;
        TIM2->CNT=0;
        TIM2CH2_CAPTURE_STA|=0X40;                       //��ǲ�����������
        TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);
      }
    }
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);                     //����жϱ�־λ
}

void TIM3_IRQHandler(void)
{
  if(!(TIM3CH3_CAPTURE_STA&0X80))                        //��δ�ɹ�����
  {
    if(TIM_GetITStatus(TIM3,TIM_IT_CC3) != RESET)
    {
      if(TIM3CH3_CAPTURE_STA&0X40)                       //֮ǰ�Ѿ�����������
      {
        TIM3CH3_CAPTURE_STA|=0X80;                       //��ǳɹ�����һ����������
        TIM3CH3_CAPTURE_VAL=TIM3->CCR3;
        TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising);   //����Ϊ�����ز���
      }
      else
      {
        TIM3CH3_CAPTURE_STA=0;
        TIM3CH3_CAPTURE_VAL=0;
        TIM3->CNT=0;
        TIM3CH3_CAPTURE_STA|=0X40;                       //��ǲ�����������
        TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);
      }
    }
  }
  TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);                     //����жϱ�־λ
}

void TIM5_IRQHandler(void)
{
  if(!(TIM5CH3_CAPTURE_STA&0X80))                        //��δ�ɹ�����
  {
    if(TIM_GetITStatus(TIM5,TIM_IT_CC3) != RESET)
    {
      if(TIM5CH3_CAPTURE_STA&0X40)                       //֮ǰ�Ѿ�����������
      {
        TIM5CH3_CAPTURE_STA|=0X80;                       //��ǳɹ�����һ����������
        TIM5CH3_CAPTURE_VAL=TIM5->CCR3;
        TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Rising);   //����Ϊ�����ز���
      }
      else
      {
        TIM5CH3_CAPTURE_STA=0;
        TIM5CH3_CAPTURE_VAL=0;
        TIM5->CNT=0;
        TIM5CH3_CAPTURE_STA|=0X40;                       //��ǲ�����������
        TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Falling);
      }
    }
  }
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);                     //����жϱ�־λ
}
