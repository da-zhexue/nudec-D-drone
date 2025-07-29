#include "User_Task.h"
#include "Drv_RcIn.h"
#include "LX_FC_Fun.h"
#include "uart_receive.h"

void UserTask_OneKeyCmd(void)
{
    //////////////////////////////////////////////////////////////////////
    //一键起飞/降落例程
    //////////////////////////////////////////////////////////////////////
    //用静态变量记录一键起飞/降落指令已经执行。
    static u8 one_key_takeoff_f = 1, one_key_land_f = 1, one_key_mission_f = 0;
    static u8 mission_step;
    //判断有遥控信号才执行
    if (rc_in.no_signal == 0)
    {
        //判断第6通道拨杆位置 1300<CH_6<1700
        if (rc_in.rc_ch.st_data.ch_[ch_5_aux1] > 1300 && rc_in.rc_ch.st_data.ch_[ch_5_aux1] < 1700)
        {
            //还没有执行
            if (one_key_takeoff_f == 0)
            {
                //标记已经执行
                one_key_takeoff_f =
                    //执行一键起飞
                    OneKey_Takeoff(100); //参数单位：厘米； 0：默认上位机设置的高度。
            }
        }
        else
        {
            //复位标记，以便再次执行
            one_key_takeoff_f = 0;
        }
        //
        //判断第6通道拨杆位置 800<CH_6<1200
        if (rc_in.rc_ch.st_data.ch_[ch_5_aux1] > 800 && rc_in.rc_ch.st_data.ch_[ch_5_aux1] < 1200)
        {
            //还没有执行
            if (one_key_land_f == 0)
            {
                //标记已经执行
                one_key_land_f =
                    //执行一键降落
                    OneKey_Land();
            }
        }
        else
        {
            //复位标记，以便再次执行
            one_key_land_f = 0;
        }
	//判断第6通道拨杆位置 1700<CH_6<2200 
		if(rc_in.rc_ch.st_data.ch_[ch_5_aux1]>1700 && rc_in.rc_ch.st_data.ch_[ch_5_aux1]<2200)
		{
			//还没有执行
			if(one_key_mission_f ==0)
			{
				//标记已经执行
				one_key_mission_f = 1;
				//开始流程
				mission_step = 1;
			}
		}
		else
		{
			//复位标记，以便再次执行
			one_key_mission_f = 0;		
		}
		//
		if(one_key_mission_f==1)
		{
			static u16 time_dly_cnt_ms;
			//
			switch(mission_step)
			{
				case 0:
				{
					//reset
					time_dly_cnt_ms = 0;
				}
				break;
				case 1:
				{
					//切换程控模式
					mission_step += LX_Change_Mode(3);
				}
				break;
				case 2:
				{
					//解锁
					mission_step += FC_Unlock();
				}
				break;
				case 3:
				{
					//等2秒
					if(time_dly_cnt_ms<2000)
					{
						time_dly_cnt_ms+=20;//ms
					}
					else
					{
						time_dly_cnt_ms = 0;
						mission_step += 1;
					}
				}
				break;
				case 4:
				{
					//起飞
					mission_step += OneKey_Takeoff(150);//参数单位：厘米； 0：默认上位机设置的高度。
				}
				break;
				case 5:
				{
//					//等10秒
//					if(time_dly_cnt_ms<10000)
//					{
//						time_dly_cnt_ms+=20;//ms
//					}
//					else
//					{
//						time_dly_cnt_ms = 0;
//						mission_step += 1;
//					}	
                   mission_step+=time_dly_cnt(10000);					
				}
				break;
				case 6:
				{
					//左移2米
					mission_step += Horizontal_Move(250, 50, 270);
				}
				break;	
				case 7:
				{
					 mission_step+=time_dly_cnt(8000);
				}
				break;
				case 8:
				{
					//前移3米
					mission_step += Horizontal_Move(350, 50, 0);
				}
				break;
				case 9: 
				{
					//等10秒
					 mission_step+=time_dly_cnt(10000);	
				}
				break;
				case 10:
				{   s16 move_y = circle_data.y - 60;
					s16 move_x = circle_data.x - 80;
					mission_step+=XY_Compensate_2(0,move_x,0,move_y);
				}
				break;
				case 11:
				{
					//执行一键降落
					OneKey_Land();	
				}
				break;				
				default:break;
			}
		}
		else
		{
			mission_step = 0;
		}
	}
    ////////////////////////////////////////////////////////////////////////
}
