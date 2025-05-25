#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"
#include <stdio.h>
#include "sys.h"
/********************队列接收串口数据********************/
#define USART1_LEN 800 //缓冲区大小
typedef struct 
{
	unsigned char buff[USART1_LEN];//缓冲区
    uint16_t usart1_rx_len;//保存的数据长度
    uint8_t usart1_flag;//数据接收完成标志
	uint16_t w;//写
	uint16_t r;//读
	uint8_t is_last_reserve;//上一次接收不全标志位
	uint16_t last_reserve_length;//上一次接收不全的长度
}USART1_RX;

extern USART1_RX USART1_rx;
void Usartx_Init(USART_TypeDef *USARTx,u32 baud,u32 sysclk);
extern uint16_t ERROR_FLAG;  						              //测距错误计算次数标志位，达到一定次数跳出


uint16_t Usart1_Annular_txdata(uint8_t *tx_data);
void Usart1_SendString(unsigned char *data,unsigned int num);//串口1发送字符串

void MODBUS(unsigned char *recv_buffer,unsigned int length);

#endif

