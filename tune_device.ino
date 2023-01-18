#include <Arduino.h>
#include "GPIOManager.h"
#include "TempSensorManager.h"
#include "PTCHeaterManager.h"

const int BUTTON_PIN = 0;
const int HEATER_PIN = 2;

GPIOManager gpioManager(BUTTON_PIN);
TempSensorManager tempSensorManager;
PTCHeaterManager ptcHeaterManager;
// HeaterControl heater(HEATER_PIN);

void setup() 
{
    Serial.begin(115200);
    gpioManager.setup();
    tempSensorManager.setup();
    ptcHeaterManager.setup();

    //heater.setTargetTemp(37.0);
}


void loop() 
{
    gpioManager.checkButtonStatus();
    tempSensorManager.calculateTemperature();
    double temp = tempSensorManager.getPIDTemp();

    Serial.printf("temp %f\n", temp);
    ptcHeaterManager.update(temp);
}
