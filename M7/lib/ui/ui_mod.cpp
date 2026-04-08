#include "ui_mod.h"
#include <lvgl.h>
#include <Arduino_H7_Video.h>

extern Arduino_H7_Video Display;

static lv_subject_t s_temp;
static lv_subject_t s_hum;
static lv_subject_t s_press;
static lv_subject_t s_dev_name;
static char device_name_buffer[32] = "--";

void init_data_subjects(){
    lv_subject_init_int(&s_temp, INT32_MIN);
    lv_subject_init_int(&s_hum, INT32_MIN);
    lv_subject_init_int(&s_press, INT32_MIN);
    lv_subject_init_pointer(&s_dev_name, device_name_buffer);
};

void initUI(){

    init_data_subjects();

    //ESS Screen
    lv_obj_t * screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, Display.width(),Display.height());

    lv_obj_t * flexbox = lv_obj_create(screen);
    lv_obj_set_flex_flow(flexbox, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(flexbox, lv_pct(100), lv_pct(100));

    //label sourcedevice + MAC
    auto label_device_name = lv_label_create(flexbox);
    lv_obj_set_width(label_device_name, lv_pct(100));
    lv_obj_set_style_text_font(label_device_name, &lv_font_montserrat_48, 0);
    lv_label_bind_text(label_device_name,&s_dev_name,"Device : %s");

    //label temperature +  data
    auto label_temperature = lv_label_create(flexbox);
    lv_obj_set_width(label_temperature, lv_pct(100));
    lv_obj_set_style_text_font(label_temperature, &lv_font_montserrat_30, 0);
    lv_label_bind_text(label_temperature,&s_temp,"Temperature : %d C");
    
    //label humidity +  data
    auto label_humidity = lv_label_create(flexbox);
    lv_obj_set_width(label_humidity, lv_pct(100));
    lv_obj_set_style_text_font(label_humidity, &lv_font_montserrat_30, 0);
    lv_label_bind_text(label_humidity,&s_hum,"Humidity : %d %%");

    //label pressure +  data
    auto label_pressure = lv_label_create(flexbox);
    lv_obj_set_width(label_pressure, lv_pct(100));
    lv_obj_set_style_text_font(label_pressure, &lv_font_montserrat_30, 0);
    lv_label_bind_text(label_pressure,&s_press,"Pressure : %d hPa");

};