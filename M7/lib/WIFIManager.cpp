#include "WIFIManager.h"
#define WL_CONNECTION_ATTEMPTS 5

void printWifiStatus();

WIFIManager::WIFIManager(){

}
int WIFIManager::getStatus(){
    return WiFi.status();
}
int WIFIManager::connect(const std::string &ssid, const std::string &password){

    uint8_t attempts = WL_CONNECTION_ATTEMPTS;

    while(wifi_status != WL_CONNECTED & attempts != 0){
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid.c_str());
        wifi_status = WiFi.begin(ssid.c_str(), password.c_str());
        attempts--;
    }

    if(wifi_status == WL_CONNECTED){
        printWifiStatus();
    };
    

}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}