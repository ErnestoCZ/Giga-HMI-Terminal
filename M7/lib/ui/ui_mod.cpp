#include "ui_mod.h"
#include <lvgl.h>
#include <Arduino_H7_Video.h>

extern Arduino_H7_Video Display;


void initUI(){

    //ESS Screen
    lv_obj_t * screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, Display.width(),Display.height());

    lv_obj_t * flexbox = lv_obj_create(screen);
    lv_obj_set_flex_flow(flexbox, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(flexbox, lv_pct(100), lv_pct(100));

    //label sourcedevice + MAC
    auto label_device_name = lv_label_create(flexbox);
    lv_label_set_text_fmt(label_device_name, "Device : %s (%s)", "--", "--:--:--:--:--:--");
    lv_obj_set_width(label_device_name, lv_pct(100));
    lv_obj_set_style_text_font(label_device_name, &lv_font_montserrat_48, 0);

    //label temperature +  data
    auto label_temperature = lv_label_create(flexbox);
    lv_label_set_text_fmt(label_temperature, "Temperature : %s", "--");
    lv_obj_set_width(label_temperature, lv_pct(100));
    lv_obj_set_style_text_font(label_temperature, &lv_font_montserrat_30, 0);
    
    //label humidity +  data
    auto label_humidity = lv_label_create(flexbox);
    lv_label_set_text_fmt(label_humidity, "Humidity : %s", "--");
    lv_obj_set_width(label_humidity, lv_pct(100));
    lv_obj_set_style_text_font(label_humidity, &lv_font_montserrat_30, 0);
    //label pressure +  data
    auto label_pressure = lv_label_create(flexbox);
    lv_label_set_text_fmt(label_pressure, "Pressure : %s", "--");
    lv_obj_set_width(label_pressure, lv_pct(100));
    lv_obj_set_style_text_font(label_pressure, &lv_font_montserrat_30, 0);

};