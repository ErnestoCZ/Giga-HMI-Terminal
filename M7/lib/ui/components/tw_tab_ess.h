#pragma once
#include <lvgl.h>

int tw_ess_set_device_name(const char * dev_name, const size_t len);

int tw_ess_set_temperature_value(const int32_t val);

int tw_ess_set_humidity_value(const int32_t val);

int tw_ess_set_pressure_value(const int32_t val);

void create_tw_tab_ess(lv_obj_t* parent);
    