#include "SystemManager.h"

#define FCT_ALL_OUTPUT_TIME_MS 1800000

static ScreenMessages AllOutputMsg("FCT ALL","OUTPUTS ON");
static ScreenMessages AllOutpustMap1Msg("CS11","TOGGLE OUPUTS");
static ScreenMessages AllOutputsMap2Msg("CS06","TOGGLE OUPUTS");
static bool outputsActive = false;
static int outputCounter = 0;

void SystemManager::turnOffAllOutputs(void)
{
    //Tank
    myTank->turnElementOff();
    myTank->closeInletValve();
    //Coffee outputs
    myRightSingleHead->closeDispenseValve();
    myLeftHead->closeDispenseValve();
    myWarmers->myWarmer1->turnOffElement();
    myWarmers->myWarmer2->turnOffElement();
    myWarmers->myWarmer3->turnOffElement();
    myWarmers->myWarmer4->turnOffElement();
    myWarmers->myWarmer5->turnOffElement();
    myWarmers->myWarmer6->turnOffElement();
    //Tea outputs
    initTeaGPIO();
    gpio_set_pin_level(LOCK_PIN, 0);
    gpio_set_pin_level(SIDE_SELECT_PIN, 0);
    gpio_set_pin_level(SUGAR1_PIN, 0);
    gpio_set_pin_level(SUGAR0_PIN, 0);
    gpio_set_pin_level(DIL1_PIN, 0);
    gpio_set_pin_level(DIL0_PIN, 0);
    //LEDs
    myUI->LEDs->turnOffAllLEDs(true);     
    outputsActive = false;
}

void SystemManager::turnOnAllOutputs(void)
{
    //Tank
    myTank->turnElementOn();
    myTank->openInletValve();
    //Coffee outputs
    myRightSingleHead->openDispenseValve();
    myLeftHead->openDispenseValve();
    myWarmers->myWarmer1->turnOnElement();
    myWarmers->myWarmer2->turnOnElement();
    myWarmers->myWarmer3->turnOnElement();
    myWarmers->myWarmer4->turnOnElement();
    myWarmers->myWarmer5->turnOnElement();
    myWarmers->myWarmer6->turnOnElement();
    //Tea outputs
    initTeaGPIO();
    gpio_set_pin_level(LOCK_PIN, 1);
    gpio_set_pin_level(SIDE_SELECT_PIN, 1);
    gpio_set_pin_level(SUGAR1_PIN, 1);
    gpio_set_pin_level(SUGAR0_PIN, 1);
    gpio_set_pin_level(DIL1_PIN, 1);
    gpio_set_pin_level(DIL0_PIN, 1);
    //LEDs
    myUI->LEDs->turnOnAllLEDs();     
    outputsActive = true;
}

void SystemManager::stateFCTAllOutputsOnTest(void)
{
    if(changedState)
    {
        myUI->Screen->showMessageNow(&AllOutputMsg);
        changedState = false;
        outputsActive = false;
        outputCounter = 0;         
    }
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&AllOutputMsg);     
        myUI->Screen->addMessageToQueue(&AllOutpustMap1Msg);
        myUI->Screen->addMessageToQueue(&AllOutputsMap2Msg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        turnOffAllOutputs();
        changeState(FCT_MAIN_MENU);
        return;
    }
    else if(releasedTouchValue == TOUCH_LEFT_SINGLE_BREW)
    {
        if(outputsActive)
        {
            turnOffAllOutputs();
        }
        else
        {
            turnOnAllOutputs();
        }
        outputCounter = 0;
    }
    else if(releasedTouchValue == TOUCH_RIGHT_BREW)
    {
        if(outputsActive)
        {
            turnOffAllOutputs();
        }
        else
        {
            turnOnAllOutputs();
        }
        outputCounter = 0;
    }   
    
    outputCounter += SYSTEM_MANAGER_TASK_RATE_MS; 
    if(outputCounter > FCT_ALL_OUTPUT_TIME_MS)
    {
        turnOffAllOutputs();
    }
}