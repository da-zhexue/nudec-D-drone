#include "uart_receive.h"
#include "Drv_Uart.h"
#include "LX_FC_Fun.h"

CircleInfo circle_data = {.x = 80, .y = 60};
PositionInfo position_data;
SearchInfo search_data;
LidarInfo lidar_data;

void processUART3Data(u8* data, u8 cmd);

void UART3_DataParser(u8 data) {
    static uart3_parse_state parse_state = WAIT_HEADER_AA;
    static u8 cmd_type = 0;
    static u8 data_index = 0;
    static u8 temp_buffer[5];  // 扩大缓冲区以容纳不同指令
    
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

void processUART3Data(u8* data, u8 cmd)
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

void pwm_turn(u8 cmd) // cmd=0x01->yaw    0x02->pitch 
{
		u8 uart2_send_data[4];
		uart2_send_data[0] = 0xAA;
		uart2_send_data[1] = 0x55;
		uart2_send_data[2] = cmd;
		uart2_send_data[3] = 0x5D;
		
		DrvUart2SendBuf(uart2_send_data, sizeof(uart2_send_data));
}

void time_dly_cnt(u16 delay_ms, u8* step)
{
		static u16 dly_cnt = 0;
		if(dly_cnt<delay_ms)
		{
				dly_cnt+=20;//ms
		}
		else
		{
				dly_cnt = 0;
				step += 1;
		}			
}

void XY_Compensate(s16 current_x, s16 target_x, s16 current_y, s16 target_y, u8* step)
{
		static u8 compensate_step = 0;
		s16 move_x_cm = target_x - current_x;
		s16 move_y_cm = target_y - current_y;
		switch (compensate_step)
		{
				case 0:
						if(move_x_cm >= NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_x_cm, COMPENSATE_VELOCITY, 90);
						else if(move_x_cm <= -NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_x_cm*(-1), COMPENSATE_VELOCITY, 270);
						else 
								compensate_step++;
						break;
				case 1:
						//等3秒
						time_dly_cnt(3000, &compensate_step);
						break;
				case 2:
						if(move_y_cm >= NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_y_cm, COMPENSATE_VELOCITY, 180);
						else if(move_y_cm <= -NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_y_cm*(-1), COMPENSATE_VELOCITY, 0);
						else 
								compensate_step++;
						break;
				case 3:
						//等3秒
						time_dly_cnt(3000, &compensate_step);
						step++;
						break;
		}
		
}
