#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_H7_Video.h>
#include "Arduino_GigaDisplayTouch.h"

Arduino_H7_Video Display(800,480,GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;


void setup() {
  // put your setup code here, to run once:
  Display.begin();
  TouchDetector.begin();
  

  //Creation of lvgl graphics
  lv_obj_t * screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, Display.width(), Display.height());

}

void loop() {
  // put your main code here, to run repeatedly:
  lv_timer_handler();
}