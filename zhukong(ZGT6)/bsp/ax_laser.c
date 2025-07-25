#include "ax_laser.h"
#include <stdio.h>
#include "usart.h"
#include "math.h"

static uint8_t uart6_rx_con=0;        //���ռ�����
static uint8_t uart6_rx_buf[100];     //���ջ���
static uint8_t uart6_rx_buf2[100];     //���ջ���
uint16_t ax_lidar_data[360];

// �˲�����
void PointCloudFilter(LaserScan *Scan) {
    int i0, i1, i2, i3, i4;
    float x0, y0, x1, y1, x2, y2, x3, y3, r1, r2;
    float d01, d12, d23, d34;
    float a, b, Adeta;
    float FilterRatioAdj;
    float AngDiff;
    int DepthState;
    const float FilterRatio = 2.5f;
    const float PI = 3.14159265358979323846f;

    // ����ǶȲ�(����)
    AngDiff = 2 * PI / (Scan->size - 1);
    FilterRatioAdj = FilterRatio * sinf(AngDiff);

    // ��һ���˲�������4���������
    for (i0 = 0; i0 < Scan->size; i0++) {
        i1 = (i0 + 1) % Scan->size;
        i2 = (i0 + 2) % Scan->size;
        i3 = (i0 + 3) % Scan->size;
        
        DepthState = 0;
        DepthState |= (Scan->points[i0].range != 0) ? 0x01 : 0;
        DepthState |= (Scan->points[i1].range != 0) ? 0x02 : 0;
        DepthState |= (Scan->points[i2].range != 0) ? 0x04 : 0;
        DepthState |= (Scan->points[i3].range != 0) ? 0x08 : 0;
        
        if (DepthState == 0x0F) { // 1111
            // ����ѿ�������
            x0 = Scan->points[i0].range * cosf(Scan->points[i0].angle * PI / 180.0f);
            y0 = Scan->points[i0].range * sinf(Scan->points[i0].angle * PI / 180.0f);
            x3 = Scan->points[i3].range * cosf(Scan->points[i3].angle * PI / 180.0f);
            y3 = Scan->points[i3].range * sinf(Scan->points[i3].angle * PI / 180.0f);
            
            // �����ֵ��
            x1 = (x3 + 2 * x0) / 3;
            y1 = (y3 + 2 * y0) / 3;
            x2 = (2 * x3 + x0) / 3;
            y2 = (2 * y3 + y0) / 3;
            
            r1 = sqrtf(x1 * x1 + y1 * y1);
            r2 = sqrtf(x2 * x2 + y2 * y2);
            
            // �����ҳ�
            a = Scan->points[i0].range;
            b = Scan->points[i1].range;
            Adeta = fabsf(Scan->points[i0].angle - Scan->points[i1].angle) * PI / 180.0f;
            d01 = sqrtf(a * a + b * b - 2 * a * b * cosf(Adeta));
            
            a = Scan->points[i1].range;
            b = Scan->points[i2].range;
            Adeta = fabsf(Scan->points[i1].angle - Scan->points[i2].angle) * PI / 180.0f;
            d12 = sqrtf(a * a + b * b - 2 * a * b * cosf(Adeta));
            
            a = Scan->points[i2].range;
            b = Scan->points[i3].range;
            Adeta = fabsf(Scan->points[i2].angle - Scan->points[i3].angle) * PI / 180.0f;
            d23 = sqrtf(a * a + b * b - 2 * a * b * cosf(Adeta));
            
            // �쳣�����
            if (FilterRatioAdj * Scan->points[i1].range < d01 && 
                FilterRatioAdj * Scan->points[i1].range < d12) {
                Scan->points[i1].range = 0;
            }
            
            if (FilterRatioAdj * Scan->points[i2].range < d12 && 
                FilterRatioAdj * Scan->points[i2].range < d23) {
                Scan->points[i2].range = 0;
            }
            
            if (Scan->points[i1].range == 0 || Scan->points[i2].range == 0) {
                continue;
            }
            
            // ƽ������
            if ((Scan->points[i1].range > r1 && Scan->points[i2].range < r2) || 
                (Scan->points[i1].range < r1 && Scan->points[i2].range > r2)) {
                if (!((d01 > FilterRatioAdj * Scan->points[i0].range && 
                       d12 < FilterRatioAdj * Scan->points[i1].range && 
                       d23 < FilterRatioAdj * Scan->points[i2].range) ||
                      (d01 < FilterRatioAdj * Scan->points[i0].range && 
                       d12 > FilterRatioAdj * Scan->points[i1].range && 
                       d23 < FilterRatioAdj * Scan->points[i2].range) ||
                      (d01 < FilterRatioAdj * Scan->points[i0].range && 
                       d12 < FilterRatioAdj * Scan->points[i1].range && 
                       d23 > FilterRatioAdj * Scan->points[i2].range))) {
                    Scan->points[i1].range = r1 + 0.4f * (Scan->points[i1].range - r1);
                    Scan->points[i2].range = r2 + 0.4f * (Scan->points[i2].range - r2);
                }
            }
        } 
        // ����״̬��������ԭ�߼���
        else if (DepthState == 0x00 || DepthState == 0x08 || DepthState == 0x01) {
            // �ղ���
        } 
        else if ((DepthState & 0x0E) == 0x04) {
            Scan->points[i2].range = 0;
        } 
        else if ((DepthState & 0x07) == 0x02) {
            Scan->points[i1].range = 0;
        } 
        else if ((DepthState & 0x07) == 0x03) {
            a = Scan->points[i0].range;
            b = Scan->points[i1].range;
            if (sqrtf(a * a + b * b - 2 * a * b * cosf(AngDiff)) > FilterRatioAdj * Scan->points[i1].range) {
                Scan->points[i1].range = 0;
            }
        } 
        else if (DepthState == 0x0E) {
            a = Scan->points[i1].range;
            b = Scan->points[i2].range;
            if (sqrtf(a * a + b * b - 2 * a * b * cosf(AngDiff)) > FilterRatioAdj * Scan->points[i1].range) {
                Scan->points[i1].range = 0;
            }
        } 
        else if ((DepthState & 0x0E) == 0x0C) {
            a = Scan->points[i2].range;
            b = Scan->points[i3].range;
            if (sqrtf(a * a + b * b - 2 * a * b * cosf(AngDiff)) > FilterRatioAdj * Scan->points[i2].range) {
                Scan->points[i2].range = 0;
            }
        } 
        else if (DepthState == 0x07) {
            a = Scan->points[i1].range;
            b = Scan->points[i2].range;
            if (sqrtf(a * a + b * b - 2 * a * b * cosf(AngDiff)) > FilterRatioAdj * Scan->points[i2].range) {
                Scan->points[i2].range = 0;
            }
        } 
        else if (DepthState == 0x06) {
            a = Scan->points[i1].range;
            b = Scan->points[i2].range;
            if (sqrtf(a * a + b * b - 2 * a * b * cosf(AngDiff)) > FilterRatioAdj * Scan->points[i1].range) {
                Scan->points[i1].range = 0;
                Scan->points[i2].range = 0;
            }
        }
    }

    // �ڶ����˲�������5���������
    for (i0 = 0; i0 < Scan->size; i0++) {
        i1 = (i0 + 1) % Scan->size;
        i2 = (i0 + 2) % Scan->size;
        i3 = (i0 + 3) % Scan->size;
        i4 = (i0 + 4) % Scan->size;
        
        if (Scan->points[i0].range != 0 && Scan->points[i1].range != 0 && 
            Scan->points[i2].range != 0 && Scan->points[i3].range != 0 && 
            Scan->points[i4].range != 0) {
                
            // �����ҳ�
            a = Scan->points[i0].range;
            b = Scan->points[i1].range;
            Adeta = fabsf(Scan->points[i0].angle - Scan->points[i1].angle) * PI / 180.0f;
            d01 = sqrtf(a * a + b * b - 2 * a * b * cosf(Adeta));
            
            a = Scan->points[i1].range;
            b = Scan->points[i2].range;
            Adeta = fabsf(Scan->points[i1].angle - Scan->points[i2].angle) * PI / 180.0f;
            d12 = sqrtf(a * a + b * b - 2 * a * b * cosf(Adeta));
            
            a = Scan->points[i2].range;
            b = Scan->points[i3].range;
            Adeta = fabsf(Scan->points[i2].angle - Scan->points[i3].angle) * PI / 180.0f;
            d23 = sqrtf(a * a + b * b - 2 * a * b * cosf(Adeta));
            
            a = Scan->points[i3].range;
            b = Scan->points[i4].range;
            Adeta = fabsf(Scan->points[i3].angle - Scan->points[i4].angle) * PI / 180.0f;
            d34 = sqrtf(a * a + b * b - 2 * a * b * cosf(Adeta));
            
            // �޸��м��쳣��
            if (d01 < FilterRatioAdj * Scan->points[i1].range && 
                d34 < FilterRatioAdj * Scan->points[i3].range && 
                (d12 > FilterRatioAdj * Scan->points[i2].range || 
                 d23 > FilterRatioAdj * Scan->points[i2].range)) {
                    
                if ((Scan->points[i0].range < Scan->points[i1].range && 
                     Scan->points[i3].range < Scan->points[i4].range) || 
                    (Scan->points[i0].range > Scan->points[i1].range && 
                     Scan->points[i3].range > Scan->points[i4].range)) {
                    Scan->points[i2].range = (Scan->points[i1].range + Scan->points[i3].range) / 2.0f;
                }
            }
        } 
        // �߽��ֵ����
        else if (Scan->points[i0].range == 0 && Scan->points[i1].range != 0 && 
                 Scan->points[i2].range != 0 && Scan->points[i3].range != 0 && 
                 Scan->points[i4].range != 0) {
            // ����ѿ�������
            x2 = Scan->points[i2].range * cosf(Scan->points[i2].angle * PI / 180.0f);
            y2 = Scan->points[i2].range * sinf(Scan->points[i2].angle * PI / 180.0f);
            x3 = Scan->points[i3].range * cosf(Scan->points[i3].angle * PI / 180.0f);
            y3 = Scan->points[i3].range * sinf(Scan->points[i3].angle * PI / 180.0f);
            
            // ��ֵ������
            x1 = 2 * x2 - x3;
            y1 = 2 * y2 - y3;
            Scan->points[i1].range = Scan->points[i1].range + 0.4f * (sqrtf(x1 * x1 + y1 * y1) - Scan->points[i1].range);
        } 
        else if (Scan->points[i0].range != 0 && Scan->points[i1].range != 0 && 
                 Scan->points[i2].range != 0 && Scan->points[i3].range != 0 && 
                 Scan->points[i4].range == 0) {
            x1 = Scan->points[i1].range * cosf(Scan->points[i1].angle * PI / 180.0f);
            y1 = Scan->points[i1].range * sinf(Scan->points[i1].angle * PI / 180.0f);
            x2 = Scan->points[i2].range * cosf(Scan->points[i2].angle * PI / 180.0f);
            y2 = Scan->points[i2].range * sinf(Scan->points[i2].angle * PI / 180.0f);
            
            x3 = 2 * x2 - x1;
            y3 = 2 * y2 - y1;
            Scan->points[i3].range = Scan->points[i3].range + 0.4f * (sqrtf(x3 * x3 + y3 * y3) - Scan->points[i3].range);
        }
    }
}
void ProcessLidarData(void) {
    LaserScan scan;
    scan.size = 360;
    
    // ��ʼ��ɨ������
    for (int i = 0; i < 360; i++) {
        scan.points[i].angle = (float)i; // �Ƕ�0-359��
        scan.points[i].range = ax_lidar_data[i]; // ��������(����)
    }
    
    // ִ���˲�
    PointCloudFilter(&scan);
    
    // �����״�����
    for (int i = 0; i < 360; i++) {
        ax_lidar_data[i] = scan.points[i].range;
    }
}
void AX_LIDAR_Start(void)
{
	HAL_UART_Transmit(&huart6, 0x00, 1, 100);
	uint8_t uart6_tx_buf[4];     //���ͻ���
	
	uart6_tx_buf[0] = 0xAA;    //֡ͷ
	uart6_tx_buf[1] = 0x55;    //֡ͷ
	uart6_tx_buf[2] = 0xF0;    //ָ��
	uart6_tx_buf[3] = 0x0F;     //ָ��
	
	//��ѯ���䷽ʽ��������
	HAL_UART_Transmit(&huart6, uart6_tx_buf, sizeof(uart6_tx_buf),100);
}

/**
  * @��  ��  �״�ֹͣ��תɨ��
  * @��  ��  ��
  * @����ֵ	 ��
  */
void AX_LIDAR_Stopping(void)
{
	uint8_t uart6_tx_buf[4];     //���ͻ���
	
	uart6_tx_buf[0] = 0xAA;    //֡ͷ
	uart6_tx_buf[1] = 0x55;    //֡ͷ
	uart6_tx_buf[2] = 0xF5;    //ָ��
	uart6_tx_buf[3] = 0x0A;    //ָ��
	
	//��ѯ���䷽ʽ��������
	HAL_UART_Transmit(&huart6, uart6_tx_buf, sizeof(uart6_tx_buf),100);	
	
}

void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
	volatile uint8_t Res;

	if(huart6.Instance->SR & UART_FLAG_RXNE)
	{
			Res = huart6.Instance->DR;
			
			if (uart6_rx_con < 2)
			{
				//����֡ͷ
				if(uart6_rx_con == 0)  
				{
					//�ж�֡ͷ1 AA
					if(Res == LD_HEADER1)
					{
						uart6_rx_buf[uart6_rx_con] = Res;
						uart6_rx_con = 1;					
					}
				}
				else
				{
					//�ж�֡ͷ2 55
					if(Res == LD_HEADER2)
					{
						uart6_rx_buf[uart6_rx_con] = Res;
						uart6_rx_con = 2;					
					}
				}
			}
			else  //��������
			{
				//�ж��Ƿ������
				if(uart6_rx_con < LD_F_LEN)
				{
					uart6_rx_buf[uart6_rx_con] = Res;
					uart6_rx_con++;
				}
				else
				{
					//CRCУ��ʡ��
		
					//���ݻ��浽�����飬������
					for(int i=0; i<LD_F_LEN; i++)
					{	
						uart6_rx_buf2[i] = uart6_rx_buf[i];
						uart6_rx_buf[i] = 0;
					}
					//��λ
					LD_DataHandle();
					uart6_rx_con = 0;
				}
			}
			
	}
	else if(huart6.Instance->SR & UART_FLAG_IDLE)
	{
			Res = huart6.Instance->DR;
	}
}

/**
  * @��  ��  ���ݴ�����
  * @��  ��  ��
  * @����ֵ	 ��
  */
void LD_DataHandle(void)
{
	uint8_t i;
	uint16_t index;
	float temp;
	
	//һ֡����25��������
	static LidarPointTypeDef frame_data[25];
	
	//һ֡������ʼ�ͽ����Ƕ�
	float angle_start = (((uint16_t)uart6_rx_buf2[5]<<8) + (uart6_rx_buf2[4]>>1))/64.0;
	float angle_end   = (((uint16_t)uart6_rx_buf2[7]<<8) + (uart6_rx_buf2[6]>>1))/64.0;
	float angle_area;
	
	//��ʼ�Ƕȴ��ڽ����Ƕȣ����360��
	if(angle_start > angle_end)
		angle_end += 360;
	angle_area  = (angle_end - angle_start)/24.0;
	for(i=0; i<25; i++)
	{
	
		temp = angle_start + angle_area*i;
		
		if(temp >= 360)
		{
			temp -= 360;
		}
		frame_data[i].angle = temp;
		//�������
		frame_data[i].distance =  ((uint16_t)uart6_rx_buf2[12+i*3]<<6) + (uart6_rx_buf2[11+i*3]>>2);
		
	}
	//���������Ϣ

	//����360�ȵ���Ч����
	for(i=0; i<25; i++)
	{				
		//�Ƕȶ�Ӧ�����±�
		index = (int)frame_data[i].angle;
		
		//�����������
		ax_lidar_data[index] = frame_data[i].distance;
	}
	//ProcessLidarData();
}



/******************* (C) ��Ȩ 2025 XTARK **************************************/
