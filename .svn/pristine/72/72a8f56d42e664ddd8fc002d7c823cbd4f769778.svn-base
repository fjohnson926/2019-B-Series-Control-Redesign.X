#include "SystemManager.h"

static ScreenMessages LEDOutputMsg("FCT LED","OUTPUTS");
static ScreenMessages LEDOutputMap1Msg("CS01","LED1");
static ScreenMessages LEDOutputMap2Msg("CS00","LED2");
static ScreenMessages LEDOutputMap3Msg("CS03","LED3");
static bool outputActive = false;
static int outputCounter = 0;

enum RGB
{
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
};
static int rightBrewLEDColor = COLOR_RED;
static int leftBrewLEDColor  = COLOR_RED;

 void SystemManager::turnOffLEDOutputs(void)
{
    myUI->LEDs->turnOffAllLEDs(true); 
    outputActive = false;
}

void SystemManager::stateManageLEDOutputs()
{
    if(changedState)
    {
        myUI->Screen->showMessageNow(&LEDOutputMsg);
        changedState = false;
        outputActive = false;
        outputCounter = 0;         
    }
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&LEDOutputMsg);
        myUI->Screen->addMessageToQueue(&LEDOutputMap1Msg);
        myUI->Screen->addMessageToQueue(&LEDOutputMap2Msg);        
        myUI->Screen->addMessageToQueue(&LEDOutputMap3Msg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        turnOffLEDOutputs();
        changeState(FCT_MAIN_MENU);
        return;
    }
    else if(releasedTouchValue == TOUCH_LEFT_SINGLE_BREW)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            if(leftBrewLEDColor == COLOR_RED)
            {
              myUI->LEDs->leftSingleBrewLeds->setColorRed();
              leftBrewLEDColor = COLOR_BLUE;
            }
            else if(leftBrewLEDColor == COLOR_BLUE)
            {
              myUI->LEDs->leftSingleBrewLeds->setColorGreen();
              leftBrewLEDColor = COLOR_GREEN;
            }
            else if(leftBrewLEDColor == COLOR_GREEN)
            {
              myUI->LEDs->leftSingleBrewLeds->setColorBlue();
              leftBrewLEDColor = COLOR_RED;
            }
        }
    }
    else if(releasedTouchValue == TOUCH_RIGHT_BREW)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {
            outputCounter = 0;
            outputActive = true;            
            if(rightBrewLEDColor == COLOR_RED)
            {
              myUI->LEDs->rightBrewLeds->setColorRed();
              rightBrewLEDColor = COLOR_BLUE;
            }
            else if(rightBrewLEDColor == COLOR_BLUE)
            {
              myUI->LEDs->rightBrewLeds->setColorGreen();
              rightBrewLEDColor = COLOR_GREEN;
            }
            else if(rightBrewLEDColor == COLOR_GREEN)
            {
              myUI->LEDs->rightBrewLeds->setColorBlue();
              rightBrewLEDColor = COLOR_RED;
            }
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_1)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer01Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_2)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer02Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_3)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer03Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_4)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer04Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_5)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer05Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_6)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer06Led->turnOnLED();
        }
    }    
    else if(releasedTouchValue == TOUCH_NEXT)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->basketLeftLed->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_PREVIOUS)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->basketRightLed->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->basketCenterLed->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_POWER)
    {
        if(outputActive)
        {
            turnOffLEDOutputs();
        }
        else
        {            
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->powerLed->turnOnLED();
        }
    }
    
    
    outputCounter += SYSTEM_MANAGER_TASK_RATE_MS; 
    if(outputCounter > FCT_OUTPUT_TIME_MS)
    {
        turnOffLEDOutputs();
    }
}
