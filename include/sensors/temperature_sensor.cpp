#include "sensors/temperature_sensor.h"

TemperatureSensor::TemperatureSensor(int pin) : oneWire(pin), sensors(&oneWire) {
    this->pin = pin;
}

void TemperatureSensor::begin() {
    sensors.begin();
}

TemperatureData TemperatureSensor::read() {
    TemperatureData data;
    
    sensors.requestTemperatures();
    data.celsius = sensors.getTempCByIndex(0);
    data.fahrenheit = (data.celsius * 9.0/5.0) + 32.0;
    data.status = getTemperatureStatus(data.celsius);
    
    return data;
}

String TemperatureSensor::getTemperatureStatus(float celsius) {
    if (celsius < 20) return "Dingin";
    else if (celsius < 30) return "Normal";
    else return "Panas";
}

bool TemperatureSensor::isConnected() {
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    return (temp != DEVICE_DISCONNECTED_C);
}