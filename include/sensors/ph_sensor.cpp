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

// Fungsi membaca ADC tunggal
float PHSensor::readSingleADC() {
    return analogRead(pin);
}

// Fungsi membaca ADC dengan averaging
float PHSensor::readADC() {
    const int N = 200;           // Total sampel
    const int trim = N * 0.2;    // 20% data awal di-trim
    float adcValues[N];

    // Ambil 200 sampel berturut-turut
    for (int i = 0; i < N; i++) {
        adcValues[i] = analogRead(pin);
        delay(2); // jeda kecil agar ADC stabil
    }

    // Hitung rata-rata setelah trim 20% awal
    float sum = 0;
    for (int i = trim; i < N; i++) {
        sum += adcValues[i];
    }

    float avgADC = sum / (N - trim);
    return avgADC;
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

    // Gunakan ADC averaging
    data.adc = readADC();
    data.voltage = convertToVoltage(data.adc);
    data.value = convertToPH(data.adc);
    data.status = getPHStatus(data.value);

    return data;
}

void PHSensor::calibrate(float knownPH) {
    int adc = readADC();  // gunakan ADC averaging untuk kalibrasi
    float newScale = (adc - offset) / knownPH;
    scale = newScale;

    Serial.println("pH Sensor calibrated. New scale: " + String(scale));
}