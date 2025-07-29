#include "usart.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"

uint8_t esp_data[128];
uint8_t drone_data[128];
uint8_t openmv_data[128];
int data_index=0;
int entryCount=0;
uint8_t uart6_rx_con=0;
uint8_t uart6_rx_buf[128];
char storage[32][8];

uint8_t at1[] = "AT+CIPSEND=3\r\n";
uint8_t at2[] = "AT+CIPSEND=4\r\n";
uint16_t search_for_size(uint8_t *input) 
{
    return (input[1] >= '0' && input[1] <= '9') ? 2 : 1;
}
void send_height(uint16_t height)
{
		uint8_t send_data[8];
    send_data[0] = 0xAA; // 帧头1
		send_data[1] = 0x55; // 帧头2
		send_data[2] = 0x01; // 指令位 0x01:高度 0x02:X、Y  0x04:货物位置 (如果x、y为uint16_t则0x02同时发送x、y) 0x05:openmv辅助降落信息 
    send_data[3] = (height >> 8) & 0xff; // 数据位1
    send_data[4] = height & 0xff; // 数据位2
		send_data[5] = 0x00; // 数据位3
		send_data[6] = 0x00; // 数据位4
    send_data[7] = 0x5D; // 帧尾
    HAL_UART_Transmit(&huart2, send_data, 8, 0xffff);
}
void send_xy(int16_t x, int16_t y)
{
		uint8_t send_data[8];
    send_data[0] = 0xAA; // 帧头1
		send_data[1] = 0x55; // 帧头2
		send_data[2] = 0x02; // 指令位 0x01:高度 0x02:X、Y  0x04:货物位置 (如果x、y为uint16_t则0x02同时发送x、y) 0x05:openmv辅助降落信息 
    send_data[3] = (x >> 8) & 0xff; // 数据位1
    send_data[4] = x & 0xff; // 数据位2
		send_data[5] = (y >> 8) & 0xff; // 数据位3
		send_data[6] = y & 0xff; // 数据位4
    send_data[7] = 0x5D; // 帧尾
    HAL_UART_Transmit(&huart2, send_data, 8, 0xffff);
}
void send_obstacle(uint16_t angle, uint16_t dist)
{
		uint8_t send_data[8];
    send_data[0] = 0xAA; // 帧头1
		send_data[1] = 0x55; // 帧头2
		send_data[2] = 0x08; // 指令位 0x08:雷达避障信息(最近障碍物角度和距离)
    send_data[3] = (angle >> 8) & 0xff; // 数据位1
    send_data[4] = angle & 0xff; // 数据位2
		send_data[5] = (dist >> 8) & 0xff; // 数据位3
		send_data[6] = dist & 0xff; // 数据位4
    send_data[7] = 0x5D; // 帧尾
    HAL_UART_Transmit(&huart2, send_data, 8, 0xffff);
}
void send_lidar_data(LidarDataTypeDef *data)
{
		uint8_t send_data[8];
    send_data[0] = 0xAA; // 帧头1
		send_data[1] = 0x55; // 帧头2
		send_data[2] = 0x06; // 指令位 0x06:雷达避障信息(0°和90°) 0x07:雷达避障信息(180°和270°)
    send_data[3] = (data[0].distance >> 8) & 0xff; // 数据位1
    send_data[4] = data[0].distance & 0xff; // 数据位2
		send_data[5] = (data[1].distance >> 8) & 0xff; // 数据位3
		send_data[6] = data[1].distance & 0xff; // 数据位4
    send_data[7] = 0x5D; // 帧尾
    HAL_UART_Transmit(&huart2, send_data, 8, 0xffff);
	
		send_data[2] = 0x07; 
    send_data[3] = (data[2].distance >> 8) & 0xff; // 数据位1
    send_data[4] = data[2].distance & 0xff; // 数据位2
		send_data[5] = (data[3].distance >> 8) & 0xff; // 数据位3
		send_data[6] = data[3].distance & 0xff; // 数据位4
		HAL_UART_Transmit(&huart2, send_data, 8, 0xffff);
}
// ESP8266  TX:A9 RX:A10   
void USART1_IRQHandler(void)
{
	volatile uint8_t receive;
	//receive interrupt �����ж�
	if(huart1.Instance->SR & UART_FLAG_RXNE)
	{
			receive = huart1.Instance->DR;
			
			esp_data[data_index++]=receive;
		
	}
	//idle interrupt �����ж�
	else if(huart1.Instance->SR & UART_FLAG_IDLE)
	{
			receive = huart1.Instance->DR;
			if(esp_data[2] == '+' && esp_data[3] == 'I' && esp_data[9]<='D' && esp_data[9]>='A' && esp_data[10]<='9' && esp_data[10]>='0')
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
				uint8_t transform_data[8];
				transform_data[0] = 0xAA;
				transform_data[1] = 0x55;
				transform_data[2] = 0x04;
				transform_data[3] = esp_data[9];
				transform_data[4] = esp_data[10];
				transform_data[5] = 0x00;
				transform_data[6] = 0x00;
				transform_data[7] = 0x5D;
				HAL_UART_Transmit(&huart2,transform_data,sizeof(transform_data),100);
				int tt=0;
					for(volatile int i=0;i<10000;i++)
							tt++;
			}
			//HAL_UART_Transmit(&huart2,esp_data,sizeof(esp_data),100);
			for(int i=0;i<=data_index;i++)
				esp_data[i]=0;
			data_index=0;
	}
}

// Drone  TX:A2 RX:A3
void USART2_IRQHandler(void)
{
	volatile uint8_t receive;
	//receive interrupt �����ж�
	if(huart2.Instance->SR & UART_FLAG_RXNE)
	{
			receive = huart2.Instance->DR;
			drone_data[data_index++]=receive;
		
	}
	//idle interrupt �����ж�
	else if(huart2.Instance->SR & UART_FLAG_IDLE)
	{
			receive = huart2.Instance->DR;
			if(drone_data[0] == 0xAA && drone_data[1] == 0x55 && drone_data[3] == 0x5D)
			{
					if(drone_data[2] == 0x01)
							pwm_yaw();
					else if(drone_data[2] == 0x02)
							pwm_pitch();
			}
	}
}

//OPENMV  TX:B10 RX:B11
void USART3_IRQHandler(void)
{
	volatile uint8_t receive;
	//receive interrupt �����ж�
	if(huart3.Instance->SR & UART_FLAG_RXNE)
	{
			receive = huart3.Instance->DR;
			
			openmv_data[data_index++]=receive;
		
	}
	//idle interrupt �����ж�
	else if(huart3.Instance->SR & UART_FLAG_IDLE)
	{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
			receive = huart3.Instance->DR;
			uint16_t flag = search_for_size(openmv_data);
			openmv_data[data_index++]='\r';
			openmv_data[data_index++]='\n';
			if(flag==1)
					HAL_UART_Transmit(&huart1,at1,sizeof(at1),100);
			else 
					HAL_UART_Transmit(&huart1,at2,sizeof(at2),100);
			int tt=0;
			for(volatile int i=0;i<10000;i++)
					tt++;
			HAL_UART_Transmit(&huart1,openmv_data,sizeof(openmv_data),100);	
			for(int i=0;i<=data_index;i++)
					openmv_data[i]=0;
			data_index = 0;
      //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	}
}

void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
	volatile uint8_t Res;
	LidarDataTypeDef lidardata[4];
	if(huart6.Instance->SR & UART_FLAG_RXNE)
	{
			Res = huart6.Instance->DR;
			
			if (uart6_rx_con < 2)
			{
				//接收帧头
				if(uart6_rx_con == 0)  
				{
					//判断帧头1 AA
					if(Res == LD_HEADER1)
					{
						uart6_rx_buf[uart6_rx_con] = Res;
						uart6_rx_con = 1;					
					}
				}
				else
				{
					//判断帧头2 55
					if(Res == LD_HEADER2 || Res == LD_HEADER3 || Res == LD_HEADER4)
					{
						uart6_rx_buf[uart6_rx_con] = Res;
						uart6_rx_con = 2;					
					}
					else uart6_rx_con = 0;
						
				}
			}
			else  //接收数据
			{
				//判断是否接收完
				if(uart6_rx_buf[1] == LD_HEADER2)
				{
					if(uart6_rx_con < LD_F_LEN1)
					{
						uart6_rx_buf[uart6_rx_con] = Res;
						uart6_rx_con++;
					}
					else
					{
						
						for (int i = 0; i < 4; i++) {
							uint16_t high_byte = uart6_rx_buf[2 + i*2];
							uint16_t low_byte  = uart6_rx_buf[3 + i*2];
							lidardata[i].distance = (high_byte << 8) | low_byte;
							lidardata[i].angle = i * 90;
						}
						send_lidar_data(lidardata);
	
						for(int i=0; i<LD_F_LEN1; i++)
						{	
							uart6_rx_buf[i] = 0;
						}
						//复位
						uart6_rx_con = 0;
					}
				}
				else if(uart6_rx_buf[1] == LD_HEADER3 || uart6_rx_buf[1] == LD_HEADER4)
				{
					if(uart6_rx_con < LD_F_LEN2)
					{
						uart6_rx_buf[uart6_rx_con] = Res;
						uart6_rx_con++;
					}
					else
					{
						if(uart6_rx_buf[1] == LD_HEADER3)
						{
							int16_t lidar_x = (uart6_rx_buf[2] << 8) | uart6_rx_buf[3];
							int16_t lidar_y = (uart6_rx_buf[4] << 8) | uart6_rx_buf[5];
							send_xy(lidar_x, lidar_y);
						}
						else if(uart6_rx_buf[1] == LD_HEADER4)
						{
							uint16_t min_angle = (uart6_rx_buf[2] << 8) | uart6_rx_buf[3];
							uint16_t min_dist  = (uart6_rx_buf[4] << 8) | uart6_rx_buf[5];
							send_obstacle(min_angle, min_dist);
						}
						for(int i=0; i<LD_F_LEN1; i++)
						{	
							uart6_rx_buf[i] = 0;
						}
						//复位
						uart6_rx_con = 0;
					}
				}
			}
			
	}
	else if(huart6.Instance->SR & UART_FLAG_IDLE)
	{
			Res = huart6.Instance->DR;
	}
}
