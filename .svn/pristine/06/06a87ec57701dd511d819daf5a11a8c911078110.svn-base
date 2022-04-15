#include "SystemManager.h"
#include "NvParmManager.h"

ScreenMessages EnergySavingsModeMsg("ENERGY SAVINGS", "ACTIVE", DEFAULT_STATIC_PRINT_TIME_MS);
ScreenMessages ExitEnergySavingsModeMsg("PRESS POWER", "TO EXIT", DEFAULT_STATIC_PRINT_TIME_MS);
char energySavingsTimeString[12];
ScreenMessages SystemTimeMsgII("TIME IS", energySavingsTimeString);
char wakeupTimeString[12];
ScreenMessages WakeupTimeMsg("WAKEUP AT", wakeupTimeString);

void SystemManager::stateEnergySavingsMode()
{    
    if(releasedTouchValue == TOUCH_POWER || myEnergySaver->timeToWakeUp(myCalendar->getHour(), myCalendar->getMinute()))
    {
        NvParm tankSetpointFParm;
        tankSetpointFParm = NvParmManager::getNvParm(PARMINDEX_TANK_SETPOINT_F);
        myTank->setTankSetpointF(tankSetpointFParm.u.float_parm);        
        myEnergySaver->clearCounter();
        changeState(NORMAL_MODE);
        return;
    }   
    
    myTank->setTankSetpointF(140);
    myUI->LEDs->leftSingleBrewLeds->TurnOffAllLeds();
    myUI->LEDs->rightBrewLeds->TurnOffAllLeds();
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&EnergySavingsModeMsg);
        myUI->Screen->addMessageToQueue(&ExitEnergySavingsModeMsg);
        printTimeStandardFormat(energySavingsTimeString, myCalendar->getHour(), myCalendar->getMinute(), myCalendar->is24TimeFormatEnabled());
        myUI->Screen->addMessageToQueue(&SystemTimeMsgII);
        if(myEnergySaver->isAutoWakeupEnabled())
        {
            printTimeStandardFormat(wakeupTimeString, myEnergySaver->getWakeupHr(), myEnergySaver->getWakeupMin(), myCalendar->is24TimeFormatEnabled());        
            myUI->Screen->addMessageToQueue(&WakeupTimeMsg);
        }
    }
}
