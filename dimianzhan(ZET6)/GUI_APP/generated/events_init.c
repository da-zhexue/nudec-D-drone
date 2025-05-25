/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

#include "custom.h"

static void screen_slider_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_RELEASED:
    {
        int slider_val = lv_slider_get_value(guider_ui.screen_slider);
        slider_event_cb(slider_val,guider_ui.screen_table);
        break;
    }
    default:
        break;
    }
}

static void screen_Reset_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        num_reset(guider_ui.screen_table);
        break;
    }
    default:
        break;
    }
}

static void screen_Confirm_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        char in1[32];
        lv_dropdown_get_selected_str(guider_ui.screen_input,in1,sizeof(in1));
        const char* input=in1;
        search_for_position(input);
        break;
    }
    default:
        break;
    }
}

static void screen_next_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_1, guider_ui.screen_1_del, &guider_ui.screen_del, setup_scr_screen_1, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_slider, screen_slider_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_Reset, screen_Reset_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_Confirm, screen_Confirm_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_next, screen_next_event_handler, LV_EVENT_ALL, ui);
}

static void screen_1_confirm2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        char input2[32];
        lv_dropdown_get_selected_str(guider_ui.screen_1_input2,input2,sizeof(input2));
        show_position(input2);
        break;
    }
    default:
        break;
    }
}

static void screen_1_back_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_1_del, setup_scr_screen, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_1 (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_1_confirm2, screen_1_confirm2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_1_back, screen_1_back_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
