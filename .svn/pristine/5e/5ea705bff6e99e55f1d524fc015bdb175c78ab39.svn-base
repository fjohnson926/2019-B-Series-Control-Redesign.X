#include "SystemManager.h"
#include "sprintfoverride.h"
#include "gm_SystemPinMap.h"

#define CHAR_LOW '0'
#define CHAR_HIGH '1'

#define CS00_INDEX    3
#define CS01_INDEX      CS00_INDEX + 1
#define CS02_INDEX      CS01_INDEX + 1
#define CS03_INDEX      CS02_INDEX + 1
#define CS04_INDEX      CS03_INDEX + 1
#define CS05_INDEX      CS04_INDEX + 1
#define CS06_INDEX      CS05_INDEX + 1
#define CS07_INDEX      CS06_INDEX + 1
#define CS08_INDEX      CS07_INDEX + 1
#define CS09_INDEX      CS08_INDEX + 1
#define CS10_INDEX      CS09_INDEX + 1
#define CS11_INDEX      CS10_INDEX + 1
#define CS12_INDEX      CS11_INDEX + 1




char OnBoardInputLabel[] = "SEN0123456789ABC";

static char line2buffer[20];
static ScreenMessages TouchSensorStatusMsg(OnBoardInputLabel, line2buffer, 500);

static void updateLineBuffers(uint16_t currentTouchValue)
{   
    for(int i = 0; i < 20; i++)
    {
      line2buffer[i]= ' ';
    }
    
    if(currentTouchValue & TOUCH_ACCEPT)
    {
        line2buffer[CS00_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS00_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_NEXT)
    {
        line2buffer[CS01_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS01_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_CANCEL)
    {
        line2buffer[CS02_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS02_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_PREVIOUS)
    {
        line2buffer[CS03_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS03_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_POWER)
    {
        line2buffer[CS04_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS04_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_WARM_1)
    {
        line2buffer[CS05_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS05_INDEX] = CHAR_LOW;
    }
    
        
    if(currentTouchValue & TOUCH_LEFT_SINGLE_BREW)
    {
        line2buffer[CS06_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS06_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_WARM_3)
    {
        line2buffer[CS07_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS07_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_WARM_5)
    {
        line2buffer[CS08_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS08_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_WARM_6)
    {
        line2buffer[CS09_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS09_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_WARM_4)
    {
        line2buffer[CS10_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS10_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_RIGHT_BREW)
    {
        line2buffer[CS11_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS11_INDEX] = CHAR_LOW;
    }
    
    if(currentTouchValue & TOUCH_WARM_2)
    {
        line2buffer[CS12_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[CS12_INDEX] = CHAR_LOW;
    }
}

void SystemManager::stateFCTViewTouchSensorStatus()
{    
    if(changedState)
    {
        changedState = false;
        updateLineBuffers(currentTouchValue);
        myUI->Screen->showMessageNow(&TouchSensorStatusMsg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();        
        changeState(FCT_MAIN_MENU);
        return;
    }
    
    if(!myUI->Screen->isBusy())
    {   
        updateLineBuffers(currentTouchValue);
        myUI->Screen->addMessageToQueue(&TouchSensorStatusMsg);
    }    
}