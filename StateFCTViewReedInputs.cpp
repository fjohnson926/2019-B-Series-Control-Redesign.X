#include "SystemManager.h"
#include "sprintfoverride.h"
#include "gm_SystemPinMap.h"

#define CHAR_LOW '0'
#define CHAR_HIGH '1'
#define REED1_INDEX 4
#define REED2_INDEX REED1_INDEX + 1
#define REED3_INDEX REED2_INDEX + 1
#define REED4_INDEX REED3_INDEX + 1

char ReedLabel[] = "REED 1234";
static char line2buffer[20];
static ScreenMessages ReedStatusMsg(ReedLabel, line2buffer, 500);

static void updateLineBuffers()
{   
    line2buffer[0] = ' ';
    line2buffer[1] = ' ';
    line2buffer[2] = ' ';
    line2buffer[3] = ' ';
    
    if(gpio_get_pin_level(REED0_PIN))
    {
        line2buffer[REED1_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[REED1_INDEX] = CHAR_LOW;
    }
    if(gpio_get_pin_level(REED1_PIN))
    {
        line2buffer[REED2_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[REED2_INDEX] = CHAR_LOW;
    }
    if(gpio_get_pin_level(REED2_PIN))
    {
        line2buffer[REED3_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[REED3_INDEX] = CHAR_LOW;
    }
        if(gpio_get_pin_level(REED3_PIN))
    {
        line2buffer[REED4_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[REED4_INDEX] = CHAR_LOW;
    }
}

void SystemManager::stateFCTViewReedInputs()
{    
    if(changedState)
    {
        changedState = false;
        updateLineBuffers();
        myUI->Screen->showMessageNow(&ReedStatusMsg);
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
        myUI->Screen->addMessageToQueue(&ReedStatusMsg);
    }    
}