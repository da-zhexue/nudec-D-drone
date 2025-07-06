/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lvgl.h"
#include "custom.h"
#include "usart.h"
/*********************
 *      DEFINES
 *********************/
char num_data[4][6][4]={{"0","0","0","0","0","0"},
                        {"0","0","0","0","0","0"},
                        {"0","0","0","0","0","0"},
                        {"0","0","0","0","0","0"}};
int num_index=1;
uint8_t send_data[5]={0,0,0,0};
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
void slider_event_cb(int slider_val, lv_obj_t *table) {
    char buf[4];
    buf[2]=buf[3]=0;
    for(uint8_t lie = 0; lie < 2; lie++)
    {
        buf[0]='A'+slider_val*2+lie;
        for(uint8_t row = 0; row < 6; row++) {
            buf[1]=row+1+'0';
            lv_table_set_cell_value(table, row+1, lie*2, buf);
            lv_table_set_cell_value(table, row+1, 1+lie*2, num_data[slider_val*2+lie][row]);
        } 
    }
          
}
void num_reset(lv_obj_t *table)
{
    char str[4] = "0";
    for(int i=0; i<4; i++)
        for(int j=0; j<6; j++)
        {
            num_data[i][j][0] = '0';
            num_data[i][j][1] = 0;
            lv_table_set_cell_value(table, j+1, 1, str);
        }
            
}
void search_position(const char *input,int show_position)
{
    char output[5] = "NULL";
    int first_letter = -1, second_letter = -1;
    for(int i=0;i<4;i++)
        for(int j=0;j<6;j++)
            if(num_data[i][j][0] == input[0] && num_data[i][j][1] == input[1])
            {
                output[2]=output[3]=0;
                output[0]='A'+i;
                output[1]='0'+j+1;
                first_letter=i;
                second_letter=j+1;
                break;
            }
    if(show_position == 1)
    {
        //lv_table_set_cell_value(guider_ui.screen_1_output2, 0, 0, output);
        lv_obj_t *lines[4][3] = {
                                 {guider_ui.screen_1_line_1_1, guider_ui.screen_1_line_1_2,guider_ui.screen_1_line_1_3},
                                 {guider_ui.screen_1_line_2_1, guider_ui.screen_1_line_2_2,guider_ui.screen_1_line_2_3},
                                 {guider_ui.screen_1_line_3_1, guider_ui.screen_1_line_3_2,guider_ui.screen_1_line_3_3},
                                 {guider_ui.screen_1_line_4_1, guider_ui.screen_1_line_4_2,guider_ui.screen_1_line_4_3}
                                };
        if(first_letter>=0)
            for(int i=0;i<3;i++)
                lv_obj_clear_flag(lines[first_letter][i],LV_OBJ_FLAG_HIDDEN);
				send_data[0] = output[0];
				send_data[1] = output[1];
				send_data[2] = '\r';
				send_data[3] = '\n';
    }
    else if(show_position == 0)
        lv_table_set_cell_value(guider_ui.screen_output, 0, 0, output);
		else if(show_position == 2 && output[0] == send_data[0] && output[1] == send_data[1])
		{
				lv_table_set_cell_value(guider_ui.screen_1_input2, 0, 0, input);
				lv_table_set_cell_value(guider_ui.screen_1_output2, 0, 0, output);
		}
}
void send_position()
{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
		uint8_t at0[] = "AT+CIPSEND=0,4\r\n";
		const uint8_t* send_data_uint = send_data;
		for(int i=0;i<3;i++)
		{
			HAL_UART_Transmit(&huart3,at0,sizeof(at0),1000);
			int tt=0;
			for(volatile int i=0;i<10000;i++)
					tt++;
			HAL_UART_Transmit(&huart3,send_data_uint,sizeof(send_data_uint),100);
			tt=0;
			for(volatile int i=0;i<10000;i++)
					tt++;
		}
}
void renovate_the_num(const char *num,lv_obj_t *table)
{
	if(num_index <= 24)
	{
		int num_x = (num_index-1)/6;
		int num_y = (num_index-1)%6;
		num_data[num_x][num_y][0]=num[0];
		num_data[num_x][num_y][1]=num[1];
		slider_event_cb(num_x/2,table);
		num_index++;
	}
	else if(num_index == 25)
	{
		num_index++;
		search_position(num,1);
	}
	else if(num_index == 26)
	{
		search_position(num,2);
	}
}
/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
}

