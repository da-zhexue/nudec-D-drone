/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "stp23.h"
#include <stdio.h>
volatile int led_flag = 0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void esp_ap_init()
{
	
	//usart1: ESP8266                  TX:A9  RX:A10
	//usart2: Drone                    TX:A2  RX:A3
	//usart3: OPENMV                   TX:B10 RX:B11 
	//uart4:  STP_23 Laser             TX:C10 RX:C11
	//usart6: Raspberry Pi or Laser    TX:C6  RX:C7
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);  //receive interrupt6
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //idle interrupt
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);  //receive interrupt
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  //idle interrupt
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);  //receive interrupt
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);  //idle interrupt
//	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);  //receive interrupt
//	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);  //idle interrupt
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);  //receive interrupt
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);  //idle interrupt
	uint8_t at1[] = "AT+CWMODE=1\r\n";
	uint8_t at2[] = "AT+RST\r\n";
	uint8_t at3[] = "AT+CWJAP=\"esp8266\",\"66666666\"\r\n";
	uint8_t at4[] = "AT+CIPMUX=0\r\n";
	uint8_t at5[] = "AT+CIPSTART=\"TCP\",\"192.168.4.1\",8086\r\n";
	uint8_t at7[] = "AT+CIPSEND=4\r\n";
	uint8_t at8[] = "okok\r\n";
	HAL_UART_Transmit(&huart1,at1,sizeof(at1),100);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart1,at2,sizeof(at2),100);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart1,at3,sizeof(at3),100);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart1,at4,sizeof(at4),100);
	HAL_Delay(3000);
	HAL_UART_Transmit(&huart1,at5,sizeof(at5),100);
	HAL_Delay(5000);
	HAL_UART_Transmit(&huart1,at7,sizeof(at7),100);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart1,at8,sizeof(at8),100);
//	HAL_Delay(3000);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
}	
int fputc(int ch, FILE *f)
{
 
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
 
  return ch;
 
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
	esp_ap_init();
	pwm_on();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1) == GPIO_PIN_SET)
		{
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		}
    HAL_Delay(100);
    send_height(distance);
		//printf("distance: %d \n", distance);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
