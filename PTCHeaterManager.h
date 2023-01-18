#include <Arduino.h>
#include <PID_v1.h>

class PTCHeaterManager
{
    private:
        double setpoint, input, output;
        PID pid;
        int pwmValue;
        int HEATER_PIN = 2;

    public:
        PTCHeaterManager();
        void setup();
        void update(double temperature);
};