#ifndef _UART_RECEIVE_H_
#define _UART_RECEIVE_H_

#include "main.h"

#define CMD_HEIGHT_GET       0x01  // 辅助定高
#define CMD_XY_GET            0x02  // 辅助定位X、Y
//#define CMD_Y_GET            0x03  // 辅助定位Y
#define CMD_SEARCH           0x04  // 定向查找货物
#define CMD_LAND             0x05  // 辅助降落
#define CMD_OBSTACLE_AVOID_1 0x06  // 避障(0°和90°)
#define CMD_OBSTACLE_AVOID_2 0x07  // 避障(180°和270°)

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
} CircleInfo; // 降落点信息
typedef struct{
	int16_t x, y, h;
} PositionInfo; // 当前位置信息
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
