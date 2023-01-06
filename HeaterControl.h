#ifndef HEATERCONTROL_H
#define HEATERCONTROL_H

#include <Arduino.h>
#include "esp32-hal-ledc.h"

class HeaterControl {
 public:
  // Constructor
  HeaterControl(int heaterPin);

  // Set target temperature
  void setTargetTemp(float targetTemp);

  // Update heater power using PID loop
  void update(double temperature);

 private:
  // PWM pin for heater control
  int heaterPin_;

  // Target temperature
  float targetTemp_;

  // PID constants
  float kp_;
  float ki_;
  float kd_;

  // PID terms
  float integral_;
  float derivative_;
  float previousError_;
};

#endif  // HEATERCONTROL_H