#ifndef _UART_RECEIVE_H_
#define _UART_RECEIVE_H_

#include "SysConfig.h"

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
	u16 x, y;
	u16 distance, angle;
} CircleInfo; // �������Ϣ
typedef struct{
	s16 x, y;
	u16 h;
} PositionInfo; // ��ǰλ����Ϣ
typedef struct{
	u8 character;
	u8 num;
} SearchInfo;
typedef struct{
	u16 dis[4];
} LidarInfo;
extern CircleInfo circle_data;
extern PositionInfo position_data;
extern SearchInfo search_data;
extern LidarInfo lidar_data;

void UART3_DataParser(u8 data);

#endif
