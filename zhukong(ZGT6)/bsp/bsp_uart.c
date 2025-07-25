#include "usart.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"

uint8_t esp_data[128];
uint8_t drone_data[128];
uint8_t openmv_data[128];
int data_index=0;
int entryCount=0;
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
		send_data[2] = 0x01; // 指令位 0x01:高度 0x02:X(float) 0x03:Y(float) 0x04:货物位置 (如果x、y为uint16_t则0x02同时发送x、y)
    send_data[3] = (height >> 8) & 0xff; // 数据位1
    send_data[4] = height & 0xff; // 数据位2
		send_data[5] = 0x00; // 数据位3
		send_data[6] = 0x00; // 数据位4
    send_data[7] = 0x5D; // 帧尾
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
			if(drone_data[0] == 'o' && drone_data[1] == 'k')
					pwm_turn();
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

// uart6�ж���ax_laser.c��, �������ݮ������������, ���Ǹ��ļ�����ж�ע�͵�
// Raspberry Pi or Laser  TX:C6 RX:C7
//void USART6_IRQHandler(void)
//{

//  HAL_UART_IRQHandler(&huart6);

//}
