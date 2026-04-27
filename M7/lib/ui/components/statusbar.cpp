#include "statusbar.h"


void create_statusBar(lv_obj_t * parent){
    //TODO statusbar
    auto statusBar = lv_obj_create(parent);
    lv_obj_set_size(statusBar,lv_pct(100),lv_pct(10));
    lv_obj_align(statusBar,LV_ALIGN_BOTTOM_MID,0,0);
    lv_obj_set_flex_flow(statusBar,LV_FLEX_FLOW_ROW);
    lv_obj_clear_flag(statusBar,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(statusBar,0,LV_PART_MAIN);
    auto status_WiFi_label = lv_label_create(statusBar);
    lv_label_set_text_fmt(status_WiFi_label,"WiFi(%s) :", "-");
    auto status_WiFi_led = lv_led_create(statusBar);
    lv_obj_set_size(status_WiFi_led,lv_pct(5),lv_pct(50));
    auto status_BL_label = lv_label_create(statusBar);
    lv_label_set_text_fmt(status_BL_label,"Bluetooth : ");
    auto status_BL_led = lv_led_create(statusBar);
    lv_obj_set_size(status_BL_led,lv_pct(5),lv_pct(50));

    lv_led_set_color(status_WiFi_led,lv_palette_main(LV_PALETTE_RED));
    lv_led_set_color(status_BL_led,lv_palette_main(LV_PALETTE_RED));
    lv_obj_set_flex_align(statusBar,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
};