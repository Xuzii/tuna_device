#include "PTCHeaterManager.h"

PTCHeaterManager::PTCHeaterManager() : pid(&input, &output, &setpoint, 2, 5, 1, DIRECT)
{

}

void PTCHeaterManager::setup()
{
    ledcAttachPin(HEATER_PIN, PWM1_Ch);
    ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
    ledcWrite(PWM1_Ch, 0);
    pid.SetMode(AUTOMATIC);
    pid.SetSampleTime(1000);
    setpoint = 37.0;
}

void PTCHeaterManager::update(double temperature)
{
    input = temperature;
    pid.Compute();
    pwmValue = map(input, 0, 100, 0, 100);
    ledcWrite(PWM1_Ch, pwmValue);
    
}