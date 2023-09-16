#include "main.h"

/*--------------------------USART1全局变量-----------------------------------*/
u8 USART1_RXBUFF[USART1_DMA_RX_LEN];    //USART1 DMA接收缓存区
u8 USART1_TXBUFF[USART1_DMA_TX_LEN];    //USART1 DMA发送缓存区
/*--------------------------------------------------------------------------*/


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
  int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
  x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
  while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
  USART1->DR = (u8) ch;      
  return ch;
}
#endif 


/**
  * @brief 串口1初始化
  * @param BaudRate
  * @retval None
  * @details BaudRate 115200
  *          TX PA.9   RX PA.10
  *          USART1_RX-->DMA1_CH5    USART1_TX-->DMA1_CH4
  *          字长8位    一位停止位    无奇偶校验位
  *          中断先、响优先级：1、1
  *          使能DMA接收,使能IDLE中断，接收完一帧数据后的一个字节空闲后进入中断
  *          使能DMA发送
  */
void Usart1_Init(u32 baudrate)
{
  GPIO_InitTypeDef GPIO_InitStrue;
  USART_InitTypeDef USART_InitStrue;
  NVIC_InitTypeDef NVIC_InitStrue;
  DMA_InitTypeDef DMA_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
  
  GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_InitStrue.GPIO_Pin=GPIO_Pin_9;
  GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
  
  GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;
  GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
  
  USART_InitStrue.USART_BaudRate=baudrate;
  USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
  USART_InitStrue.USART_Parity=USART_Parity_No;
  USART_InitStrue.USART_StopBits=USART_StopBits_1;
  USART_InitStrue.USART_WordLength=USART_WordLength_8b;
  
  USART_Init(USART1,&USART_InitStrue);
  USART_Cmd(USART1,ENABLE);//使能串口1
  
  USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//开启接收中断
  
  NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;
  NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
  NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
  NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
  NVIC_Init(&NVIC_InitStrue);
  
  DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;  //DMA外设ADC基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&USART1_RXBUFF;  //DMA内存基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从内存读取发送到外设
  DMA_InitStructure.DMA_BufferSize = USART1_DMA_RX_LEN;  //DMA通道的DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
  DMA_Cmd(DMA1_Channel5, ENABLE);

  DMA_DeInit(DMA1_Channel4);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&USART1_TXBUFF;  //DMA内存基地址 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  
  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
  
#if DRIVER_CHECK
  printf("Usart1 init successful!\r\n");
#endif
}

/**
  * @brief 串口1的DMA发送函数，发送一组数据
  * @param DataToSend 要发送数据的数组的指针
  * @param data_num 要发送的数据的个数
  * @retval None
  */
void Usart1_Send(unsigned char *DataToSend,u8 data_num)
{
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_ClearFlag(DMA1_FLAG_TC4);//清除DMA1通道4传输完成标志
    for(int i=0;i<data_num;i++)
    {
      USART1_TXBUFF[i]=*DataToSend;
      DataToSend++;
    }
    DMA_SetCurrDataCounter(DMA1_Channel4,data_num);
    DMA_Cmd(DMA1_Channel4,ENABLE);
}

void USART1_IRQHandler(void)
{
  //u8 buff;
  if(USART_GetITStatus(USART1,USART_IT_IDLE)!= RESET)				//总线空闲
  {
    //clear the idle pending flag 
    (void)USART1->SR;
    (void)USART1->DR;

/*
    buff=DMA_GetCurrDataCounter(DMA1_Channel5);
    DMA_Cmd(DMA1_Channel4,DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel4,USART1_DMA_RX_LEN-buff);
    DMA_Cmd(DMA1_Channel4,ENABLE);
*/

//此处可做数据解析

/*
    for(int i=0;i<USART1_DMA_RX_LEN;i++)
    {
      buff=USART1_RXBUFF[i];
      while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
      USART_SendData(USART1,buff);
    }
*/

    //开始新的DMA传输
    DMA_Cmd(DMA1_Channel5,DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel5,USART1_DMA_RX_LEN);
    DMA_Cmd(DMA1_Channel5,ENABLE);
  }
}
