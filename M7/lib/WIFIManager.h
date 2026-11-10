#ifndef  WIFI_MANAGER_H 
#define WIFI_MANAGER_H 
#include <WiFi.h>
#include <unordered_map>
#include <string>

class WIFIManager{

private:
    std::unordered_map<std::string,std::string> known_credentials;
    static WiFiClient wifi_client;
    int wifi_status = WL_IDLE_STATUS;

public:
    WIFIManager();
    ~WIFIManager() = default;
    int getStatus();
    int connect(const std::string &ssid, const std::string &password);

};

#endif // WIFI_MANAGER_H