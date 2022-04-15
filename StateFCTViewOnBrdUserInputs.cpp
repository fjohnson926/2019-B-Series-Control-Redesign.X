#include "SystemManager.h"
#include "sprintfoverride.h"
#include "gm_SystemPinMap.h"

#define CHAR_LOW '0'
#define CHAR_HIGH '1'
#define PROG_BUTTON_INDEX 4
#define PROG_LOCKOUT_SW_INDEX 10


char SensorLabel[] = "PROG PLOCK";
static char line2buffer[20];
static ScreenMessages OnBoardInputStatusMsg(SensorLabel, line2buffer, 500);

static void updateLineBuffers()
{   
    for(int i = 0; i < 20; i++)
    {
      line2buffer[i]= ' ';
    }
    
    if(gpio_get_pin_level(PROG_BUTTON))
    {
        line2buffer[PROG_BUTTON_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[PROG_BUTTON_INDEX] = CHAR_LOW;
    }
    if(gpio_get_pin_level(PROG_LOCKOUT_SW_PIN))
    {
        line2buffer[PROG_LOCKOUT_SW_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[PROG_LOCKOUT_SW_INDEX] = CHAR_LOW;
    }    
}

void SystemManager::stateFCTViewOnBoardUserInputs()
{    
    if(changedState)
    {
        changedState = false;
        updateLineBuffers();
        myUI->Screen->showMessageNow(&OnBoardInputStatusMsg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();        
        changeState(FCT_MAIN_MENU);
        return;
    }
    
    if(!myUI->Screen->isBusy())
    {   
        updateLineBuffers();
        myUI->Screen->addMessageToQueue(&OnBoardInputStatusMsg);
    }    
}