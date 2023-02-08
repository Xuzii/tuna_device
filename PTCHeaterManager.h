#include <Arduino.h>
#include <PID_v1.h>

class PTCHeaterManager
{
    private:
        double setpoint, input, output;
        PID pid;
        int pwmValue;
        int HEATER_PIN = 18;

        // arduino analogWrite setting
        uint8_t PWM1_Ch = 2;
        uint8_t PWM1_Res = 8;
        double PWM1_Freq = 1000;

    public:
        PTCHeaterManager();
        void setup();
        void update(double temperature);
};