#include "ui_mod.h"
#include <lvgl.h>
#include <Arduino_H7_Video.h>
#include "components/tw_tab_ess.h"

extern Arduino_H7_Video Display;

void initUI(){

    lv_obj_t * screen = lv_screen_active();
    lv_obj_set_size(screen, Display.width(),Display.height());

    auto tabWidget = lv_tabview_create(screen);
    create_tw_tab_ess(tabWidget);

};