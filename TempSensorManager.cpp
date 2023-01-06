#include "TempSensorManager.h"

TempSensorManager::TempSensorManager()
{

}

void TempSensorManager::setup()
{
    initProbe1();
    initProbe2();

    initialized = true;

    /*
    timerProbeSample = xTimerCreate(
    "timerProbeSample",        
    pdMS_TO_TICKS(SAMPLE_TIME), 
    pdTRUE,                
    (void *)this,          
    _timerProbeSample);            

    if (timerProbeSample == NULL)
    {
        DEBUG_PRINTF("Unable to create timerProbeSample\n");
        halt_on_error();
    }

    if(xTimerStart(timerProbeSample, portMAX_DELAY) == pdFAIL)
    {
        DEBUG_PRINTF("Unable to start timerProbeSample\n");
        halt_on_error();
    }
    */

}

void TempSensorManager::calculateTemperature()
{
    // get voltage
    double voltage1 = readProbe1();
    //Serial.printf("voltage1: %f\n", voltage1);
    
    // find resistance based on voltage
    double resistance = (R1*voltage1)/(VDD - voltage1);
    //Serial.printf("resistance: %f\n", resistance);

    // get temperature based on resistance
    temperatureProbe1 = getTempSteinhart(resistance);

}

bool TempSensorManager::isValidTemp(double temp) 
{
    if(isnan(temp))
    {
        return false;
    }
    return temp > -0.5 && temp < 300.0;
}

void TempSensorManager::initProbe(adc1_channel_t channel)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel, ADC_ATTEN_DB_11);
    if (!cali_enable) 
    {
        cali_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1,
            ADC_ATTEN_DB_11,
            ADC_WIDTH_BIT_12,
            V_REF,
            &characteristics);
        //DEBUG_PRINTF("VREF %u\n", characteristics.vref);    
    }
}

double TempSensorManager::readProbe(adc1_channel_t channel) 
{
	int32_t raw = 0;
    const uint32_t SAMPLES = 0;
    const uint32_t TOTAL_SHIFT = SAMPLES<<1;
    for(int32_t i = 0; i < (1<<TOTAL_SHIFT);i++) 
    {
        int newValue = adc1_get_raw(channel);
        if(newValue == -1)
        {
            return NAN;
        }
        
        raw += adc1_get_raw(channel);
    }

    raw = raw>>TOTAL_SHIFT;
    if (cali_enable) 
    {
        return esp_adc_cal_raw_to_voltage(raw, &characteristics) * 0.001;
    }
    
    return ((float)raw*VDD_MAX)/MAX_12BIT_ADC_VALUE;
}

double TempSensorManager::getTempSteinhart(double r)
{
    double logR = log(r); // get the logarithm of the resistance
    double temperature = 1 / (0.0012671238906383896 + 0.0002102861133812485 * logR + 1.9209353577048828e-7 * logR * logR * logR);
    temperature = temperature - 273.15; // convert from Kelvin to Celsius
    return temperature;
}

double TempSensorManager::getTempLUP(double resistance)
{
    // Find the two closest values in the lookup table
    int index = 0;
    while (resistance > lookupTable[index])
    {
        index++;
    }
    int index1 = index - 1;
    int index2 = index;

    // Interpolate between the two values
    float value1 = lookupTable[index1];
    float value2 = lookupTable[index2];
    float temperature = (index2 - index1) * resistance + value1;
    return temperature;
}

double TempSensorManager::getTempImpl(double probe)
{
    if(!initialized)
    {
        return NAN;
    }

    return probe;
}