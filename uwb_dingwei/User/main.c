/*! ------------------------------------------------------------------------------------------------------------------
 * @brief 基于PG5.8A基站和标签输出数据解析例程
 * 例程使用单片机：STM32F103C8T6
 * @author 广州联网科技有限公司
 * @web www.gzlwkj.com
 */
 
 /*
   2023/08/14 v2.2
	 修改了串口输出卡死问题
	 添加A基站数据解析例程
	 添加DMA发送
   2023/08/14 v2.3
     添加软件看门狗
	 增加了对接收不全数据的处理
	 修改了数据偏移操作
	 修改了环形缓存区的读取
*/

#include "stm32f10x.h"
#include "usart.h"
#include "timer.h"
#include "dma.h"
#include <string.h>


uint8_t usart1_rx_buf[400];	//读取串口数据缓存
#define ERROR_MAX	1200

int main()
{
    uint16_t usart1_rx_length=0;
	Usartx_Init(USART1,115200,72);	
	MYDMA_Config1();  //串口1 DMA打开，发送开DMA
	TIMx_Init(TIM2,3600-1,25-1);//通过定时器2辅助串口接收数据, 2.5ms
	TIMx_Init(TIM3,3600-1,2-1);
	while(1)
	{
		if(USART1_rx.usart1_flag)
		{
			usart1_rx_length = Usart1_Annular_txdata(usart1_rx_buf);//读取缓冲区的内容
			if(usart1_rx_length)	//读取到的长度
			{
				MODBUS(usart1_rx_buf, usart1_rx_length);
			}
		}
		if(ERROR_FLAG > ERROR_MAX)
		{
			ERROR_FLAG = 0;
			USART1_rx.usart1_flag = 0;
			USART1_rx.r = 0;
			USART1_rx.w = 0;
			USART1_rx.usart1_rx_len = 0;
			memset(usart1_rx_buf,0,sizeof(usart1_rx_buf));
		}
	}
}
