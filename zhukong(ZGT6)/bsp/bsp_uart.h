#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "main.h"

#define LD_HEADER1    0xAA     //帧头1
#define LD_HEADER2    0x55     //帧头2
#define LD_HEADER3    0x44     //帧头2
#define LD_F_LEN1        11     //帧长度1
#define LD_F_LEN2        7     //帧长度1
typedef struct
{
	uint16_t    angle;         //角度
	uint16_t    distance;      //距离

}LidarDataTypeDef;
extern LidarDataTypeDef lidardata[4];

void send_height(uint16_t height);

#endif
