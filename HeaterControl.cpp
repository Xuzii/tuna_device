#include "HeaterControl.h"

HeaterControl::HeaterControl(int heaterPin) : heaterPin_(heaterPin) {
  // Set heater and temperature sensor pins as outputs
  pinMode(heaterPin_, OUTPUT);
  //pinMode(tempSensorPin_, INPUT);

  // Set default PID constants
  kp_ = 1.0;
  ki_ = 0.1;
  kd_ = 0.01;

  // Initialize PID terms
  integral_ = 0.0;
  derivative_ = 0.0;
  previousError_ = 0.0;
}

void HeaterControl::setTargetTemp(float targetTemp) {
  targetTemp_ = targetTemp;
}

void HeaterControl::update(double temperature) {
  // Read temperature sensor value
  float temp = temperature;

  // Calculate error between current and target temperatures
  float error = targetTemp_ - temp;

  // Calculate PID output
  float output = kp_ * error + ki_ * integral_ + kd_ * derivative_;

  // Constrain output to 0-255 range
  output = constrain(output, 0, 255);

  // Set heater power using PID output
  analogWrite(heaterPin_, output);

  // Update integral and derivative terms for next iteration
  integral_ += error;
  derivative_ = error - previousError_;
  previousError_ = error;
}