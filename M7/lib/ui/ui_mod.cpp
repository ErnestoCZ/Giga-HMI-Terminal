#include "ui_mod.h"
#include <lvgl.h>
#include <Arduino_H7_Video.h>

extern Arduino_H7_Video Display;


void initUI(){

    lv_obj_t * screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, Display.width(),Display.height());

    

};