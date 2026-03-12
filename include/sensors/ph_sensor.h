#ifndef PH_SENSOR_H
#define PH_SENSOR_H

#include <Arduino.h>
#include "../config.h"
#include "../data_structures.h"

class PHSensor {
private:
    int pin;
    float offset;
    float scale;
    
    // Private methods
    float readADC();
    float convertToVoltage(int adc);
    float convertToPH(int adc);
    String getPHStatus(float ph);

public:
    PHSensor(int pin, float offset, float scale);
    void begin();
    PHData read();
    void calibrate(float knownPH);  // Untuk kalibrasi
};

#endif