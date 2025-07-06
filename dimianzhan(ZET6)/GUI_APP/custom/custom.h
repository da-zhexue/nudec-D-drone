/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"
//char num_data[4][6][4];
void custom_init(lv_ui *ui);
void slider_event_cb(int slider_val, lv_obj_t *table);
void num_reset();
void search_position(const char *input,int show_position);
void send_position();
void renovate_the_num(const char *num,lv_obj_t *table);
#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
