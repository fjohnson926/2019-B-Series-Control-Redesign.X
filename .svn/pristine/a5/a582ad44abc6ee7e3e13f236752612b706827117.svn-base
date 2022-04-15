#include "SystemManager.h"

static ScreenMessages TankOutputMsg("FCT TANK","OUTPUTS");
static ScreenMessages TankOutputMap1Msg("CS11","HEATER");
static ScreenMessages TankOutputMap2Msg("CS06","INLET");


static bool outputActive = false;
static int outputCounter = 0;

 void SystemManager::turnOffTankOutputs(void)
{
    myTank->turnElementOff();
    myTank->closeInletValve();
    myUI->LEDs->turnOffAllLEDs(true);     
    outputActive = false;
}

void SystemManager::stateManageTankOutputs()
{
    if(changedState)
    {
        myUI->Screen->showMessageNow(&TankOutputMsg);
        changedState = false;
        outputActive = false;
        outputCounter = 0;         
    }
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&TankOutputMsg);     
        myUI->Screen->addMessageToQueue(&TankOutputMap1Msg);
        myUI->Screen->addMessageToQueue(&TankOutputMap2Msg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        turnOffTankOutputs();
        changeState(FCT_MAIN_MENU);
        return;
    }
    else if(releasedTouchValue == TOUCH_LEFT_SINGLE_BREW)
    {
        if(outputActive)
        {
            turnOffTankOutputs();
        }
        else
        {
            myTank->openInletValve();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->leftSingleBrewLeds->setColorRed();
        }
    }
    else if(releasedTouchValue == TOUCH_RIGHT_BREW)
    {
        if(outputActive)
        {
            turnOffTankOutputs();
        }
        else
        {
            myTank->turnElementOn();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->rightBrewLeds->setColorRed();
        }
    }   
    
    outputCounter += SYSTEM_MANAGER_TASK_RATE_MS; 
    if(outputCounter > FCT_OUTPUT_TIME_MS)
    {
        turnOffTankOutputs();
    }
}
