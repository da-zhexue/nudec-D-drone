/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_table;
	lv_obj_t *screen_slider;
	lv_obj_t *screen_Reset;
	lv_obj_t *screen_Reset_label;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_input;
	lv_obj_t *screen_Confirm;
	lv_obj_t *screen_Confirm_label;
	lv_obj_t *screen_output;
	lv_obj_t *screen_next;
	lv_obj_t *screen_next_label;
	lv_obj_t *screen_1;
	bool screen_1_del;
	lv_obj_t *screen_1_img_1;
	lv_obj_t *screen_1_output2;
	lv_obj_t *screen_1_input2;
	lv_obj_t *screen_1_confirm2;
	lv_obj_t *screen_1_confirm2_label;
	lv_obj_t *screen_1_back;
	lv_obj_t *screen_1_back_label;
	lv_obj_t *screen_1_line_1_2;
	lv_obj_t *screen_1_line_2_2;
	lv_obj_t *screen_1_line_3_2;
	lv_obj_t *screen_1_line_4_2;
	lv_obj_t *screen_1_line_1_1;
	lv_obj_t *screen_1_line_2_1;
	lv_obj_t *screen_1_line_3_1;
	lv_obj_t *screen_1_line_4_1;
	lv_obj_t *screen_1_line_1_3;
	lv_obj_t *screen_1_line_3_3;
	lv_obj_t *screen_1_line_2_3;
	lv_obj_t *screen_1_line_4_3;
	lv_obj_t *g_kb_top_layer;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
void setup_scr_screen_1(lv_ui *ui);
LV_IMG_DECLARE(_field_alpha_204x149);

LV_FONT_DECLARE(lv_font_montserratMedium_11)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_23)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18)

void esp_ap_init();
#ifdef __cplusplus
}
#endif
#endif
