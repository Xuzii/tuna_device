#include "displaymanager.h"
#include "tempprobemanager.h"
#include "pidmanager.h"
#include "WIFIManagerImpl.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define DISPLAY_SCK 23
#define DISPLAY_SDA 22

#define DISPLAY_RESET 25

#define DISPLAY_I2C_FREQ 800000
#define DISPLAY_I2C_ADDR 0x3C

#define DISPLAY_CARD_COUNT 7

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, DISPLAY_RESET,DISPLAY_I2C_FREQ, DISPLAY_I2C_FREQ);

DisplayManager::DisplayManager(TempProbeManager& _probeMgr, PIDManager& __pidMgr, WifiManagerImpl& __wifiMgr):
                   probeMgr(_probeMgr), pidMgr(__pidMgr), wifiMgr(__wifiMgr)
{
    pinMode(DISPLAY_RESET, OUTPUT);
    digitalWrite(DISPLAY_RESET, HIGH);
}


void DisplayManager::begin()
{
    if (!Wire.begin(DISPLAY_SDA, DISPLAY_SCK))
    {
        //DEBUG_PRINTLN("I2C failed");
        for (;;);
    }
    Wire.setClock(DISPLAY_I2C_FREQ);
    display.setRotation(2);
    if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDR, true, false))
    { 
        //DEBUG_PRINTLN("SSD1306 allocation failed");
        for (;;);
    }

    display.clearDisplay();
    // testdrawline();
    display.display();
}

void DisplayManager::testdrawline() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

typedef void (*drawLine)();

void DisplayManager::printTemp()
{
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    display.printf("Temp:%.1f", probeMgr.getTemp1());
}

void DisplayManager::printTargetTemp()
{
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 17);     
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    display.printf("Targt:%.1f", pidMgr.getSetpoint());
}


void DisplayManager::printState()
{
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 34);     
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    const char* strState =  "Idle";

    state = pidMgr.getState();

    if(state == PIDManager::Tuning)
    {
        strState = "Tuning";
    } 
    else if(state == PIDManager::PidEnabled)
    {
        strState = "PID Running";
    }
    
    display.printf("State:\n%s", strState);
}

void DisplayManager::printIP()
{
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);     
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    String ip = wifiMgr.GetIp().toString();

    display.printf("IP:%s", ip);
}

void DisplayManager::loop()
{
    uint32_t now  = millis();
    if(now - last_read < SAMPLE_INTERVAL)
    {
        return;
    }
    last_read = now;  
    display.clearDisplay();
    printTemp();
    printTargetTemp();
    printState();
    display.display();
}