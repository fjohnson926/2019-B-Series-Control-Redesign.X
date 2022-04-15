#include "SystemManager.h"

static ScreenMessages CoffeeOutputMsg("FCT COFFEE","OUTPUTS");
static ScreenMessages CoffeeOutputMap1Msg("CS6", "DMPL");
static ScreenMessages CoffeeOutputMap2Msg("CS11","DMPRS");
static ScreenMessages CoffeeOutputMap3Msg("CS05","WARMER0");
static ScreenMessages CoffeeOutputMap4Msg("CS12","WARMER1");
static ScreenMessages CoffeeOutputMap5Msg("CS07","WARMER2");
static ScreenMessages CoffeeOutputMap6Msg("CS10","WARMER3");
static ScreenMessages CoffeeOutputMap7Msg("CS08","WARMER4");
static ScreenMessages CoffeeOutputMap8Msg("CS09","WARMER5");

static bool outputActive = false;
static int outputCounter = 0;

 void SystemManager::turnOffCoffeeOutputs(void)
{
    myLeftHead->closeDispenseValve();
    myRightSingleHead->closeDispenseValve();
    myWarmers->myWarmer1->turnOffElement();
    myWarmers->myWarmer2->turnOffElement();
    myWarmers->myWarmer3->turnOffElement();
    myWarmers->myWarmer4->turnOffElement();
    myWarmers->myWarmer5->turnOffElement();
    myWarmers->myWarmer6->turnOffElement();
    myUI->LEDs->turnOffAllLEDs(true); 
    outputActive = false;
}

void SystemManager::stateManageCoffeeOutputs()
{
    if(changedState)
    {
        myUI->Screen->showMessageNow(&CoffeeOutputMsg);
        changedState = false;
        outputActive = false;
        outputCounter = 0;         
    }
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&CoffeeOutputMsg);
        myUI->Screen->addMessageToQueue(&CoffeeOutputMap1Msg);
        myUI->Screen->addMessageToQueue(&CoffeeOutputMap2Msg);        
        myUI->Screen->addMessageToQueue(&CoffeeOutputMap3Msg);
        myUI->Screen->addMessageToQueue(&CoffeeOutputMap4Msg);
        myUI->Screen->addMessageToQueue(&CoffeeOutputMap5Msg);
        myUI->Screen->addMessageToQueue(&CoffeeOutputMap6Msg);
        myUI->Screen->addMessageToQueue(&CoffeeOutputMap7Msg);
        myUI->Screen->addMessageToQueue(&CoffeeOutputMap8Msg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        turnOffCoffeeOutputs();
        changeState(FCT_MAIN_MENU);
        return;
    }
    else if(releasedTouchValue == TOUCH_LEFT_SINGLE_BREW)
    {
        if(outputActive)
        {
            turnOffCoffeeOutputs();
        }
        else
        {
            myLeftHead->openDispenseValve();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->leftSingleBrewLeds->setColorRed();
        }
    }
    else if(releasedTouchValue == TOUCH_RIGHT_BREW)
    {
        if(outputActive)
        {
            turnOffCoffeeOutputs();
        }
        else
        {
            myRightSingleHead->openDispenseValve();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->rightBrewLeds->setColorRed();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_1)
    {
        if(outputActive)
        {
            turnOffCoffeeOutputs();
        }
        else
        {
            myWarmers->myWarmer1->turnOnElement();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer01Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_2)
    {
        if(outputActive)
        {
            turnOffCoffeeOutputs();
        }
        else
        {
            myWarmers->myWarmer2->turnOnElement();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer02Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_3)
    {
        if(outputActive)
        {
            turnOffCoffeeOutputs();
        }
        else
        {
            myWarmers->myWarmer3->turnOnElement();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer03Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_4)
    {
        if(outputActive)
        {
            turnOffCoffeeOutputs();
        }
        else
        {
            myWarmers->myWarmer4->turnOnElement();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer04Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_5)
    {
        if(outputActive)
        {
            turnOffCoffeeOutputs();
        }
        else
        {
            myWarmers->myWarmer5->turnOnElement();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer05Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_6)
    {
        if(outputActive)
        {
            turnOffCoffeeOutputs();
        }
        else
        {
            myWarmers->myWarmer6->turnOnElement();
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer06Led->turnOnLED();
        }
    }
    
    outputCounter += SYSTEM_MANAGER_TASK_RATE_MS; 
    if(outputCounter > FCT_OUTPUT_TIME_MS)
    {
        turnOffCoffeeOutputs();
    }
}
