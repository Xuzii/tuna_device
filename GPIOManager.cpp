#include "GPIOManager.h"

GPIOManager::GPIOManager(int buttonPin) 
 //   : buttonPin(buttonPin), buttonPressed(false)
{
    this->buttonPin = buttonPin;
    buttonPressed = false;
}

void GPIOManager::setup()
{
    pinMode(buttonPin, INPUT_PULLUP);
}

void GPIOManager::checkButtonStatus()
{
    // read the state of the button
    int buttonState = digitalRead(buttonPin);

    // if the button is pressed and the flag is not set
    if (buttonState == LOW && !buttonPressed) {
        buttonPressed = true;  // set the flag

        // do something here (e.g. turn on an LED, send a message)
        Serial.printf("Button Pressed \n");
    }
    // if the button is not pressed and the flag is set
    else if (buttonState == HIGH && buttonPressed) {
        buttonPressed = false;  // clear the flag
    }
}

bool GPIOManager::getButtonPressed()
{
    return buttonPressed;
}