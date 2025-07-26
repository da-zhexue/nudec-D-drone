#include "uart_receive.h"
#include "usart.h"

CircleInfo circle_data = {.x = 80, .y = 60};
PositionInfo position_data;
SearchInfo search_data;
LidarInfo lidar_data;

void processUART3Data(uint8_t* data, uint8_t cmd);

void UART3_DataParser(uint8_t data) {
    static uart3_parse_state parse_state = WAIT_HEADER_AA;
    static uint8_t cmd_type = 0;
    static uint8_t data_index = 0;
    static uint8_t temp_buffer[5];  // 扩大缓冲区以容纳不同指令
    
    switch(parse_state) {
        case WAIT_HEADER_AA:
            if(data == 0xAA) 
                parse_state = WAIT_HEADER_55;
            break;
            
        case WAIT_HEADER_55:
            parse_state = (data == 0x55) ? WAIT_CMD : WAIT_HEADER_AA;
            break;
            
        case WAIT_CMD:
            cmd_type = data;  // 存储指令类型
            parse_state = PARSE_DATA;
            break;
            
        case PARSE_DATA:
            temp_buffer[data_index++] = data;
            if(data_index >= 4) {
                parse_state = WAIT_FOOTER_5D;
            }
            break;
            
        case WAIT_FOOTER_5D:
            if(data == 0x5D) {
                // 根据指令类型分发处理
								processUART3Data(temp_buffer, cmd_type);
            }
            parse_state = WAIT_HEADER_AA;
						data_index = 0;
            break;
    }
}

void processUART3Data(uint8_t* data, uint8_t cmd)
{
		switch(cmd){
			case CMD_HEIGHT_GET:
					position_data.h = (data[0] << 8) | data[1];
					break;
			case CMD_XY_GET:
					position_data.x = (data[0] << 8) | data[1];
					position_data.y = (data[2] << 8) | data[3];
					break;
			case CMD_SEARCH:
					search_data.character = data[0];
					search_data.num = data[1];
					break;
			case CMD_LAND:
					circle_data.x = data[0];
					circle_data.y = data[1];
					break;
			case CMD_OBSTACLE_AVOID_1:
					lidar_data.dis[0] = (data[0] << 8) | data[1];
					lidar_data.dis[1] = (data[2] << 8) | data[3];
					break;
			case CMD_OBSTACLE_AVOID_2:
					lidar_data.dis[2] = (data[0] << 8) | data[1];
					lidar_data.dis[3] = (data[2] << 8) | data[3];
					break;
		}
}
