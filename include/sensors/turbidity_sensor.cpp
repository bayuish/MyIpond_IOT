#include "sensors/turbidity_sensor.h"

TurbiditySensor::TurbiditySensor(int pin, float offset, float scale) {
    this->pin = pin;
    this->offset = offset;
    this->scale = scale;
}

void TurbiditySensor::begin() {
    pinMode(pin, INPUT);
}

float TurbiditySensor::readADC() {
    return analogRead(pin);
}

float TurbiditySensor::convertToVoltage(int adc) {
    return (adc / 4095.0) * 3.3;
}

float TurbiditySensor::convertToNTU(int adc) {
    // Rumus: (offset - adc) / scale
    float ntu = (offset - adc) / scale;
    if (ntu < 0) ntu = 0;
    return ntu;
}

String TurbiditySensor::getTurbidityStatus(float ntu) {
    if (ntu < 5) return "Jernih";
    else if (ntu < 25) return "Sedikit Keruh";
    else if (ntu < 100) return "Keruh";
    else return "Sangat Keruh";
}

TurbidityData TurbiditySensor::read() {
    TurbidityData data;
    
    data.adc = readADC();
    data.voltage = convertToVoltage(data.adc);
    data.ntu = convertToNTU(data.adc);
    data.status = getTurbidityStatus(data.ntu);
    
    return data;
}

void TurbiditySensor::calibrate(float knownNTU) {
    // Logika kalibrasi
    int adc = readADC();
    float newScale = (offset - adc) / knownNTU;
    scale = newScale;
    
    Serial.println("Turbidity sensor calibrated. New scale: " + String(scale));
}