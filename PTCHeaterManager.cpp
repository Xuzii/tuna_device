#include "PTCHeaterManager.h"

PTCHeaterManager::PTCHeaterManager() : pid(&input, &output, &setpoint, 2, 5, 1, DIRECT)
{

}

void PTCHeaterManager::setup()
{
    pinMode(HEATER_PIN, OUTPUT);
    pid.SetMode(AUTOMATIC);
    pid.SetSampleTime(1000);
    setpoint = 37.0;
}

void PTCHeaterManager::update(double temperature)
{
    input = temperature;
    pid.Compute();
    pwmValue = map(input, 0, 100, 0, 255);
    analogWrite(HEATER_PIN, pwmValue);
}