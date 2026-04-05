#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_H7_Video.h>
#include "Arduino_GigaDisplayTouch.h"
#include "config.h"
#include "ui_mod.h"
Arduino_H7_Video Display(DISPLAY_WIDTH,DISPLAY_HEIGHT,GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;


void setup() {
  // put your setup code here, to run once:
  if(Display.begin() != 0) return;
  if(TouchDetector.begin() == false) return;
  

  //Creation of lvgl graphics
  initUI();

}

void loop() {
  // put your main code here, to run repeatedly:
  lv_timer_handler();
}