#include "sensors/ph_sensor.h"

PHSensor::PHSensor(int pin, float offset, float scale) {
    this->pin = pin;
    this->offset = offset;
    this->scale = scale;
}

void PHSensor::begin() {
    // Tidak perlu inisialisasi khusus untuk ADC
    pinMode(pin, INPUT);
}

float PHSensor::readADC() {
    return analogRead(pin);
}

float PHSensor::convertToVoltage(int adc) {
    // ESP32 ADC 12-bit (0-4095) dengan referensi 3.3V
    return (adc / 4095.0) * 3.3;
}

float PHSensor::convertToPH(int adc) {
    // Rumus: (adc - offset) / scale
    return (adc - offset) / scale;
}

String PHSensor::getPHStatus(float ph) {
    if (ph < 6.5) return "Asam";
    else if (ph > 7.5) return "Basa";
    else return "Netral";
}

PHData PHSensor::read() {
    PHData data;
    
    data.adc = readADC();
    data.voltage = convertToVoltage(data.adc);
    data.value = convertToPH(data.adc);
    data.status = getPHStatus(data.value);
    
    return data;
}

void PHSensor::calibrate(float knownPH) {
    // Logika kalibrasi sederhana
    int adc = readADC();
    float newScale = (adc - offset) / knownPH;
    scale = newScale;
    
    Serial.println("pH Sensor calibrated. New scale: " + String(scale));
}