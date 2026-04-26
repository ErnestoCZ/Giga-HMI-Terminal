#include "tw_tab_settings.h"
#include <Arduino_GigaDisplay.h>

constexpr int32_t DISPLAY_BRIGHTNESS_MIN = 20; 


static GigaDisplayBacklight backlight;
static lv_subject_t s_display_brightness;

//PFD
void on_brightness_change(lv_observer_t * observer, lv_subject_t * subject);

static void init_subjects(){
    backlight.begin();
    lv_subject_init_int(&s_display_brightness, 100);
};

static lv_obj_t* createSettingBox(lv_obj_t* parent){
    auto box = lv_obj_create(parent);
    lv_obj_set_size(box, lv_pct(100), lv_pct(20));
    lv_obj_set_flex_flow(box,LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(box, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    return box;
};

void create_tw_tab_settings(lv_obj_t* parent){
    init_subjects();
    auto tab = lv_tabview_add_tab(parent,"Settings");

    lv_obj_set_flex_flow(tab,LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(tab,lv_pct(100),lv_pct(100));
    lv_obj_set_scrollbar_mode(tab,LV_SCROLLBAR_MODE_AUTO);

    auto box_displayBrightness = createSettingBox(tab);
    auto display_brightness_label = lv_label_create(box_displayBrightness);
    lv_label_bind_text(display_brightness_label,&s_display_brightness,"Display Brightness (%d):");
    auto display_brightness_slider = lv_slider_create(box_displayBrightness);
    lv_slider_set_range(display_brightness_slider,DISPLAY_BRIGHTNESS_MIN,100);
    lv_slider_bind_value(display_brightness_slider,&s_display_brightness);
    lv_subject_add_observer(&s_display_brightness,on_brightness_change,NULL);
};

void on_brightness_change(lv_observer_t * observer, lv_subject_t * subject){
    int32_t value = lv_subject_get_int(subject);
    backlight.set(value);
};