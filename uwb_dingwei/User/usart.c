#include "usart.h"
#include "string.h"
#include "dma.h"
#include "timer.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


#define ANCHOR_LIST_COUNT			16

#define TAG_OUTPUT_DIST 		(1 << 0)
#define TAG_OUTPUT_RTLS 		(1 << 1)

#define ANC_PROTOCAL_RTLS 		(1 << 0)
#define ANC_PROTOCAL_DIST		(1 << 1)
#define ANC_PROTOCAL_RXDIAG 	(1 << 2)
#define ANC_PROTOCAL_TIMESTAMP 	(1 << 3)

//yqf
// Structure to represent a 3D vector
typedef struct {
    double x;
    double y;
    double z;
} Vector3d;

// Structure for the UWB solver
typedef struct {
    double A[3][3];      // Matrix A
    double b_tmp[3];     // Temporary b vector for calculations
} UWBSolver;
//end yqf

typedef struct 
{
	uint16_t Tag_ID;  	//标签ID
	uint32_t Cal_Flag;  //测距成功标志位 第8位 1：定位成功 第0-7：1分别代表A-H基站测距成功
	int16_t x;         	//计算出的x坐标 单位cm
	int16_t y;         	//计算出的y坐标 单位cm
	int16_t z;         	//计算出的z坐标 单位cm
	uint16_t Dist[ANCHOR_LIST_COUNT];     //测得标签与A-H基站的距离
	uint32_t Time_ts[6];      //时间戳
}Cal_data_t;

typedef struct {
  uint16_t Max_noise;
  uint16_t Std_noise;
  uint16_t Fp_amp1;
  uint16_t Fp_amp2;
  uint16_t Fp_amp3;
  uint16_t Max_growthCIR;
  uint16_t Rx_preambleCount;
  uint16_t Fp;
  double Fp_power;
  double Rx_power;
}Rx_diag_t;

Cal_data_t Last_cal_data_hds;
Rx_diag_t Rx_diag;    //基站接收信息列表

USART1_RX USART1_rx={0,0,0,0,0};//串口接收数据缓冲区初始化

char Tag_ouput_dist_str[400] = {'0'};
uint8_t temp = 0;

uint16_t ERROR_FLAG;  						              //测距错误计算次数标志位，达到一定次数跳出

#define MODBUS_ID 0x01		//Modbus ID号

const unsigned char auchCRCHi[] = /* CRC锟斤拷位锟街节憋拷*/
{ 	 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 

const unsigned char auchCRCLo[] = /* CRC锟斤拷位锟街节憋拷*/ 
{ 
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC,
	0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 
	0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 
	0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 
	0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 
	0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 
	0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 
	0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 
	0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 
	0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 
	0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 
	0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 
	0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 
	0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 
	0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;	 

/******************************************************************************
												    CRC校验
*******************************************************************************/
unsigned int CRC_Calculate(unsigned char *pdata,unsigned int num)
{
  unsigned char uchCRCHi = 0xFF ;               
	unsigned char  uchCRCLo = 0xFF ;               
	unsigned char uIndex ;                
	while(num --)                    
	{
		uIndex = uchCRCHi^*pdata++ ;
		uchCRCHi = uchCRCLo^auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo) ;
}

void Usartx_Init(USART_TypeDef *USARTx,u32 baud,u32 sysclk)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	if(USART1 == USARTx)
	{
		/*1.开时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		
		USART_InitStructure.USART_BaudRate = baud;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		/*2.配置GPIO口*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* Configure USART Rx as input floating */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		#ifdef USART1_IQR
			USART1->CR1|=1<<5;//开启串口接收中断
			STM32_NVIC_SetPriority(USART1_IRQn,0,1);//设置优先级
		#endif
		USART_Init(USART1, &USART_InitStructure);
	}
	/*3.配置串口核心寄存器*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
	USART_Cmd(USART1, ENABLE);
}
/************************串口发送字符************************/
void Usart1_SendString(unsigned char *data,unsigned int num)
{
	unsigned int i;
	if(temp == 1)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET){};
		DMA_ClearFlag(DMA1_FLAG_TC4);
	}
	else
		temp = 1;
	for(i = 0;i < num;i++)
	{
		Usart1_dma_sendbuff[i]=data[i];		
	}
	MYDMA_Enable1(num);
}
/************************串口接收中断************************/
void USART1_IRQHandler(void)
{
	uint8_t c;
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
        c = USART_ReceiveData(USART1);
        if(USART1_rx.usart1_rx_len < USART1_LEN)
        {            
            //写入数据到缓冲区
            USART1_rx.buff[USART1_rx.w] = c;
            USART1_rx.w = (USART1_rx.w + 1) % USART1_LEN;//防止地址越界
            USART1_rx.usart1_rx_len++;
			TIM2->CNT=0;//清空计数器值
			TIM_Cmd(TIM2, ENABLE);//打开定时器
        }
        else USART1_rx.usart1_flag = 1;//缓冲区满	
	}
	USART_ClearFlag(USART1,USART_IT_RXNE);
}
/**
 * @brief 从缓冲区读取数据
 * @param *tx_data 	读取数据保存地址
 */
uint16_t Usart1_Annular_txdata(uint8_t *tx_data)
{
    uint16_t len = 0;
	uint16_t read_len = 0;
    //缓冲区为空 或者 USART1_rx.usart1_flag 数据接收完成标志
    if(USART1_rx.usart1_rx_len == 0 || USART1_rx.usart1_flag == 0)
		return 0;
	
	if(USART1_rx.is_last_reserve)
	{
		read_len = USART1_rx.usart1_rx_len + USART1_rx.last_reserve_length;		//需要读取的长度要加上剩余的长度
		USART1_rx.is_last_reserve = 0;
	}
	else
	{
		read_len = USART1_rx.usart1_rx_len;		//没有剩余的长度 直接读取
	}
	
    while(read_len)		//读取缓冲区全部数据
    {
        *tx_data = USART1_rx.buff[USART1_rx.r];//读取缓冲区数据
        USART1_rx.r = (USART1_rx.r + 1) % USART1_LEN;
        read_len--;//缓冲区长度-1
        tx_data++;
        len++;
    }
	USART1_rx.usart1_rx_len = 0;	//缓冲区长度清零
    USART1_rx.usart1_flag = 0;//清除标志位
    return len;
}

//--------  yqf  ----
// Function to calculate the determinant of a 3x3 matrix
double determinant3x3(double matrix[3][3]) {
    return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
           matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
           matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

// Function to subtract two vectors
Vector3d vector_subtract(Vector3d a, Vector3d b) {
    Vector3d result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

// Function to calculate the cross product of two vectors
Vector3d cross_product(Vector3d a, Vector3d b) {
    Vector3d result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

// Function to calculate the dot product of two vectors
double dot_product(Vector3d a, Vector3d b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


// Initialize the UWB solver
bool uwb_solver_init(UWBSolver* solver, Vector3d anchors[], int num_anchors, double coplanar_threshold) {
    if (num_anchors != 4) {
        //printf("Error: The number of anchors must be 4.\n");
        return false;
    }

    // Check if the anchors are coplanar
    Vector3d v1 = vector_subtract(anchors[1], anchors[0]);
    Vector3d v2 = vector_subtract(anchors[2], anchors[0]);
    Vector3d v3 = vector_subtract(anchors[3], anchors[0]);
    Vector3d normal = cross_product(v1, v2);
    
    if (fabs(dot_product(normal, v3)) < coplanar_threshold) {
        //printf("Error: The anchors are coplanar.\n");
        return false;
    }

    // Build matrix A
    solver->A[0][0] = anchors[1].x - anchors[0].x;
    solver->A[0][1] = anchors[1].y - anchors[0].y;
    solver->A[0][2] = anchors[1].z - anchors[0].z;
    
    solver->A[1][0] = anchors[2].x - anchors[0].x;
    solver->A[1][1] = anchors[2].y - anchors[0].y;
    solver->A[1][2] = anchors[2].z - anchors[0].z;
    
    solver->A[2][0] = anchors[3].x - anchors[0].x;
    solver->A[2][1] = anchors[3].y - anchors[0].y;
    solver->A[2][2] = anchors[3].z - anchors[0].z;

    // Check if the matrix is invertible
    double det = determinant3x3(solver->A);
    if (fabs(det) < 1e-8) {
        //printf("Error: Matrix A is near-singular or singular. Cannot compute inverse.\n");
        return false;
    }

    // Calculate b_tmp
    solver->b_tmp[0] = (anchors[1].x * anchors[1].x - anchors[0].x * anchors[0].x +
                       anchors[1].y * anchors[1].y - anchors[0].y * anchors[0].y +
                       anchors[1].z * anchors[1].z - anchors[0].z * anchors[0].z) / 2.0;
    
    solver->b_tmp[1] = (anchors[2].x * anchors[2].x - anchors[0].x * anchors[0].x +
                       anchors[2].y * anchors[2].y - anchors[0].y * anchors[0].y +
                       anchors[2].z * anchors[2].z - anchors[0].z * anchors[0].z) / 2.0;
    
    solver->b_tmp[2] = (anchors[3].x * anchors[3].x - anchors[0].x * anchors[0].x +
                       anchors[3].y * anchors[3].y - anchors[0].y * anchors[0].y +
                       anchors[3].z * anchors[3].z - anchors[0].z * anchors[0].z) / 2.0;
    
    //printf("uwb_solver init success\n");
    return true;
}

Vector3d uwb_solver_solve(UWBSolver* solver, double distances[4]) {
    // Calculate b
    double b[3];
    b[0] = solver->b_tmp[0] - (distances[1] * distances[1] - distances[0] * distances[0]) / 2.0;
    b[1] = solver->b_tmp[1] - (distances[2] * distances[2] - distances[0] * distances[0]) / 2.0;
    b[2] = solver->b_tmp[2] - (distances[3] * distances[3] - distances[0] * distances[0]) / 2.0;

    // Calculate det(A)
    double detA = determinant3x3(solver->A);
    
    // Create matrices Ax, Ay, Az by replacing columns with b
    double Ax[3][3], Ay[3][3], Az[3][3];
    
    // Copy A to Ax, Ay, Az
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Ax[i][j] = solver->A[i][j];
            Ay[i][j] = solver->A[i][j];
            Az[i][j] = solver->A[i][j];
        }
    }
    
    // Replace columns with b
    for (int i = 0; i < 3; i++) {
        Ax[i][0] = b[i]; // Replace first column with b
        Ay[i][1] = b[i]; // Replace second column with b
        Az[i][2] = b[i]; // Replace third column with b
    }
    
    // Calculate determinants
    double detAx = determinant3x3(Ax);
    double detAy = determinant3x3(Ay);
    double detAz = determinant3x3(Az);
    
    // Apply Cramer's rule
    Vector3d position = {
        detAx / detA,
        detAy / detA,
        detAz / detA
    };
    
    return position;
}

/******************************************************************************
			DAM发送解析数据函数
*******************************************************************************/
void Prepare_ascii_dist_output(Cal_data_t* now_data)
{
	char dist_str[25];
	uint8_t i;
	Vector3d anchors[4] = {
        {0.0, 0.0, 160.5},   // Anchor 0
        {358.83, -2.72, 131.8},   // Anchor 1
        {337.0, 679.7, 193.0},   // Anchor 2
        {-261.0, 556.9, 173.0}    // Anchor 3
    };
	double distances[4] = {now_data->Dist[0], now_data->Dist[1], now_data->Dist[2], now_data->Dist[3]};
	UWBSolver solver;
	Vector3d position;
	memset(Tag_ouput_dist_str,0,sizeof(Tag_ouput_dist_str));
	if (uwb_solver_init(&solver, anchors, 4, 1e-6)) {
        position = uwb_solver_solve(&solver, distances);
        //sprintf(dist_str,"Calculated position: x=%f, y=%f, z=%f\n", position.x, position.y, position.z);
    }
	double pos[4];
	pos[0] = position.x / 100.0;
	pos[1] = position.y / 100.0;
	pos[2] = position.z / 100.0;
	pos[3] = 0.0;
	strcpy(Tag_ouput_dist_str,"Dist: ");
	for(i = 0;i < ANCHOR_LIST_COUNT;i++)
	{
		memset(dist_str,0,sizeof(dist_str));
		if(i != ANCHOR_LIST_COUNT - 1)
		{
			if((now_data->Cal_Flag >> i & 0x01) == 1)
			{
				sprintf(dist_str,"Anc%c: %lf  m , ",0x41 + i, pos[i]);
			}
			else
			{
				sprintf(dist_str,"Anc%c: %d  m , ",0x41 + i, -1);
			}
		}
		else
		{
			if((now_data->Cal_Flag >> i & 0x01) == 1)
			{
				sprintf(dist_str,"Anc%c: %d cm\r\n",0x41 + i, now_data->Dist[i]);
			}
			else
			{
				sprintf(dist_str,"Anc%c: %d cm\r\n",0x41 + i, -1);
		}
		}
		strcat(Tag_ouput_dist_str, dist_str);
	}
}

void Prepare_ascii_rtls_output(Cal_data_t* now_data, uint8_t mode)
{
	memset(Tag_ouput_dist_str,0,sizeof(Tag_ouput_dist_str));
	if(mode == 1)  //二维定位模式
	{
		sprintf(Tag_ouput_dist_str,"Rtls:X = %d cm , Y = %d cm %s;\r\n",now_data->x,now_data->y,(now_data->Cal_Flag >> 16 & 0x01) == 1 ? "Yes" : "No");
	}
	else if(mode == 2)  //三维定位模式
	{
		sprintf(Tag_ouput_dist_str,"Rtls:X = %d cm , Y = %d cm, Z = %d cm %s;\r\n",now_data->x, now_data->y, now_data->z, (now_data->Cal_Flag >> 16 & 0x01) == 1 ? "Yes" : "No");
	}
}




/**
 * @brief 解析标签接收到数据输出字符串信息
 * @param recv_buffer 	接收到的数据
 * @param length 		数据长度
 * @param read_idx 		偏移长度
 */
void Tag_Resolve_OutputStr(unsigned char *recv_buffer, unsigned int length, uint16_t idx)
{
	uint16_t output_protocal;
	uint16_t Location_FLAG;
	uint16_t read_idx = idx;
	
	output_protocal = recv_buffer[read_idx + 2] << 8 | recv_buffer[read_idx + 3];
	Last_cal_data_hds.Cal_Flag = recv_buffer[read_idx + 4] << 8 | recv_buffer[read_idx + 5];

	read_idx += 6;
	if(output_protocal & TAG_OUTPUT_DIST)	//测距信息
	{
		for(int i = 0;i < ANCHOR_LIST_COUNT; i++)
		{
			if((Last_cal_data_hds.Cal_Flag>>i)&0x01)
			{
				Last_cal_data_hds.Dist[i] = recv_buffer[i * 2 + read_idx] << 8 | recv_buffer[i * 2 + 1 + read_idx];
			}
			else
			{
				Last_cal_data_hds.Dist[i] = 0;
			}
		}
		read_idx += 32;
	}
	Location_FLAG = recv_buffer[read_idx] << 8 | recv_buffer[read_idx + 1];
	read_idx += 2;
	if(output_protocal & TAG_OUTPUT_RTLS) //定位信息
	{
		if(Location_FLAG != 0)
		{
			Last_cal_data_hds.x = recv_buffer[read_idx] << 8 | recv_buffer[read_idx + 1];
			Last_cal_data_hds.y = recv_buffer[read_idx + 2] << 8 | recv_buffer[read_idx + 3];
			Last_cal_data_hds.z = recv_buffer[read_idx + 4] << 8 | recv_buffer[read_idx + 5];
		}
		else
		{
			Last_cal_data_hds.x = 0;
			Last_cal_data_hds.y = 0;
			Last_cal_data_hds.z = 0;
		}
	}
	Prepare_ascii_dist_output(&Last_cal_data_hds);
	Usart1_SendString((unsigned char*)Tag_ouput_dist_str,strlen((const char*)Tag_ouput_dist_str));
}

/**
 * @brief 解析主基站接收到数据输出字符串信息
 * @param recv_buffer 	接收到的数据
 * @param length 		数据长度
 * @param read_idx 		偏移长度
 */
void Anchor_Resolve_OutputStr(unsigned char *recv_buffer,unsigned int length,uint16_t idx)
{
	uint16_t output_protocal;
	uint16_t read_idx = idx;
	
	output_protocal = recv_buffer[read_idx + 2] << 8 | recv_buffer[read_idx + 3];
	Last_cal_data_hds.Tag_ID = recv_buffer[read_idx + 4] << 8 | recv_buffer[read_idx + 5];
	//状态标志位
	Last_cal_data_hds.Cal_Flag =  (uint32_t)((uint32_t)recv_buffer[read_idx + 6] << 24 | (uint32_t)recv_buffer[read_idx + 7] << 16 
										   | (uint16_t)recv_buffer[read_idx + 8] << 8 | recv_buffer[read_idx + 9]);
	read_idx += 10;
	if(output_protocal & ANC_PROTOCAL_RTLS)  //定位数据
	{									
		if(Last_cal_data_hds.Cal_Flag & (1 << 16))  //定位解算计算成功
		{
			Last_cal_data_hds.x = recv_buffer[read_idx] << 8 | recv_buffer[read_idx + 1];
			Last_cal_data_hds.y = recv_buffer[read_idx + 2] << 8 | recv_buffer[read_idx + 3];
			Last_cal_data_hds.z = recv_buffer[read_idx + 4] << 8 | recv_buffer[read_idx + 5];
		}
		else
		{
			Last_cal_data_hds.x = 0;
			Last_cal_data_hds.y = 0;
			Last_cal_data_hds.z = 0;
		}
		read_idx += 6;
	}
	if (output_protocal & ANC_PROTOCAL_DIST)  //距离可输出
	{
		//获取距离值
		for (int i = 0; i < ANCHOR_LIST_COUNT; i++)
		{
			if((Last_cal_data_hds.Cal_Flag >> i) & 0x01) //((Cal_Flag >> i) & 0x01) == 0x01
			{
				Last_cal_data_hds.Dist[i] = recv_buffer[i * 2 + read_idx] << 8 | recv_buffer[i * 2 + 1 + read_idx];
			}
			else
			{
				Last_cal_data_hds.Dist[i] = 0;
			}
		}
		read_idx += 32;
	}
	if (output_protocal & ANC_PROTOCAL_RXDIAG)  //接收强度信息
	{
		Rx_diag.Max_noise = recv_buffer[read_idx] << 8 | recv_buffer[read_idx + 1];
		Rx_diag.Std_noise = recv_buffer[read_idx + 2] << 8 | recv_buffer[read_idx + 3];
		Rx_diag.Fp_amp1 = recv_buffer[read_idx + 4] << 8 | recv_buffer[read_idx + 5];
		Rx_diag.Fp_amp2 = recv_buffer[read_idx + 6] << 8 | recv_buffer[read_idx + 7];
		Rx_diag.Fp_amp3 = recv_buffer[read_idx + 8] << 8 | recv_buffer[read_idx + 9];
		Rx_diag.Max_growthCIR = recv_buffer[read_idx + 10] << 8 | recv_buffer[read_idx + 11];
		Rx_diag.Rx_preambleCount = recv_buffer[read_idx + 12] << 8 | recv_buffer[read_idx + 13];
		Rx_diag.Fp = recv_buffer[read_idx + 14] << 8 | recv_buffer[read_idx + 15];
		read_idx += 16;
	}
	if (output_protocal & ANC_PROTOCAL_TIMESTAMP)  //时间戳信息
	{
		 for (int j = 0; j < 6; j++)
		 {
			 Last_cal_data_hds.Time_ts[j] = (uint32_t)((uint32_t)recv_buffer[j * 4 + read_idx] << 24 | (uint32_t)recv_buffer[j * 4 + 1 + read_idx] << 16 
			 | (uint16_t)recv_buffer[j * 4 + 2 + read_idx] << 8 | (uint16_t)recv_buffer[j * 4 + 3 + read_idx]);
		 }
	}
	Prepare_ascii_dist_output(&Last_cal_data_hds);
	Usart1_SendString((unsigned char*)Tag_ouput_dist_str,strlen((const char*)Tag_ouput_dist_str));
}

/******************************************************************************
						  MODBUS接收数据处理函数
*******************************************************************************/
void MODBUS(unsigned char *recv_buffer,unsigned int length)
{
	unsigned int crc;
	
	uint16_t recv_length = 0;
	uint16_t index = 0;				//当前解析数据的位置
	if(length >= 4)
	{
		while(index < length && length >= 4)
		{
			if(recv_buffer[index] != MODBUS_ID || recv_buffer[index + 1] != 0x03)  //modbusid或功能码不正确 继续读取缓存区下一个字节的内容
			{
				index++;			//解析下一位数据
				continue;
			}
			
			recv_length = recv_buffer[index + 2] + 5; //读取寄存器长度
			if(recv_length + index > length) //接收到的数据小于寄存器长度 接受不全 重新接收
			{
				uint16_t reserve_len = length - index;	//剩余的长度
				USART1_rx.is_last_reserve = 1;	//剩余标志位
				USART1_rx.last_reserve_length = reserve_len + 1;	//剩余长度
				if(USART1_rx.r - reserve_len > 0)  //缓存区没有循环过 直接回退
				{
					USART1_rx.r -= reserve_len;
				}
				else  //缓存区发生过循环 需要处理回到真正的队首
				{
					USART1_rx.r = USART1_LEN - reserve_len + USART1_rx.r;
				}
				break;
			}
			crc = CRC_Calculate(&recv_buffer[index], recv_length - 2); //CRC校验
			if(recv_buffer[recv_length - 2 + index] != (crc / 256) || recv_buffer[recv_length - 1 + index] != (crc % 256)) //crc校验失败 丢掉这部分数据
			{
				index += recv_length;
				continue;
			}
			//crc校验成功 开始解析
			uint16_t read_idx = index + 3;	//从Modbus地址第3位开始解析
			index += recv_length;			//记录下一包数据的位置
			if(recv_buffer[read_idx] == 0xCA && recv_buffer[read_idx + 1] == 0xDA)	//A基站解析
			{
				Anchor_Resolve_OutputStr(recv_buffer,length,read_idx);
			}
			else if(recv_buffer[read_idx] == 0xAC && recv_buffer[read_idx + 1] == 0xDA)	//标签解析
			{
				Tag_Resolve_OutputStr(recv_buffer,length,read_idx);
			}
			ERROR_FLAG = 0;
		}
	}
}


