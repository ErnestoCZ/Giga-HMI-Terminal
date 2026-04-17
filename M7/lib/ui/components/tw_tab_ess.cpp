#include "tw_tab_ess.h"
#include <Arduino.h>
#define DEVICE_NAME_BUFFER_LEN 32
static lv_subject_t s_temp;
static lv_subject_t s_hum;
static lv_subject_t s_press;
static lv_subject_t s_dev_name;
static char device_name_buffer[DEVICE_NAME_BUFFER_LEN] = "--";

static void init_data_subjects(){
    lv_subject_init_int(&s_temp, 0);
    lv_subject_init_int(&s_hum, 0);
    lv_subject_init_int(&s_press, 0);
    lv_subject_init_pointer(&s_dev_name, device_name_buffer);
};
void temperatureFormatter(lv_observer_t* observer, lv_subject_t* subject){
    lv_obj_t* label = lv_observer_get_target_obj(observer);
    int32_t value = lv_subject_get_int(subject);
    lv_label_set_text_fmt(label,"Temperature : %d.%d °C",(value/100),(int32_t)(value%100));
}
void humidityFormatter(lv_observer_t* observer, lv_subject_t* subject){
    lv_obj_t* label = lv_observer_get_target_obj(observer);
    int32_t value = lv_subject_get_int(subject);
    lv_label_set_text_fmt(label,"Humidity : %d.%d %%",(value/100),(int32_t)(value%100));
}
void pressureFormatter(lv_observer_t* observer, lv_subject_t* subject){
    lv_obj_t* label = lv_observer_get_target_obj(observer);
    int32_t value = lv_subject_get_int(subject);
    lv_label_set_text_fmt(label,"Pressure : %d.%d hPa",(value/100),(int32_t)(value%100));
}

int tw_ess_set_device_name(const char * dev_name, const unsigned int len){
    if(len > DEVICE_NAME_BUFFER_LEN) return 1;

    strcpy(device_name_buffer,dev_name);
    lv_subject_set_pointer(&s_dev_name,device_name_buffer);
    return 0;
};

int tw_ess_set_temperature_value(const int32_t val){
    lv_subject_set_int(&s_temp, val);
    return 0;
};

int tw_ess_set_humidity_value(const int32_t val){
    lv_subject_set_int(&s_hum,val);
    return 0;
};

int tw_ess_set_pressure_value(const int32_t val){
    lv_subject_set_int(&s_press, val);
    return 0;
};

void create_tw_tab_ess(lv_obj_t* parent){
    init_data_subjects();
    auto tab = lv_tabview_add_tab(parent, "ESS");

    lv_obj_set_flex_flow(tab, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(tab, lv_pct(100), lv_pct(100));
    lv_obj_set_scrollbar_mode(tab,LV_SCROLLBAR_MODE_AUTO);

    //label sourcedevice + MAC
    auto label_device_name = lv_label_create(tab);
    lv_obj_set_width(label_device_name, lv_pct(100));
    lv_obj_set_style_text_font(label_device_name, &lv_font_montserrat_48, 0);
    lv_label_bind_text(label_device_name,&s_dev_name,"Device : %s");

    //label temperature +  data
    auto label_temperature = lv_label_create(tab);
    lv_obj_set_width(label_temperature, lv_pct(100));
    lv_obj_set_style_text_font(label_temperature, &lv_font_montserrat_30, 0);
    lv_subject_add_observer_obj(&s_temp,temperatureFormatter,label_temperature,NULL);
    
    //label humidity +  data
    auto label_humidity = lv_label_create(tab);
    lv_obj_set_width(label_humidity, lv_pct(100));
    lv_obj_set_style_text_font(label_humidity, &lv_font_montserrat_30, 0);
    lv_subject_add_observer_obj(&s_hum,humidityFormatter,label_humidity,NULL);

    //label pressure +  data
    auto label_pressure = lv_label_create(tab);
    lv_obj_set_width(label_pressure, lv_pct(100));
    lv_obj_set_style_text_font(label_pressure, &lv_font_montserrat_30, 0);
    lv_subject_add_observer_obj(&s_press,pressureFormatter,label_pressure,NULL);

    // auto chart = lv_chart_create(tab);
    // lv_chart_set_axis_range(chart,LV_CHART_AXIS_PRIMARY_Y,-10,100);
    // auto ser1 = lv_chart_add_series(chart,{0,255,128},LV_CHART_AXIS_PRIMARY_Y);
    // for (size_t i = 0; i < 100; i++)
    // {
    //     lv_chart_set_next_value(chart,ser1,lv_rand(20,30));
    // }
    // lv_chart_refresh(chart);
    

};