#include <Arduino.h>
#include "GPIOManager.h"
#include "TempSensorManager.h"
#include "HeaterControl.h"

const int BUTTON_PIN = 0;
const int HEATER_PIN = 2;

GPIOManager gpioManager(BUTTON_PIN);
TempSensorManager tempSensorManager;
HeaterControl heater(HEATER_PIN);

void setup() 
{
    Serial.begin(115200);
    gpioManager.setup();
    tempSensorManager.setup();
    heater.setTargetTemp(37.0);
}


void loop() 
{
    gpioManager.checkButtonStatus();
    tempSensorManager.calculateTemperature();
    heater.update(tempSensorManager.getPIDTemp());
}
