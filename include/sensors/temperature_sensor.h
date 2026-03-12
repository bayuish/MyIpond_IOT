#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "../config.h"
#include "../data_structures.h"

class TemperatureSensor {
private:
    OneWire oneWire;
    DallasTemperature sensors;
    int pin;
    
    String getTemperatureStatus(float celsius);

public:
    TemperatureSensor(int pin);
    void begin();
    TemperatureData read();
    bool isConnected();  // Cek koneksi sensor
};

#endif