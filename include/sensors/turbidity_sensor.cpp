#include "sensors/turbidity_sensor.h"

TurbiditySensor::TurbiditySensor(int pin, float offset, float scale) {
    this->pin = pin;
    this->offset = offset;
    this->scale = scale;
}

void TurbiditySensor::begin() {
    pinMode(pin, INPUT);
}

// Baca satu kali ADC
float TurbiditySensor::readSingleADC() {
    return analogRead(pin);
}

// Baca ADC dengan averaging + trim 20%
float TurbiditySensor::readADC() {
    const int N = 200;          // total sampel
    const int trim = N * 0.2;   // 20% data awal di-trim
    float adcValues[N];

    for (int i = 0; i < N; i++) {
        adcValues[i] = readSingleADC();
        delay(2); // delay kecil supaya ADC stabil
    }

    // hitung rata-rata setelah trim
    float sum = 0;
    for (int i = trim; i < N; i++) {
        sum += adcValues[i];
    }

    return sum / (N - trim);
}

float TurbiditySensor::convertToVoltage(int adc) {
    return (adc / 4095.0) * 3.3;
}

float TurbiditySensor::convertToNTU(int adc) {
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

    data.adc = readADC();                 // ADC rata-rata
    data.voltage = convertToVoltage(data.adc);
    data.ntu = convertToNTU(data.adc);
    data.status = getTurbidityStatus(data.ntu);

    return data;
}

void TurbiditySensor::calibrate(float knownNTU) {
    int adc = readADC();  // gunakan ADC rata-rata untuk kalibrasi
    float newScale = (offset - adc) / knownNTU;
    scale = newScale;

    Serial.println("Turbidity sensor calibrated. New scale: " + String(scale));
}