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
    buf[0]='A'+slider_val;
    buf[2]=buf[3]=0;
    for(uint8_t row = 0; row < 6; row++) {
        buf[1]=row+1+'0';
        lv_table_set_cell_value(table, row+1, 0, buf);
        lv_table_set_cell_value(table, row+1, 1, num_data[slider_val][row]);
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
    num_index = 0;      
}
void search_for_position(const char *input)
{
    char output[5] = "NULL";
    for(int i=0;i<4;i++)
        for(int j=0;j<6;j++)
            if(num_data[i][j][0] == input[0] && num_data[i][j][1] == input[1])
            {
                output[2]=output[3]=0;
                output[0]='A'+i;
                output[1]='0'+j+1;
                break;
            }
    lv_table_set_cell_value(guider_ui.screen_output, 0, 0, output);
}
void show_position(const char *input)
{
    char output2[5] = "NULL";
    for(int i=0;i<4;i++)
        for(int j=0;j<6;j++)
            if(num_data[i][j][0] == input[0] && num_data[i][j][1] == input[1])
            {
                output2[2]=output2[3]=0;
                output2[0]='A'+i;
                output2[1]='0'+j+1;
                break;
            }
    lv_table_set_cell_value(guider_ui.screen_1_output2, 0, 0, output2);
		uint8_t at7[] = "AT+CIPSEND=0,4\r\n";
		HAL_UART_Transmit(&huart3,at7,sizeof(at7),100);
		HAL_Delay(100);				
		HAL_UART_Transmit(&huart3,(uint8_t*)output2,sizeof(output2),100);
    switch(output2[0])
    {
        case 'A':
            lv_obj_clear_flag(guider_ui.screen_1_line_1_1,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_1_line_1_2,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_1_line_1_3,LV_OBJ_FLAG_HIDDEN);
            break;
        case 'B':
            lv_obj_clear_flag(guider_ui.screen_1_line_2_1,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_1_line_2_2,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_1_line_2_3,LV_OBJ_FLAG_HIDDEN);
            break;
        case 'C':
            lv_obj_clear_flag(guider_ui.screen_1_line_3_1,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_1_line_3_2,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_1_line_3_3,LV_OBJ_FLAG_HIDDEN);
            break;
        case 'D':
            lv_obj_clear_flag(guider_ui.screen_1_line_4_1,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_1_line_4_2,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_1_line_4_3,LV_OBJ_FLAG_HIDDEN);
            break;
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
		slider_event_cb(num_x,table);
		num_index++;
	}
}
/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
}

