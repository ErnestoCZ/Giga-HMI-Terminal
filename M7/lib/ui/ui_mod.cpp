#include "ui_mod.h"
#include <lvgl.h>
#include <Arduino_H7_Video.h>
#include "components/tw_tab_ess.h"
#include "components/tw_tab_settings.h"
#include "components/statusbar.h"
extern Arduino_H7_Video Display;

void initUI(){

    lv_obj_t * screen = lv_screen_active();
    lv_obj_set_size(screen, Display.width(),Display.height());

    auto tabWidget = lv_tabview_create(screen);
    auto tabContent = lv_tabview_get_content(tabWidget);
    lv_obj_clear_flag(tabContent,LV_OBJ_FLAG_SCROLLABLE); //Turns of gesture tab switch
    create_tw_tab_ess(tabWidget);
    create_tw_tab_settings(tabWidget);

    create_statusBar(screen);

};