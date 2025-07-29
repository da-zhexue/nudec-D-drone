#include "LX_FC_Fun.h"
#include "LX_FC_State.h"
#include "ANO_DT_LX.h"
#include "Ano_Math.h"
#include "uart_receive.h"
/*==========================================================================
 * ����    �������ɿػ�������������
 * ����ʱ�䣺2020-03-31 
 * ����		 �������ƴ�-Jyoun
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
 * ��Ŀ������18084888982��18061373080
============================================================================
 * �����ƴ��ŶӸ�л��ҵ�֧�֣���ӭ��ҽ�Ⱥ���ཻ�������ۡ�ѧϰ��
 * �������������в��õĵط�����ӭ����ש�������
 * �������������ã�����������Ƽ���֧�����ǡ�
 * ������Դ������뻶ӭ�������á��������չ��������ϣ������ʹ��ʱ��ע��������
 * ����̹������С�˳����ݣ��������������ˮ���������ӣ�Ҳ��δ�й�Ĩ��ͬ�е���Ϊ��  
 * ��Դ���ף�����������ף�ϣ����һ������ء����ﻥ������ͬ������
 * ֻ������֧�֣������������ø��á�  
===========================================================================*/

//////////////////////////////////////////////////////////////////////
//�������ܺ���
//////////////////////////////////////////////////////////////////////
//
u8 FC_Unlock()
{
	//
	fc_sta.unlock_cmd = 1; //����
	//��Э�鷢��ָ��
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
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
	fc_sta.unlock_cmd = 0; //����
	//��Э�鷢��ָ��
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
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

//�ı�ɿ�ģʽ(ģʽ0-3)
u8 LX_Change_Mode(u8 new_mode)
{
	static u8 old_mode;
	if (old_mode != new_mode)
	{
		//
		if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
		{
			old_mode = fc_sta.fc_mode_cmd = new_mode;
			//��Э�鷢��ָ��
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
	else //�Ѿ��ڵ�ǰģʽ
	{
		return 1;
	}
}

//һ������
//��Ҫע�⣬�̿�ģʽ�²���ִ�з���
u8 OneKey_Return_Home()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//��ʱ��װ
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

//xy����(xy�ֱ𲹳�)
u8 XY_Compensate(s16 current_x, s16 target_x, s16 current_y, s16 target_y)
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
						//��3��
						compensate_step += time_dly_cnt(3000);
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
						//��3��
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

//xy����(xyһ�𲹳�)
u8 XY_Compensate_2(s16 current_x, s16 target_x, s16 current_y, s16 target_y)
{
		static u8 compensate_step = 0;
		s16 move_x_cm = target_x - current_x;
		move_x_cm = (move_x_cm >= NEUTRAL_ZONE || move_x_cm <= -NEUTRAL_ZONE) ? move_x_cm : 0;
		s16 move_y_cm = target_y - current_y;
		move_y_cm = (move_y_cm >= NEUTRAL_ZONE || move_y_cm <= -NEUTRAL_ZONE) ? move_y_cm : 0;

		switch (compensate_step)
		{
				case 0:
						compensate_step += MoveXY(move_x_cm, move_y_cm, COMPENSATE_VELOCITY);
						break;
				case 1:
						//��3��
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

//����
u8 Obstacle_Aviod(void)
{
		if(lidar_data.min_dis <= 30)
			return Horizontal_Move_delay(lidar_data.min_dis-30, 10, (lidar_data.min_ang+180)%360, 1000);
		else 
			return 1;
}

//һ�����(�߶�cm)
u8 OneKey_Takeoff(u16 height_cm)
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X05;
		dt.cmd_send.CMD[2] = BYTE0(height_cm);
		dt.cmd_send.CMD[3] = BYTE1(height_cm);
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//һ������
u8 OneKey_Land()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X06;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//ƽ��(����cm���ٶ�cmps������Ƕ�0-360��)
u8 Horizontal_Move(u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360)
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
		return 1;
	}
	else
	{
		return 0;
	}
}
//ƽ��+��ʱ(����cm���ٶ�cmps������Ƕ�0-360��, ��ʱms)
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
//ƽ��(X cm, Y cm, �ٶ�cmps)
u8 MoveXY(s16 x_cm, s16 y_cm, u16 velocity_cmps)
{
	
    REAL distance = my_sqrt(x_cm * x_cm + y_cm * y_cm);
    if (distance < 0.0001f) {
        return 0; // ��Ч����
    }
		
    REAL angle_rad = fast_atan2((REAL)y_cm, (REAL)x_cm);       // [-��, ��]
    REAL angle_deg = angle_rad * (180.0f / MY_PPPIII); // ת�Ƕ�[-180, 180]
    if (angle_deg < 0) {
        angle_deg += 360.0f;
    }
    
    u16 distance_cm = (u16)(distance + 0.5f);
    u16 dir_angle   = (u16)(angle_deg + 0.5f);

    return Horizontal_Move(distance_cm, velocity_cmps, dir_angle);
}
//ƽ��+��ʱ(����cm���ٶ�cmps������Ƕ�0-360��, ��ʱms)
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
//Ŀ��Եظ߶�(�߶�cm)
u8 Target_Height(s32 height_cm)
{
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
		return 1;
	}
	else
	{
		return 0;
	}
}
//Ŀ��Եظ߶�+��ʱ(�߶�cm, ��ʱms)
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
//�����߶�
u8 Up_Height(u16 height_cm, u16 velocity_cmps)
{
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
//�½��߶�
u8 Down_Height(u16 height_cm, u16 velocity_cmps)
{
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
//yaw��������ת(�Ƕ�deg 0-359�ȣ����ٶ�degps 5-90��/s)
u8 Yaw_Right_Rotate(u16 angle_deg, u16 angular_velocity_degps)
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
//yaw��������ת(�Ƕ�deg 0-359�ȣ����ٶ�degps 5-90��/s)
u8 Yaw_Left_Rotate(u16 angle_deg, u16 angular_velocity_degps)
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
//yaw����ת+��ʱ(����תΪ��, ����תΪ��)
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
//ˮƽУ׼
u8 Horizontal_Calibrate()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//������У׼
u8 Mag_Calibrate()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//6����ٶ�У׼
u8 ACC_Calibrate()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//������У׼
u8 GYR_Calibrate()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
