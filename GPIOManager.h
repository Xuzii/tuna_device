#ifndef GPIOMANAGER_H
#define GPIOMANAGER_H

#include <Arduino.h>

class GPIOManager
{
public:
    GPIOManager(int buttonPin = 0);
    
    bool getButtonPressed();
    void setup();
    void checkButtonStatus();

private:
    // button debouncer
    int buttonPin;
    bool buttonPressed;

    int switchPin = 18;
};

#endif //GPIOMANAGER_H
