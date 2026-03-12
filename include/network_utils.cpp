#include "network_utils.h"

NetworkManager::NetworkManager() : timeClient(ntpUDP, NTP_SERVER, NTP_OFFSET) {
}

void NetworkManager::begin() {
    connectToWiFi();
    timeClient.begin();
}

void NetworkManager::update() {
    timeClient.update();
}

bool NetworkManager::connectToWiFi() {
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        Serial.print(".");
        delay(500);
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✅ Connected! IP: " + WiFi.localIP().toString());
        return true;
    } else {
        Serial.println("\n❌ Failed to connect to WiFi");
        return false;
    }
}

bool NetworkManager::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String NetworkManager::getCurrentTime() {
    return timeClient.getFormattedTime();
}

String NetworkManager::createJsonPayload(AllSensorData data) {
    String json = "{";
    json += "\"ESP_ID\": \"" + data.esp_id + "\",";
    json += "\"pH\": " + String(data.ph.value) + ",";
    json += "\"pH_Status\": \"" + data.ph.status + "\",";
    json += "\"NTU\": " + String(data.turbidity.ntu) + ",";
    json += "\"Turbidity_Status\": \"" + data.turbidity.status + "\",";
    json += "\"Temperature\": " + String(data.temperature.celsius) + ",";
    json += "\"Temperature_Status\": \"" + data.temperature.status + "\",";
    json += "\"ADC_PH\": " + String(data.ph.adc) + ",";
    json += "\"Voltage_PH\": " + String(data.ph.voltage) + ",";
    json += "\"ADC_Turbidity\": " + String(data.turbidity.adc) + ",";
    json += "\"Voltage_Turbidity\": " + String(data.turbidity.voltage) + ",";
    json += "\"Timestamp\": \"" + data.timestamp + "\"";
    json += "}";
    
    return json;
}

bool NetworkManager::sendToServer(AllSensorData data) {
    if (!isWiFiConnected()) {
        Serial.println("❌ WiFi disconnected!");
        return false;
    }
    
    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    
    String payload = createJsonPayload(data);
    Serial.println("📤 Sending: " + payload);
    
    int httpCode = http.POST(payload);
    bool success = (httpCode > 0);
    
    if (success) {
        Serial.println("✅ Data sent! Response: " + http.getString());
    } else {
        Serial.println("❌ Error: " + String(httpCode));
    }
    
    http.end();
    return success;
}