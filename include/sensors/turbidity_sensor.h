#ifndef TURBIDITY_SENSOR_H
#define TURBIDITY_SENSOR_H

#include <Arduino.h>
#include "../config.h"
#include "../data_structures.h"

class TurbiditySensor {
private:
    int pin;
    float offset;
    float scale;
    
    float readADC();
    float convertToVoltage(int adc);
    float convertToNTU(int adc);
    String getTurbidityStatus(float ntu);

public:
    TurbiditySensor(int pin, float offset, float scale);
    void begin();
    TurbidityData read();
    void calibrate(float knownNTU);
};

#endif