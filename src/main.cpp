#include <Arduino.h>
#include "config.h"
#include "data_structures.h"
#include "sensors/ph_sensor.h"
#include "sensors/turbidity_sensor.h"
#include "sensors/temperature_sensor.h"
#include "network_utils.h"

// Inisialisasi semua sensor
PHSensor phSensor(PH_SENSOR_PIN, PH_OFFSET, PH_SCALE);
TurbiditySensor turbiditySensor(TURBIDITY_SENSOR_PIN, TURBIDITY_OFFSET, TURBIDITY_SCALE);
TemperatureSensor tempSensor(TEMPERATURE_SENSOR_PIN);
NetworkManager network;

// Variabel untuk menyimpan semua data
AllSensorData sensorData;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n=== ESP32 Kolam Monitoring System ===");
    
    // Inisialisasi semua komponen
    network.begin();
    
    phSensor.begin();
    turbiditySensor.begin();
    tempSensor.begin();
    
    // Set ESP ID
    sensorData.esp_id = ESP_ID;
    
    Serial.println("✅ System ready!");
}

void loop() {
    // Update waktu dari NTP
    network.update();
    
    // Baca semua sensor
    sensorData.ph = phSensor.read();
    sensorData.turbidity = turbiditySensor.read();
    sensorData.temperature = tempSensor.read();
    sensorData.timestamp = network.getCurrentTime();
    
    // Tampilkan di Serial Monitor
    Serial.println("\n📊 Data Sensor:");
    Serial.println("  pH: " + String(sensorData.ph.value) + " (" + sensorData.ph.status + ")");
    Serial.println("  Kekeruhan: " + String(sensorData.turbidity.ntu) + " NTU (" + sensorData.turbidity.status + ")");
    Serial.println("  Suhu: " + String(sensorData.temperature.celsius) + "°C (" + sensorData.temperature.status + ")");
    Serial.println("  Waktu: " + sensorData.timestamp);
    
    // Kirim ke server
    if (network.isWiFiConnected()) {
        network.sendToServer(sensorData);
    } else {
        Serial.println("⚠️ Menunggu koneksi WiFi...");
        network.connectToWiFi();
    }
    
    // Delay 5 detik sebelum pembacaan berikutnya
    delay(5000);
}