#include "LX_FC_Fun.h"
#include "LX_FC_State.h"
#include "ANO_DT_LX.h"
#include "Ano_Math.h"
#include "uart_receive.h"
/*==========================================================================
 * 描述    ：凌霄飞控基本功能主程序
 * 更新时间：2020-03-31 
 * 作者		 ：匿名科创-Jyoun
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 项目合作：18084888982，18061373080
============================================================================
 * 匿名科创团队感谢大家的支持，欢迎大家进群互相交流、讨论、学习。
 * 若您觉得匿名有不好的地方，欢迎您拍砖提意见。
 * 若您觉得匿名好，请多多帮我们推荐，支持我们。
 * 匿名开源程序代码欢迎您的引用、延伸和拓展，不过在希望您在使用时能注明出处。
 * 君子坦荡荡，小人常戚戚，匿名坚决不会请水军、请喷子，也从未有过抹黑同行的行为。  
 * 开源不易，生活更不容易，希望大家互相尊重、互帮互助，共同进步。
 * 只有您的支持，匿名才能做得更好。  
===========================================================================*/

//////////////////////////////////////////////////////////////////////
//基本功能函数
//////////////////////////////////////////////////////////////////////
//
u8 FC_Unlock()
{
	//
	fc_sta.unlock_cmd = 1; //解锁
	//按协议发送指令
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		dt.cmd_send.CID = 0x10;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x01;
		CMD_Send(0XFF, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//
u8 FC_Lock()
{
	//
	fc_sta.unlock_cmd = 0; //上锁
	//按协议发送指令
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		dt.cmd_send.CID = 0x10;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x02;
		CMD_Send(0XFF, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//改变飞控模式(模式0-3)
u8 LX_Change_Mode(u8 new_mode)
{
	static u8 old_mode;
	if (old_mode != new_mode)
	{
		//
		if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
		{
			old_mode = fc_sta.fc_mode_cmd = new_mode;
			//按协议发送指令
			dt.cmd_send.CID = 0X01;
			dt.cmd_send.CMD[0] = 0X01;
			dt.cmd_send.CMD[1] = 0X01;
			dt.cmd_send.CMD[2] = fc_sta.fc_mode_cmd;
			CMD_Send(0xff, &dt.cmd_send);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else //已经在当前模式
	{
		return 1;
	}
}

//一键返航
//需要注意，程控模式下才能执行返航
u8 OneKey_Return_Home()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X07;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//获得目标位置
void Get_target_position(u16 distance_cm, u16 dir_angle_0_360)
{
	double dx_double = (double)distance_cm * my_cos((double)dir_angle_0_360);
	double dy_double = (double)distance_cm * (-my_sin((double)dir_angle_0_360));
	target_position.x += (u16)dx_double;
	target_position.y += (u16)dy_double;
}

//延时封装
u8 time_dly_cnt(u16 delay_ms)
{
		static u16 dly_cnt = 0;
		if(dly_cnt<delay_ms)
		{
				dly_cnt+=20;//ms
		}
		else
		{
				dly_cnt = 0;
				return 1;
		}			
		return 0;
}

//xy补偿(xy分别补偿)
u8 XY_Compensate(s16 current_x, s16 target_x, s16 current_y, s16 target_y)
{
		static u8 compensate_step = 0;
		s16 move_x_cm = target_x - current_x;
		s16 move_y_cm = target_y - current_y;
		target_position.x -= move_x_cm;
		target_position.y -= move_y_cm;
		switch (compensate_step)
		{
				case 0:
						if(move_x_cm >= NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_x_cm, COMPENSATE_VELOCITY, 0);
						else if(move_x_cm <= -NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_x_cm*(-1), COMPENSATE_VELOCITY, 180);
						else 
								compensate_step=2;
						break;
				case 1:
						//等3秒
						compensate_step += time_dly_cnt(3000);
						break;
				case 2:
						if(move_y_cm >= NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_y_cm, COMPENSATE_VELOCITY, 270);
						else if(move_y_cm <= -NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_y_cm*(-1), COMPENSATE_VELOCITY, 90);
						else 
								compensate_step=4;
						break;
				case 3:
						//等3秒
						compensate_step += time_dly_cnt(3000);
						break;
				case 4:
						compensate_step = 0;
						return 1;
				default:
						compensate_step = 0;
						break;
		}
		return 0;
}

//xy补偿(xy一起补偿)
u8 XY_Compensate_2(s16 current_x, s16 target_x, s16 current_y, s16 target_y)
{
		static u8 compensate_step = 0;
		s16 move_x_cm = target_x - current_x;
		move_x_cm = (move_x_cm >= NEUTRAL_ZONE || move_x_cm <= -NEUTRAL_ZONE) ? move_x_cm : 0;
		s16 move_y_cm = target_y - current_y;
		move_y_cm = (move_y_cm >= NEUTRAL_ZONE || move_y_cm <= -NEUTRAL_ZONE) ? move_y_cm : 0;
		target_position.x -= move_x_cm;
		target_position.y -= move_y_cm;
		switch (compensate_step)
		{
				case 0:
						compensate_step += MoveXY(move_x_cm, move_y_cm, COMPENSATE_VELOCITY);
						
						break;
				case 1:
						//等3秒
						compensate_step += time_dly_cnt(3000);
						break;
				case 2:
						compensate_step = 0;
						return 1;
				default:
						compensate_step = 0;
						break;
		}
		return 0;
}
//位置补偿1
u8 Position_Compensate(void)
{
		static u8 compensate_step = 0;
		s16 current_x = position_data.x;
		s16 current_y = position_data.y;
		s16 target_x = target_position.x;
		s16 target_y = target_position.y;
		s16 move_x_cm = target_x - current_x;
		s16 move_y_cm = target_y - current_y;
		if(move_x_cm >= NEUTRAL_ZONE||move_x_cm <= -NEUTRAL_ZONE)
		{
			target_position.x -= move_x_cm;
		}
		if(move_y_cm >= NEUTRAL_ZONE ||move_y_cm <= -NEUTRAL_ZONE)
		{
			target_position.y -= move_y_cm;
		}
		switch (compensate_step)
		{
				case 0:
						if(move_x_cm >= NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_x_cm, COMPENSATE_VELOCITY, 0);
						else if(move_x_cm <= -NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_x_cm*(-1), COMPENSATE_VELOCITY, 180);
						else 
								compensate_step=2;
						break;
				case 1:
						//等3秒
						compensate_step += time_dly_cnt(3000);
						break;
				case 2:
						if(move_y_cm >= NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_y_cm, COMPENSATE_VELOCITY, 270);
						else if(move_y_cm <= -NEUTRAL_ZONE)
								compensate_step += Horizontal_Move(move_y_cm*(-1), COMPENSATE_VELOCITY, 90);
						else 
								compensate_step=4;
						break;
				case 3:
						//等3秒
						compensate_step += time_dly_cnt(3000);
						break;
				case 4:
						compensate_step = 0;
						return 1;
				default:
						compensate_step = 0;
						break;
		}
		return 0;
}
//位置补偿2
u8 Position_Compensate_2(void)
{
		static u8 compensate_step = 0;
		s16 current_x = position_data.x;
		s16 current_y = position_data.y;
		s16 target_x = target_position.x;
		s16 target_y = target_position.y;
		s16 move_x_cm = target_x - current_x;
		move_x_cm = (move_x_cm >= NEUTRAL_ZONE || move_x_cm <= -NEUTRAL_ZONE) ? move_x_cm : 0;
		s16 move_y_cm = target_y - current_y;
		move_y_cm = (move_y_cm >= NEUTRAL_ZONE || move_y_cm <= -NEUTRAL_ZONE) ? move_y_cm : 0;
		target_position.x -= move_x_cm;
		target_position.y -= move_y_cm;
		switch (compensate_step)
		{
				case 0:
						compensate_step += MoveXY(move_x_cm, move_y_cm, COMPENSATE_VELOCITY);
						
						break;
				case 1:
						//等3秒
						compensate_step += time_dly_cnt(3000);
						break;
				case 2:
						compensate_step = 0;
						return 1;
				default:
						compensate_step = 0;
						break;
		}
		return 0;
}
//避障
u8 Obstacle_Aviod(void)
{
	static u8 avoid_step = 0;
	switch (avoid_step)
	{
		case 0:
			if(lidar_data.min_dis <= 30)
			{
				u16 distance_cm = 30-lidar_data.min_dis;
				u16 dir_angle_0_360 = (lidar_data.min_ang+180)%360;
				double dx_double = (double)distance_cm * my_cos((double)dir_angle_0_360);
				double dy_double = (double)distance_cm * my_sin((double)dir_angle_0_360);
				target_position.x -= (u16)dx_double;
				target_position.y -= (u16)dy_double;
				avoid_step += Horizontal_Move(distance_cm, 10, dir_angle_0_360);
			}
			else 
				return 1;
			break;
		case 1:
			avoid_step += time_dly_cnt(3000);
			break;
		case 2:
			avoid_step = 0;
			return 1;
		default:
			avoid_step = 0;
			break;
	}
	return 0;
}

//一键起飞(高度cm)
u8 OneKey_Takeoff(u16 height_cm)
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X05;
		dt.cmd_send.CMD[2] = BYTE0(height_cm);
		dt.cmd_send.CMD[3] = BYTE1(height_cm);
		CMD_Send(0xff, &dt.cmd_send);
		target_position.h = height_cm;
		return 1;
	}
	else
	{
		return 0;
	}
}
//一键降落
u8 OneKey_Land()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X06;
		CMD_Send(0xff, &dt.cmd_send);
		target_position.h = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
//平移(距离cm，速度cmps，方向角度0-360度)
u8 Horizontal_Move(u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360)
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X03;
		//
		dt.cmd_send.CMD[2] = BYTE0(distance_cm);
		dt.cmd_send.CMD[3] = BYTE1(distance_cm);
		dt.cmd_send.CMD[4] = BYTE0(velocity_cmps);
		dt.cmd_send.CMD[5] = BYTE1(velocity_cmps);
		dt.cmd_send.CMD[6] = BYTE0(dir_angle_0_360);
		dt.cmd_send.CMD[7] = BYTE1(dir_angle_0_360);
		//
		CMD_Send(0xff, &dt.cmd_send);
		Get_target_position(distance_cm, dir_angle_0_360);
		return 1;
	}
	else
	{
		return 0;
	}
}
//平移+延时(距离cm，速度cmps，方向角度0-360度, 延时ms)
u8 Horizontal_Move_delay(u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360, u16 delay_ms)
{
	static u8 move_step = 0;
	switch(move_step)
	{
		case 0:
			move_step += Horizontal_Move(distance_cm, velocity_cmps, dir_angle_0_360);
			break;
		case 1:
			move_step += time_dly_cnt(delay_ms);
			break;
		case 2:
			move_step = 0;
			return 1;
		default:
			move_step = 0;
			break;
	}
	return 0;
}
//平移(X cm, Y cm, 速度cmps)
u8 MoveXY(s16 x_cm, s16 y_cm, u16 velocity_cmps)
{
	
    REAL distance = my_sqrt(x_cm * x_cm + y_cm * y_cm);
    if (distance < 0.0001f) {
        return 0; // 无效坐标
    }
		
    REAL angle_rad = fast_atan2((REAL)y_cm, (REAL)x_cm);       // [-π, π]
    REAL angle_deg = angle_rad * (180.0f / MY_PPPIII); // 转角度[-180, 180]
    if (angle_deg < 0) {
        angle_deg += 360.0f;
    }
    
    u16 distance_cm = (u16)(distance + 0.5f);
    u16 dir_angle   = (u16)(angle_deg + 0.5f);
		
    return Horizontal_Move(distance_cm, velocity_cmps, dir_angle);
}
//平移+延时(距离cm，速度cmps，方向角度0-360度, 延时ms)
u8 Move_XY_delay(s16 x_cm, s16 y_cm, u16 velocity_cmps, u16 delay_ms)
{
	static u8 move_step = 0;
	switch(move_step)
	{
		case 0:
			move_step += MoveXY(x_cm, y_cm, velocity_cmps);
			break;
		case 1:
			move_step += time_dly_cnt(delay_ms);
			break;
		case 2:
			move_step = 0;
			return 1;
		default:
			move_step = 0;
			break;
	}
	return 0;
}
//目标对地高度(高度cm)
u8 Target_Height(s32 height_cm)
{
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X01;
		dt.cmd_send.CMD[1] = 0X02;
		//
		dt.cmd_send.CMD[2] = BYTE0(height_cm);
		dt.cmd_send.CMD[3] = BYTE1(height_cm);
		dt.cmd_send.CMD[4] = BYTE2(height_cm);
		dt.cmd_send.CMD[5] = BYTE3(height_cm);
		//
		CMD_Send(0xff, &dt.cmd_send);
		target_position.h = height_cm;
		return 1;
	}
	else
	{
		return 0;
	}
}
//目标对地高度+延时(高度cm, 延时ms)
u8 Target_Height_delay(s32 height_cm, u16 delay_ms)
{
	static u8 move_step = 0;
	switch(move_step)
	{
		case 0:
			move_step += Target_Height(height_cm);
			break;
		case 1:
			move_step += time_dly_cnt(delay_ms);
			break;
		case 2:
			move_step = 0;
			return 1;
		default:
			move_step = 0;
			break;
	}
	return 0;
}
//上升高度
u8 Up_Height(u16 height_cm, u16 velocity_cmps)
{
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X01;
		//
		dt.cmd_send.CMD[2] = BYTE0(height_cm);
		dt.cmd_send.CMD[3] = BYTE1(height_cm);
		dt.cmd_send.CMD[4] = BYTE0(velocity_cmps);
		dt.cmd_send.CMD[5] = BYTE1(velocity_cmps);
		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//下降高度
u8 Down_Height(u16 height_cm, u16 velocity_cmps)
{
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X02;
		//
		dt.cmd_send.CMD[2] = BYTE0(height_cm);
		dt.cmd_send.CMD[3] = BYTE1(height_cm);
		dt.cmd_send.CMD[4] = BYTE0(velocity_cmps);
		dt.cmd_send.CMD[5] = BYTE1(velocity_cmps);
		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//yaw轴向右旋转(角度deg 0-359度，角速度degps 5-90度/s)
u8 Yaw_Right_Rotate(u16 angle_deg, u16 angular_velocity_degps)
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X08;
		//
		dt.cmd_send.CMD[2] = BYTE0(angle_deg);
		dt.cmd_send.CMD[3] = BYTE1(angle_deg);
		dt.cmd_send.CMD[4] = BYTE0(angular_velocity_degps);
		dt.cmd_send.CMD[5] = BYTE1(angular_velocity_degps);
		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//yaw轴向左旋转(角度deg 0-359度，角速度degps 5-90度/s)
u8 Yaw_Left_Rotate(u16 angle_deg, u16 angular_velocity_degps)
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X07;
		//
		dt.cmd_send.CMD[2] = BYTE0(angle_deg);
		dt.cmd_send.CMD[3] = BYTE1(angle_deg);
		dt.cmd_send.CMD[4] = BYTE0(angular_velocity_degps);
		dt.cmd_send.CMD[5] = BYTE1(angular_velocity_degps);
		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//yaw轴旋转+延时(向右转为正, 向左转为负)
u8 Yaw_Rotate_delay(s16 angle_deg, u16 angular_velocity_degps, u16 delay_ms)
{
	static u8 move_step = 0;
	switch(move_step)
	{
		case 0:
			if(angle_deg > 0)
				move_step += Yaw_Right_Rotate(angle_deg, angular_velocity_degps);
			else 
				move_step += Yaw_Left_Rotate(angle_deg * (-1), angular_velocity_degps);
			break;
		case 1:
			move_step += time_dly_cnt(delay_ms);
			break;
		case 2:
			move_step = 0;
			return 1;
		default:
			move_step = 0;
			break;
	}
	return 0;
}
//水平校准
u8 Horizontal_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X03;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//磁力计校准
u8 Mag_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X04;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//6面加速度校准
u8 ACC_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X05;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//陀螺仪校准
u8 GYR_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X02;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
