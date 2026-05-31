#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_H7_Video.h>
#include <ArduinoBLE.h>
#include "Arduino_GigaDisplayTouch.h"
#include "config.h"
#include "ui_mod.h"
#include "components/tw_tab_ess.h"
#include "QSPIFBlockDevice.h"
#include "LittleFileSystem.h"
#include <Thread.h>

rtos::Thread ui_thread(osPriorityNormal, 8192);
rtos::Mutex lvgl_mutex;
static void ui_thread_handler(){
  initUI();
  for(;;){
    lvgl_mutex.lock();
    lv_timer_handler();
    lvgl_mutex.unlock();
  }
};

QSPIFBlockDevice bd;
mbed::LittleFileSystem fs("fs");


#define BLE_ESS_UUID 181a
#define BLE_TEMPERATURE_CHAR "2a6e"
#define BLE_HUMIDITY_CHAR "2a6f"
#define BLE_PRESSURE_CHAR "2a6d"
void BLE_TemperatureUpdateHandler(BLEDevice device, BLECharacteristic characteristic){
  int32_t value = 0;
  characteristic.readValue(value);
  tw_ess_set_temperature_value(value);
};
void BLE_HumidityUpdateHandler(BLEDevice device, BLECharacteristic characteristic){
  int32_t value = 0;
  characteristic.readValue(value);
  tw_ess_set_humidity_value(value);

};
void BLE_AirPressureUpdateHandler(BLEDevice device, BLECharacteristic characteristic){
  int32_t value = 0;
  characteristic.readValue(value);
  tw_ess_set_pressure_value(value);

};

Arduino_H7_Video Display(DISPLAY_WIDTH,DISPLAY_HEIGHT,GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;
BLEDevice peripheral;
void setup() {
  // put your setup code here, to run once:
  if(Display.begin() != 0) return;
  if(TouchDetector.begin() == false) return;
  ui_thread.start(ui_thread_handler);
  BLE.begin();

  int err = fs.mount(&bd);
  if(err){
    Serial.println("Mount failed. Formatting...");
    err = fs.reformat(&bd);
    if(err){
      Serial.println("Error formatting QSPI Flash!");
      return;
    }
  }
  FILE *f =fopen("/fs/test.txt","r");
  if(f == NULL){
    Serial.println("Requested File does not exists");
  }else{
    char buffer[256];
    while(fgets(buffer,sizeof(buffer),f)){
      Serial.println(buffer);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  BLE.scanForUuid("181a");

  peripheral = BLE.available();
  if(peripheral && !peripheral.connected()){
    BLE.stopScan();
    Serial.println(peripheral.localName());
    tw_ess_set_device_name(peripheral.localName().c_str(), peripheral.localName().length());
    for (int i = 0; i < peripheral.advertisedServiceUuidCount(); i++)
    {
      Serial.println(peripheral.advertisedServiceUuid(i));
      if(peripheral.advertisedServiceUuid(i) == "181a"){

        if(!peripheral.connected()){
          peripheral.connect();
          Serial.println("Connection established to peripheral");

        }else{
          Serial.println("Peripheral already connected");
        }
        peripheral.discoverService("181a");
        if(peripheral.hasCharacteristic(BLE_TEMPERATURE_CHAR) && peripheral.characteristic(BLE_TEMPERATURE_CHAR).canSubscribe()){
          peripheral.characteristic(BLE_TEMPERATURE_CHAR).setEventHandler(BLEWritten, BLE_TemperatureUpdateHandler);
          peripheral.characteristic(BLE_TEMPERATURE_CHAR).subscribe();
        }
        if(peripheral.hasCharacteristic(BLE_HUMIDITY_CHAR) && peripheral.characteristic(BLE_HUMIDITY_CHAR).canSubscribe()){
          peripheral.characteristic(BLE_HUMIDITY_CHAR).setEventHandler(BLEWritten, BLE_HumidityUpdateHandler);
          peripheral.characteristic(BLE_HUMIDITY_CHAR).subscribe();
        }
        if(peripheral.hasCharacteristic(BLE_PRESSURE_CHAR) && peripheral.characteristic(BLE_PRESSURE_CHAR).canSubscribe()){
          peripheral.characteristic(BLE_PRESSURE_CHAR).setEventHandler(BLEWritten, BLE_AirPressureUpdateHandler);
          peripheral.characteristic(BLE_PRESSURE_CHAR).subscribe();
        }
      }
    }
  }

}