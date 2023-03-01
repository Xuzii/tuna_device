#ifndef DISPLAYMANAGER
#define DISPLAYMANAGER

#include <Arduino.h>
#include "pidmanager.h"

class PIDManager;
class TempProbeManager;
class WifiManagerImpl;

class DisplayManager
{
public:
    DisplayManager(TempProbeManager& _probeMgr, PIDManager& __pidMgr, WifiManagerImpl& __wifiMgr);

    void begin();
    void loop();

private:
    TempProbeManager& probeMgr;
    PIDManager& pidMgr;
    WifiManagerImpl& wifiMgr;

    void testdrawline();
    void printTemp();
    void printState();
    void printTargetTemp();
    void printIP();

    bool tempUnitC = true;

    bool connectionStatus = false;
    double input = 0.0;
    double setpoint = 0.0;
    double output = 0.0;
    double probe1= 0.0;
    PIDManager::PID_STATE state = PIDManager::Idle;

    constexpr static const uint32_t SAMPLE_INTERVAL = 5000;
    uint32_t last_read = 0;


    
};

#endif //DISPLAYMANAGER