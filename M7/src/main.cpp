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

enum class state{
  INIT,
  IDLE,
  SCAN,
  CONNECT,
  SUBSCRIBE,
};
void stateMachine(enum state &currentState);

static state currentState = state::INIT;

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


#define BLE_ESS_UUID "181a"
#define BLE_TEMPERATURE_CHAR "2a6e"
#define BLE_HUMIDITY_CHAR "2a6f"
#define BLE_PRESSURE_CHAR "2a6d"

void BLE_ESS_TempHandler(BLEDevice device, BLECharacteristic characteristic){
  int32_t value = 0;
  characteristic.readValue(value);
  if(!strcmp(characteristic.uuid(), BLE_TEMPERATURE_CHAR)){
    tw_ess_set_temperature_value(value);
  }
};
void BLE_ESS_HumHandler(BLEDevice device, BLECharacteristic characteristic){
  int32_t value = 0;
  characteristic.readValue(value);
  if(!strcmp(characteristic.uuid(), BLE_HUMIDITY_CHAR)){
    tw_ess_set_humidity_value(value);
  }
};
void BLE_ESS_PresHandler(BLEDevice device, BLECharacteristic characteristic){
  int32_t value = 0;
  characteristic.readValue(value);
  if(!strcmp(characteristic.uuid(), BLE_PRESSURE_CHAR)){
    tw_ess_set_pressure_value(value);
  }
};

Arduino_H7_Video Display(DISPLAY_WIDTH,DISPLAY_HEIGHT,GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;
BLEDevice peripheral;
void setup() {
  // put your setup code here, to run once:
  if(Display.begin() != 0) return;
  if(TouchDetector.begin() == false) return;
  ui_thread.start(ui_thread_handler);
  Serial.begin(115200);
  BLE.begin();
}

static void checkAndSubscribe(const char* uuid, BLECharacteristicEventHandler handler){
  Serial.print("Checking Characteristic ");
  Serial.println(uuid);

  if(peripheral.characteristic(uuid).canSubscribe()){
    peripheral.characteristic(uuid).setEventHandler(BLEWritten, handler);
    peripheral.characteristic(uuid).subscribe();
  }
};

void loop() {

  stateMachine(currentState);

}


void InitHandler(){

};
void IdleHandler(enum state &currentState){
  if(!BLE.connected()){
    //TODO resetUI()
    currentState = state::SCAN;
  }else{
    currentState = state::IDLE;
  }
};
void ScanHandler(enum state &currentState){
  BLE.scanForUuid(BLE_ESS_UUID);
  peripheral = BLE.available();
  if(!peripheral.connected()){
    currentState = state::CONNECT;
  }else{
    currentState = state::SCAN;
  };

};
void ConnectHandler(enum state &currentState){
  peripheral.connect();
  if(peripheral.connected()){
    auto remoteName = peripheral.localName();
    tw_ess_set_device_name(remoteName.c_str(), remoteName.length());
    currentState = state::SUBSCRIBE;
  }else{
    currentState = state::SCAN;
  };

};
void SubscribeHandler(enum state &currentState){

  if (peripheral.discoverService(BLE_ESS_UUID)) {
    // Explicitly check after discovering attributes
    for (int i = 0; i < peripheral.characteristicCount(); i++) {
      BLECharacteristic c = peripheral.characteristic(i);
      
      if (!strcmp(c.uuid(), BLE_TEMPERATURE_CHAR)) {
        checkAndSubscribe(BLE_TEMPERATURE_CHAR, BLE_ESS_TempHandler);
      } else if (!strcmp(c.uuid(), BLE_HUMIDITY_CHAR)) {
        checkAndSubscribe(BLE_HUMIDITY_CHAR, BLE_ESS_HumHandler);
      } else if (!strcmp(c.uuid(), BLE_PRESSURE_CHAR)) {
        checkAndSubscribe(BLE_PRESSURE_CHAR, BLE_ESS_PresHandler);
      }
    }
  }
  currentState = state::IDLE;
};

void stateMachine(enum state &currentState){
  Serial.print("Current State is ");
  Serial.println(static_cast<int>(currentState));

  switch (currentState)
  {
  case state::INIT:
    currentState = state::IDLE;
    break;
  case state::IDLE:
    IdleHandler(currentState);
    break;
  case state::SCAN:
    ScanHandler(currentState);
    break;
  case state::CONNECT:
    ConnectHandler(currentState);
    break;
  case state::SUBSCRIBE:
    SubscribeHandler(currentState);
    break;
  
  default:
    currentState = state::IDLE;
    break;
  }
}