#ifndef USART1_H
#define USART1_H
#include "main.h"

#define USART1_DMA_RX_LEN 256
#define USART1_DMA_TX_LEN 256
void Usart1_Init(u32 baudrate);
void Usart1_Send(unsigned char *DataToSend,u8 data_num);

#endif
