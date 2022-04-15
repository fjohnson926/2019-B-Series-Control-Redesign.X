#include "SystemManager.h"
#include "sprintfoverride.h"
#include "gm_SystemPinMap.h"

#define CHAR_LOW '0'
#define CHAR_HIGH '1'
#define DIPSW1_INDEX 6
#define DIPSW2_INDEX DIPSW1_INDEX + 1
#define DIPSW3_INDEX DIPSW2_INDEX + 1
#define DIPSW4_INDEX DIPSW3_INDEX + 1
#define DIPSW5_INDEX DIPSW4_INDEX + 1
#define DIPSW6_INDEX DIPSW5_INDEX + 1

char DipSWLabel[] = "DIPSW 123456";
static char line2buffer[20];
static ScreenMessages DipSWStatusMsg(DipSWLabel, line2buffer, 500);

static void updateLineBuffers()
{   
    line2buffer[0] = ' ';
    line2buffer[1] = ' ';
    line2buffer[2] = ' ';
    line2buffer[3] = ' ';
    line2buffer[4] = ' ';
    line2buffer[5] = ' ';
    
    if(gpio_get_pin_level(MODEL_BIT1_PIN))
    {
        line2buffer[DIPSW1_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[DIPSW1_INDEX] = CHAR_LOW;
    }
    if(gpio_get_pin_level(MODEL_BIT2_PIN))
    {
        line2buffer[DIPSW2_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[DIPSW2_INDEX] = CHAR_LOW;
    }
    if(gpio_get_pin_level(MODEL_BIT3_PIN))
    {
        line2buffer[DIPSW3_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[DIPSW3_INDEX] = CHAR_LOW;
    }
    if(gpio_get_pin_level(MODEL_BIT4_PIN))
    {
        line2buffer[DIPSW4_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[DIPSW4_INDEX] = CHAR_LOW;
    }
    if(gpio_get_pin_level(MODEL_BIT5_PIN))
    {
        line2buffer[DIPSW5_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[DIPSW5_INDEX] = CHAR_LOW;
    }
    if(gpio_get_pin_level(MODEL_BIT6_PIN))
    {
        line2buffer[DIPSW6_INDEX] = CHAR_HIGH;
    }
    else
    {
        line2buffer[DIPSW6_INDEX] = CHAR_LOW;
    }
}

void SystemManager::stateFCTViewDipSwitchInputs()
{    
    if(changedState)
    {
        changedState = false;
        updateLineBuffers();
        myUI->Screen->showMessageNow(&DipSWStatusMsg);
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
        myUI->Screen->addMessageToQueue(&DipSWStatusMsg);
    }    
}