///**			                                                    
//		   ____                    _____ _______ _____       XTARK@塔克创新
//		  / __ \                  / ____|__   __|  __ \ 
//		 | |  | |_ __   ___ _ __ | |       | |  | |__) |
//		 | |  | | '_ \ / _ \ '_ \| |       | |  |  _  / 
//		 | |__| | |_) |  __/ | | | |____   | |  | | \ \ 
//		  \____/| .__/ \___|_| |_|\_____|  |_|  |_|  \_\
//				| |                                     
//				|_|                OpenCTR   机器人控制器
//									 
//  ****************************************************************************** 
//  *           
//  * 版权所有： XTARK@塔克创新  版权所有，盗版必究
//  * 公司网站： www.xtark.cn   www.tarkbot.com
//  * 淘宝店铺： https://xtark.taobao.com  
//  * 塔克微信： 塔克创新（关注公众号，获取最新更新资讯）
//  *      
//  ******************************************************************************
//  * @作  者  塔克创新团队
//  * @内  容  国科光芯D6雷达处理
//  *
//  ******************************************************************************
//  */

///* Define to prevent recursive inclusion -------------------------------------*/
//#ifndef __AX_LASER_H
//#define __AX_LASER_H

///* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include <stdio.h>
////雷达接收数据帧相关定义
//#define LD_HEADER1    0xAA     //帧头1
//#define LD_HEADER2    0x55     //帧头2
//#define LD_F_LEN        85     //帧长度
//#define LD_PNAM         25     //一帧测量点数量

////雷达点结构体定义
//typedef struct
//{
//	float       angle;         //角度
//	uint16_t    distance;      //距离
//	uint8_t     intensity;     //光强

//}LidarPointTypeDef;
//typedef struct {
//    float angle;    // 角度(度)
//    float range;    // 距离(毫米)
//} Point;
//typedef struct {
//    Point points[360];  // 360个点(每度一个点)
//    int size;           // 点数(固定为360)
//} LaserScan;

////雷达输出有效数据，其中序列为角度
//extern uint16_t ax_lidar_data[360];

//int fputc(int ch, FILE *f);
//////接口函数
//void AX_LIDAR_Start(void);    //雷达启动旋转
//void LD_DataHandle(void);  //数据解析
//void AX_LIDAR_Stop(void);     //雷达停止旋转

//#endif 

///******************* (C) 版权 2025 XTARK **************************************/
