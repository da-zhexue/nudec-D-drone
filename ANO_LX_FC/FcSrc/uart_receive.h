#ifndef _UART_RECEIVE_H_
#define _UART_RECEIVE_H_

#include "SysConfig.h"

#define CMD_HEIGHT_GET     0x01  // 辅助定高
#define CMD_X_GET          0x02  // 辅助定位X
#define CMD_Y_GET          0x03  // 辅助定位Y
#define CMD_SEARCH         0x04  // 定向查找货物
#define CMD_LAND           0x05  // 辅助降落

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
} CircleInfo; // 降落点信息
typedef struct{
	u16 x, y, h;
} PositionInfo; // 当前位置信息
typedef struct{
	u8 character;
	u8 num;
} SearchInfo;

extern CircleInfo circle_data;
extern PositionInfo position_data;
extern SearchInfo search_data;

void UART3_DataParser(u8 data);

#endif
