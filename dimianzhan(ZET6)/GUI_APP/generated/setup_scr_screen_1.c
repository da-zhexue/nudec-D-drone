/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_1(lv_ui *ui)
{
    //Write codes screen_1
    ui->screen_1 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_1
    ui->screen_1_img_1 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_1, &_field_alpha_204x149);
    lv_img_set_pivot(ui->screen_1_img_1, 50,50);
    lv_img_set_angle(ui->screen_1_img_1, 0);
    lv_obj_set_pos(ui->screen_1_img_1, 59, 5);
    lv_obj_set_size(ui->screen_1_img_1, 204, 149);

    //Write style for screen_1_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_output2
    ui->screen_1_output2 = lv_table_create(ui->screen_1);
    lv_table_set_col_cnt(ui->screen_1_output2,1);
    lv_table_set_row_cnt(ui->screen_1_output2,1);
    lv_table_set_cell_value(ui->screen_1_output2,0,0,"OUTPUT");
    lv_obj_set_pos(ui->screen_1_output2, 92, 198);
    lv_obj_set_scrollbar_mode(ui->screen_1_output2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_output2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->screen_1_output2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_output2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_output2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_output2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_output2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_output2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_output2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_output2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_output2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_output2, lv_color_hex(0xd5dee6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_output2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_output2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_output2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_1_output2, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_output2, lv_color_hex(0x393c41), LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_output2, &lv_font_montserratMedium_12, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_output2, 255, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_output2, LV_TEXT_ALIGN_CENTER, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_output2, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_output2, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_output2, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_output2, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_output2, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_output2, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);

    //Write codes screen_1_input2
    ui->screen_1_input2 = lv_dropdown_create(ui->screen_1);
    lv_dropdown_set_options(ui->screen_1_input2, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24");
    lv_obj_set_pos(ui->screen_1_input2, 109, 162);
    lv_obj_set_size(ui->screen_1_input2, 39, 27);

    //Write style for screen_1_input2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_input2, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_input2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_input2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_input2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_input2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_input2, lv_color_hex(0xe1e6ee), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_input2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_input2, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_input2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_input2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_input2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_input2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_input2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_input2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_input2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_screen_1_input2_extra_list_selected_checked
    static lv_style_t style_screen_1_input2_extra_list_selected_checked;
    ui_init_style(&style_screen_1_input2_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_1_input2_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_screen_1_input2_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_1_input2_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_1_input2_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_1_input2_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_screen_1_input2_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_1_input2_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_1_input2_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_1_input2), &style_screen_1_input2_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_input2_extra_list_main_default
    static lv_style_t style_screen_1_input2_extra_list_main_default;
    ui_init_style(&style_screen_1_input2_extra_list_main_default);

    lv_style_set_max_height(&style_screen_1_input2_extra_list_main_default, 90);
    lv_style_set_text_color(&style_screen_1_input2_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_1_input2_extra_list_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_1_input2_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_1_input2_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_screen_1_input2_extra_list_main_default, 255);
    lv_style_set_border_color(&style_screen_1_input2_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_screen_1_input2_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_1_input2_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_1_input2_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_1_input2_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_1_input2_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_1_input2), &style_screen_1_input2_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_input2_extra_list_scrollbar_default
    static lv_style_t style_screen_1_input2_extra_list_scrollbar_default;
    ui_init_style(&style_screen_1_input2_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_1_input2_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_1_input2_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_1_input2_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_screen_1_input2_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_1_input2), &style_screen_1_input2_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_1_confirm2
    ui->screen_1_confirm2 = lv_btn_create(ui->screen_1);
    ui->screen_1_confirm2_label = lv_label_create(ui->screen_1_confirm2);
    lv_label_set_text(ui->screen_1_confirm2_label, "OK\n");
    lv_label_set_long_mode(ui->screen_1_confirm2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_confirm2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_confirm2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_confirm2_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_confirm2, 172, 162);
    lv_obj_set_size(ui->screen_1_confirm2, 40, 27);

    //Write style for screen_1_confirm2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_confirm2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_confirm2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_confirm2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_confirm2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_confirm2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_confirm2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_confirm2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_confirm2, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_confirm2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_confirm2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_back
    ui->screen_1_back = lv_btn_create(ui->screen_1);
    ui->screen_1_back_label = lv_label_create(ui->screen_1_back);
    lv_label_set_text(ui->screen_1_back_label, "BACK");
    lv_label_set_long_mode(ui->screen_1_back_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_back_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_back, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_1_back_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_1_back, 260, 206);
    lv_obj_set_size(ui->screen_1_back, 52, 30);

    //Write style for screen_1_back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_back, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_back, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_back, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_back, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_back, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_back, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_1_2
    ui->screen_1_line_1_2 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_1_2[] = {{0, 0},{0, 90},};
    lv_line_set_points(ui->screen_1_line_1_2, screen_1_line_1_2, 2);
    lv_obj_set_pos(ui->screen_1_line_1_2, 100, 35);
    lv_obj_set_size(ui->screen_1_line_1_2, 1, 90);
    lv_obj_add_flag(ui->screen_1_line_1_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_1_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_1_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_1_2, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_1_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_1_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_2_2
    ui->screen_1_line_2_2 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_2_2[] = {{0, 0},{0, 90},};
    lv_line_set_points(ui->screen_1_line_2_2, screen_1_line_2_2, 2);
    lv_obj_set_pos(ui->screen_1_line_2_2, 140, 35);
    lv_obj_set_size(ui->screen_1_line_2_2, 1, 90);
    lv_obj_add_flag(ui->screen_1_line_2_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_2_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_2_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_2_2, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_2_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_2_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_3_2
    ui->screen_1_line_3_2 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_3_2[] = {{0, 0},{0, 90},};
    lv_line_set_points(ui->screen_1_line_3_2, screen_1_line_3_2, 2);
    lv_obj_set_pos(ui->screen_1_line_3_2, 181, 35);
    lv_obj_set_size(ui->screen_1_line_3_2, 1, 90);
    lv_obj_add_flag(ui->screen_1_line_3_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_3_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_3_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_3_2, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_3_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_3_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_4_2
    ui->screen_1_line_4_2 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_4_2[] = {{0, 0},{0, 90},};
    lv_line_set_points(ui->screen_1_line_4_2, screen_1_line_4_2, 2);
    lv_obj_set_pos(ui->screen_1_line_4_2, 220, 35);
    lv_obj_set_size(ui->screen_1_line_4_2, 1, 90);
    lv_obj_add_flag(ui->screen_1_line_4_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_4_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_4_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_4_2, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_4_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_4_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_1_1
    ui->screen_1_line_1_1 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_1_1[] = {{0, 0},{10, 0},};
    lv_line_set_points(ui->screen_1_line_1_1, screen_1_line_1_1, 2);
    lv_obj_set_pos(ui->screen_1_line_1_1, 90, 125);
    lv_obj_set_size(ui->screen_1_line_1_1, 10, 1);
    lv_obj_add_flag(ui->screen_1_line_1_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_1_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_1_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_1_1, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_1_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_1_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_2_1
    ui->screen_1_line_2_1 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_2_1[] = {{0, 0},{48, 0},};
    lv_line_set_points(ui->screen_1_line_2_1, screen_1_line_2_1, 2);
    lv_obj_set_pos(ui->screen_1_line_2_1, 90, 125);
    lv_obj_set_size(ui->screen_1_line_2_1, 48, 1);
    lv_obj_add_flag(ui->screen_1_line_2_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_2_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_2_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_2_1, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_2_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_2_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_3_1
    ui->screen_1_line_3_1 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_3_1[] = {{0, 0},{91, 0},};
    lv_line_set_points(ui->screen_1_line_3_1, screen_1_line_3_1, 2);
    lv_obj_set_pos(ui->screen_1_line_3_1, 90, 125);
    lv_obj_set_size(ui->screen_1_line_3_1, 91, 1);
    lv_obj_add_flag(ui->screen_1_line_3_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_3_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_3_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_3_1, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_3_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_3_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_4_1
    ui->screen_1_line_4_1 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_4_1[] = {{0, 0},{130, 0},};
    lv_line_set_points(ui->screen_1_line_4_1, screen_1_line_4_1, 2);
    lv_obj_set_pos(ui->screen_1_line_4_1, 90, 125);
    lv_obj_set_size(ui->screen_1_line_4_1, 130, 1);
    lv_obj_add_flag(ui->screen_1_line_4_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_4_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_4_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_4_1, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_4_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_4_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_1_3
    ui->screen_1_line_1_3 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_1_3[] = {{0, 0},{130, 0},};
    lv_line_set_points(ui->screen_1_line_1_3, screen_1_line_1_3, 2);
    lv_obj_set_pos(ui->screen_1_line_1_3, 100, 35);
    lv_obj_set_size(ui->screen_1_line_1_3, 130, 1);
    lv_obj_add_flag(ui->screen_1_line_1_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_1_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_1_3, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_1_3, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_1_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_1_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_3_3
    ui->screen_1_line_3_3 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_3_3[] = {{0, 0},{48, 0},};
    lv_line_set_points(ui->screen_1_line_3_3, screen_1_line_3_3, 2);
    lv_obj_set_pos(ui->screen_1_line_3_3, 182, 35);
    lv_obj_set_size(ui->screen_1_line_3_3, 48, 1);
    lv_obj_add_flag(ui->screen_1_line_3_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_3_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_3_3, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_3_3, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_3_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_3_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_2_3
    ui->screen_1_line_2_3 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_2_3[] = {{0, 0},{91, 0},};
    lv_line_set_points(ui->screen_1_line_2_3, screen_1_line_2_3, 2);
    lv_obj_set_pos(ui->screen_1_line_2_3, 141, 35);
    lv_obj_set_size(ui->screen_1_line_2_3, 91, 1);
    lv_obj_add_flag(ui->screen_1_line_2_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_2_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_2_3, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_2_3, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_2_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_2_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_line_4_3
    ui->screen_1_line_4_3 = lv_line_create(ui->screen_1);
    static lv_point_t screen_1_line_4_3[] = {{0, 0},{10, 0},};
    lv_line_set_points(ui->screen_1_line_4_3, screen_1_line_4_3, 2);
    lv_obj_set_pos(ui->screen_1_line_4_3, 221, 35);
    lv_obj_set_size(ui->screen_1_line_4_3, 10, 1);
    lv_obj_add_flag(ui->screen_1_line_4_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_1_line_4_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_1_line_4_3, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_line_4_3, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_line_4_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_1_line_4_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_1.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_1);

    //Init events for screen.
    events_init_screen_1(ui);
}
