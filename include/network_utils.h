#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "config.h"
#include "data_structures.h"

class NetworkManager {
private:
    WiFiUDP ntpUDP;
    NTPClient timeClient;
    
    String createJsonPayload(AllSensorData data);

public:
    NetworkManager();
    void begin();
    void update();
    bool connectToWiFi();
    bool isWiFiConnected();
    String getCurrentTime();
    bool sendToServer(AllSensorData data);
};

#endif