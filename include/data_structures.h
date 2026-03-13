#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <Arduino.h>

struct PHData {
    float value;
    int adc;
    float avgADC;
    float voltage;
    String status;  // "Asam", "Netral", "Basa"
};

struct TurbidityData {
    float ntu;
    int adc;
    float avgADC;
    float voltage;
    String status;  // "Jernih", "Keruh", "Sangat Keruh"
};

struct TemperatureData {
    float celsius;
    float fahrenheit;
    String status;  // "Dingin", "Normal", "Panas"
};

struct AllSensorData {
    PHData ph;
    TurbidityData turbidity;
    TemperatureData temperature;
    String timestamp;
    String esp_id;
};

#endif