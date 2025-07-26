#ifndef _UART_RECEIVE_H_
#define _UART_RECEIVE_H_

#include "main.h"

#define CMD_HEIGHT_GET       0x01  // ��������
#define CMD_XY_GET            0x02  // ������λX��Y
//#define CMD_Y_GET            0x03  // ������λY
#define CMD_SEARCH           0x04  // ������һ���
#define CMD_LAND             0x05  // ��������
#define CMD_OBSTACLE_AVOID_1 0x06  // ����(0���90��)
#define CMD_OBSTACLE_AVOID_2 0x07  // ����(180���270��)

typedef enum {
    WAIT_HEADER_AA,
    WAIT_HEADER_55,
    WAIT_CMD,
    PARSE_DATA,
    WAIT_FOOTER_5D
} uart3_parse_state;

typedef struct{
	uint16_t x, y;
	uint16_t distance, angle;
} CircleInfo; // �������Ϣ
typedef struct{
	int16_t x, y, h;
} PositionInfo; // ��ǰλ����Ϣ
typedef struct{
	uint8_t character;
	uint8_t num;
} SearchInfo;
typedef struct{
	uint16_t dis[4];
} LidarInfo;
extern CircleInfo circle_data;
extern PositionInfo position_data;
extern SearchInfo search_data;
extern LidarInfo lidar_data;

void UART3_DataParser(uint8_t data);

#endif
