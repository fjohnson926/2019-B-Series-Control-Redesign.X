#include "SystemManager.h"

ScreenMessages StandbyMessage("STANDBY", "MODE");
static char timeString[12];
static ScreenMessages SystemTimeMsg("TIME IS", timeString);
int LEDToggleCounter = 0;
bool powerLEDOn = false;
const int powerLEDOnTimeMs = 500;
const int powerLEDOffTimeMs = 5000;

void SystemManager::stateStandbyMode()
{
    if(changedState)
    {
        changedState = false;
        myUI->Screen->resetScreen();
        myUI->LEDs->powerLed->turnOffLed();
        myUI->LEDs->turnOffAllLEDs(true);
        powerLEDOn = false;
        LEDToggleCounter = 0;
        myTank->disableHeating();
        myTank->disableFilling();
   
    }
    
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&StandbyMessage);
        printTimeStandardFormat(timeString, myCalendar->getHour(), myCalendar->getMinute(), myCalendar->is24TimeFormatEnabled());   
        myUI->Screen->addMessageToQueue(&SystemTimeMsg);
    }
    
    if(releasedTouchValue == TOUCH_POWER)
    {
        NVIC_SystemReset();
    }
    
    LEDToggleCounter+= SYSTEM_MANAGER_TASK_RATE_MS;
    if(LEDToggleCounter >= powerLEDOffTimeMs && !powerLEDOn)
    {
        myUI->LEDs->powerLed->turnOnLED();
        powerLEDOn = true;
        LEDToggleCounter = 0;
    }
    else if(LEDToggleCounter >= powerLEDOnTimeMs && powerLEDOn)
    {
        myUI->LEDs->powerLed->turnOffLed();
        powerLEDOn = false;
        LEDToggleCounter = 0;
    }
}
