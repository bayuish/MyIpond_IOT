#ifndef CONFIG_H
#define CONFIG_H

// ============= WiFi Configuration =============
#define WIFI_SSID "MBC 2.4GHz"
#define WIFI_PASSWORD "gogombc123"

// ============= Server Configuration =============
#define SERVER_URL "http://mechine-learning-ecis.mbclaboratory.com/predict"

// ============= Pin Configuration =============
// Sensor pH
#define PH_SENSOR_PIN 32

// Sensor Kekeruhan
#define TURBIDITY_SENSOR_PIN 33

// Sensor Suhu DS18B20
#define TEMPERATURE_SENSOR_PIN 27

// ============= ESP Configuration =============
#define ESP_ID "ESP_Kolam1"

// ============= NTP Configuration =============
#define NTP_OFFSET 25200  // GMT+7 untuk Indonesia
#define NTP_SERVER "id.pool.ntp.org"

// ============= Kalibrasi Sensor =============
// Kalibrasi pH
#define PH_OFFSET 123.33
#define PH_SCALE 175.06

// Kalibrasi Turbidity
#define TURBIDITY_OFFSET 2464.3571
#define TURBIDITY_SCALE 0.8452

#endif