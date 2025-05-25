#ifndef __DMA_H
#define	__DMA_H	   

#include "stm32f10x.h"

#define USART1_DMA_SEND_MAXLEN 400
#define USART3_DMA_SEND_MAXLEN 400

extern uint8_t Usart1_dma_sendbuff[USART1_DMA_SEND_MAXLEN];
extern uint8_t Usart3_dma_sendbuff[USART3_DMA_SEND_MAXLEN];

void MYDMA_Config1(void);
void MYDMA_Config3(void);
void MYDMA_Enable1(u16 len);
void MYDMA_Enable3(u16 len);
#endif
