#pragma once
#include <Arduino.h>

void initUI(void);
int ui_set_device_name(const char * dev_name, const size_t len);
int ui_set_temperature_value(const int32_t val);
int ui_set_humidity_value(const int32_t val);
int ui_set_pressure_value(const int32_t val);