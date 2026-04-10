#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_H7_Video.h>
#include "Arduino_GigaDisplayTouch.h"
#include "config.h"
#include "ui_mod.h"
Arduino_H7_Video Display(DISPLAY_WIDTH,DISPLAY_HEIGHT,GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;
#include <ArduinoBLE.h>
BLEDevice peripheral;
void setup() {
  // put your setup code here, to run once:
  if(Display.begin() != 0) return;
  if(TouchDetector.begin() == false) return;

  BLE.begin();

  Serial.println("Init completed");
  BLE.scanForName("NRF5340Dev");


  //Creation of lvgl graphics
  initUI();

}

void loop() {
  // put your main code here, to run repeatedly:
  lv_timer_handler();
  peripheral = BLE.available();
  if(peripheral){
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    ui_set_device_name(peripheral.localName().c_str(), (size_t)peripheral.localName().length());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();
    peripheral.connect();
    if(peripheral.connected()){
      peripheral.discoverService(peripheral.advertisedServiceUuid().c_str());
      if(peripheral.characteristicCount() > 0){
        BLEService service = peripheral.service(peripheral.advertisedServiceUuid().c_str());
        Serial.println("Found the following characteristics in ESS:");
        for (int i = 0; i < service.characteristicCount(); i++)
        {
          int32_t charValue = 0;
          BLECharacteristic characteristic = peripheral.characteristic(i);
          Serial.println(characteristic.uuid());
          if(characteristic.read()){
            characteristic.readValue(charValue);
            if(strcmp(characteristic.uuid(), "2a6e") == 0){
              ui_set_temperature_value(charValue);
            }
            if(strcmp(characteristic.uuid(), "2a6f") == 0){
              ui_set_humidity_value(charValue);
            }
            if(strcmp(characteristic.uuid(), "2a6d") == 0){
              ui_set_pressure_value(charValue);
            }
          }
        }
        
      }
      
        
      peripheral.disconnect();
      BLE.scanForName("NRF5340Dev");
    }
  }
}