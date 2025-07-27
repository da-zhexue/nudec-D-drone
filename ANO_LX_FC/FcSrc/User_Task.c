#include "User_Task.h"
#include "Drv_RcIn.h"
#include "LX_FC_Fun.h"
#include "uart_receive.h"

void UserTask_OneKeyCmd(void)
{
    //////////////////////////////////////////////////////////////////////
    //һ�����/��������
    //////////////////////////////////////////////////////////////////////
    //�þ�̬������¼һ�����/����ָ���Ѿ�ִ�С�
    static u8 one_key_takeoff_f = 1, one_key_land_f = 1, one_key_mission_f = 0;
    static u8 mission_step;
    //�ж���ң���źŲ�ִ��
    if (rc_in.no_signal == 0)
    {
        //�жϵ�6ͨ������λ�� 1300<CH_6<1700
        if (rc_in.rc_ch.st_data.ch_[ch_5_aux1] > 1300 && rc_in.rc_ch.st_data.ch_[ch_5_aux1] < 1700)
        {
            //��û��ִ��
            if (one_key_takeoff_f == 0)
            {
                //����Ѿ�ִ��
                one_key_takeoff_f =
                    //ִ��һ�����
                    OneKey_Takeoff(100); //������λ�����ף� 0��Ĭ����λ�����õĸ߶ȡ�
            }
        }
        else
        {
            //��λ��ǣ��Ա��ٴ�ִ��
            one_key_takeoff_f = 0;
        }
        //
        //�жϵ�6ͨ������λ�� 800<CH_6<1200
        if (rc_in.rc_ch.st_data.ch_[ch_5_aux1] > 800 && rc_in.rc_ch.st_data.ch_[ch_5_aux1] < 1200)
        {
            //��û��ִ��
            if (one_key_land_f == 0)
            {
                //����Ѿ�ִ��
                one_key_land_f =
                    //ִ��һ������
                    OneKey_Land();
            }
        }
        else
        {
            //��λ��ǣ��Ա��ٴ�ִ��
            one_key_land_f = 0;
        }
	//�жϵ�6ͨ������λ�� 1700<CH_6<2200 
		if(rc_in.rc_ch.st_data.ch_[ch_5_aux1]>1700 && rc_in.rc_ch.st_data.ch_[ch_5_aux1]<2200)
		{
			//��û��ִ��
			if(one_key_mission_f ==0)
			{
				//����Ѿ�ִ��
				one_key_mission_f = 1;
				//��ʼ����
				mission_step = 1;
			}
		}
		else
		{
			//��λ��ǣ��Ա��ٴ�ִ��
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
					//�л��̿�ģʽ
					mission_step += LX_Change_Mode(3);
				}
				break;
				case 2:
				{
					//����
					mission_step += FC_Unlock();
				}
				break;
				case 3:
				{
					//��2��
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
					//���
					mission_step += OneKey_Takeoff(150);//������λ�����ף� 0��Ĭ����λ�����õĸ߶ȡ�
				}
				break;
				case 5:
				{
					//��10��
					if(time_dly_cnt_ms<10000)
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
				case 6:
				{
					//����2��
					mission_step += Horizontal_Move(250, 50, 270);
				}
				break;	
				case 7:
				{
					//��10��
					if(time_dly_cnt_ms<8000)
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
				case 8:
				{
					//ǰ��3��
					mission_step += Horizontal_Move(350, 50, 0);
				}
				break;
				case 9:
				{
					//��10��
					if(time_dly_cnt_ms<10000)
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
				case 10:
				{
					s16 move_x = circle_data.x - 80;
					if(move_x >= 5)
						mission_step += Horizontal_Move(move_x, 15, 90);
					else if(move_x <= -5)
						mission_step += Horizontal_Move(move_x*(-1), 15, 270);
					else 
						mission_step++;
				}
				break;	
				case 11:
				{
					//��3��
					if(time_dly_cnt_ms<4000)
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
				case 12:
				{
					s16 move_y = circle_data.y - 60;
					if(move_y >= 5)
						mission_step += Horizontal_Move(move_y, 15, 180);
					else if(move_y <= -5)
						mission_step += Horizontal_Move(move_y*(-1), 15, 0);
					else 
						mission_step++;
				}
				break;
				case 13:
				{
					//��3��
					if(time_dly_cnt_ms<4000)
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
				case 14:
				{
					//ִ��һ������
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
