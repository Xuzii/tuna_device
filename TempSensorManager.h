#ifndef TEMPSENSORMANAGER_H
#define TEMPSENSORMANAGER_H

#include <Arduino.h>
#include "esp_adc_cal.h"
#include "driver/adc.h"
#include <atomic>

class TempSensorManager
{
public:
    TempSensorManager(); 
    void setup();
    void calculateTemperature();

    double getPIDTemp()
    {
        return getTemp1();
    }

    double getTemp1() 
    {
        return getTempImpl(temperatureProbe1);
    }

    double getTemp2()
    {
        return getTempImpl(temperatureProbe2);
    }

    const uint32_t getSampleInterval()
    {
        return SAMPLE_INTERVAL;
    }

private:
    constexpr static const uint32_t V_REF = 1100;
    constexpr static const float VDD = 3.3; 
    constexpr static const float VDD_MAX = 3.9; 
    constexpr static const uint32_t MAX_12BIT_ADC_VALUE= (2^12)-1;
    constexpr static const uint32_t SAMPLE_INTERVAL = 100;
    static const uint32_t SAMPLE_TIME =  100;

    // thermistor specification
    constexpr static const float R1 = 16000.0;
    const double b = 3950.0;
    const double r0 = 10000.0;
    const double t0 = 298.15;

    const adc1_channel_t PROBE1_CHANNEL = ADC1_CHANNEL_6;
    const adc1_channel_t PROBE2_CHANNEL = ADC1_CHANNEL_7;

    esp_adc_cal_characteristics_t characteristics;

    /*
    const int lookupTable[101] = {
        31.77, 30.25, 28.82, 27.45, 26.13, 24.94, 23.77, 22.67, 21.62, 20.63, 19.68, 18.78, 17.93, 17.12, 16.35, 15.62, 14.93, 14.26, 13.63, 13.04, 12.47, 11.92, 11.41, 10.91, 10.45, 10, 9.575, 9.17, 8.784, 8.416, 8.064, 7.73, 7.41, 7.106, 6.815, 6.538, 6.273, 6.02, 5.778, 5.548, 5.327, 5.117, 4.915, 4.723, 4.539, 4.363, 4.195, 4.034, 3.88, 3.733, 3.592, 3.457, 3.328, 3.204, 3.086, 2.972, 5.863, 2.759, 2.659, 2.564, 2.472, 2.384, 2.299, 2.218, 2.141, 2.066, 1.994, 1.926, 1.86, 1.796, 1.735, 1.677, 1.621, 1.567, 1.515, 1.465, 1.417, 1.371, 1.326, 1.284, 1.243, 1.203, 1.165, 1.128, 1.093, 1.059, 1.027, 0.9955, 0.9654, 0.9363, 0.9083, 0.8812, 0.855, 0.8297, 0.8052, 0.7816, 0.7587, 0.7366, 0.7152, 0.6845, 0.6744
    };
    */

   const int lookupTable[101] = {};

    // flags
    bool initialized = false;
    bool cali_enable =  false;

    // output temperature after calculation
    double temperatureProbe1 = NAN;
    double temperatureProbe2 = NAN; 

    // check if temperature makes sense
    bool isValidTemp(double temp);

    // initialize temperature probes
    void initProbe(adc1_channel_t channel);
    void initProbe1()
    {
        return initProbe(PROBE1_CHANNEL);
    }

    void initProbe2()
    {
        return initProbe(PROBE2_CHANNEL);
    }

    // read raw value from temperature probe
    double readProbe(adc1_channel_t channel);
    double readProbe1()
    {
        return readProbe(PROBE1_CHANNEL);
    }

    double readProbe2()
    {
        return readProbe(PROBE2_CHANNEL);
    }

    double getTempSteinhart(double r);
    double getTempLUP(double r);

    double getTempImpl(double probe);
};

#endif //TEMPSENSORMANAGER_H
