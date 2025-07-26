#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "main.h"

#define LD_HEADER1    0xAA     //֡ͷ1
#define LD_HEADER2    0x55     //֡ͷ2
#define LD_HEADER3    0x44     //֡ͷ2
#define LD_F_LEN1        11     //֡����1
#define LD_F_LEN2        7     //֡����1
typedef struct
{
	uint16_t    angle;         //�Ƕ�
	uint16_t    distance;      //����

}LidarDataTypeDef;
extern LidarDataTypeDef lidardata[4];

void send_height(uint16_t height);

#endif
