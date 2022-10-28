#include "SystemManager.h"
#include "NvParmManager.h"
#include "smath.h"


int uiEditBrewLockoutTimeS = 195;

#define BUTTON_HELD_EVENT_SPEED 1000

//HOLD Stages
#define STAGE1_VALUE 1
#define STAGE1_TIME_MS 6000

#define STAGE2_VALUE 5
#define STAGE2_TIME_MS 11000

#define STAGE3_VALUE 10

#define MIN_BREW_LOCKOUT_TIME_S 0
#define MAX_BREW_LOCKOUT_TIME_S 255

static char editLockoutTimeBuffer[20];
ScreenMessages editLockoutMsg("EDIT DRIP TIME", editLockoutTimeBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);


static void updateLockoutBuffer()
{
    if(uiEditBrewLockoutTimeS)
    {
      sprintf(editLockoutTimeBuffer,"%ds", uiEditBrewLockoutTimeS); // Changed "%ds" to "%s" FJ
    }
    else
    {
      sprintf(editLockoutTimeBuffer,"%DISABLED", uiEditBrewLockoutTimeS);
    }
}

void SystemManager::stateEditBrewLockoutTime()
{
    
    if(changedState)
    {
        uiEditBrewLockoutTimeS = NvParmManager::getNvParm(PARM_INDEX_BREW_LOCKOUT_TIME_S).u.integer_parm;
        updateLockoutBuffer();
        myUI->Screen->showMessageNow(&editLockoutMsg);
        changedState = false;
    }        
    //Touch NEXT management
    if(releasedTouchValue == TOUCH_NEXT && uiEditBrewLockoutTimeS < MAX_BREW_LOCKOUT_TIME_S)    
    {
        uiEditBrewLockoutTimeS++;
        updateLockoutBuffer();
        myUI->Screen->showMessageNow(&editLockoutMsg);
    }    
    else if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs < STAGE1_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
    {
        if(uiEditBrewLockoutTimeS + STAGE1_VALUE < MAX_BREW_LOCKOUT_TIME_S)
        {
            uiEditBrewLockoutTimeS += STAGE1_VALUE;
            updateLockoutBuffer();
            myUI->Screen->showMessageNow(&editLockoutMsg);
        }
    }    
    else if(holdTouchValue == TOUCH_NEXT && betweenValues(holdTouchTimeMs, STAGE1_TIME_MS, STAGE2_TIME_MS)&& holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
    {
        if(uiEditBrewLockoutTimeS + STAGE2_VALUE < MAX_BREW_LOCKOUT_TIME_S)
        {
            uiEditBrewLockoutTimeS += STAGE2_VALUE;
            updateLockoutBuffer();
            myUI->Screen->showMessageNow(&editLockoutMsg);
        }
    }    
    else if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= STAGE2_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
    {
        if(uiEditBrewLockoutTimeS + STAGE3_VALUE < MAX_BREW_LOCKOUT_TIME_S)
        {
            uiEditBrewLockoutTimeS += STAGE3_VALUE;
            updateLockoutBuffer();
            myUI->Screen->showMessageNow(&editLockoutMsg);
        }
    }   
    
    //Touch Previous management
    else if(releasedTouchValue == TOUCH_PREVIOUS && uiEditBrewLockoutTimeS > 0)
    {
        uiEditBrewLockoutTimeS--;
        updateLockoutBuffer();
        myUI->Screen->showMessageNow(&editLockoutMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs < STAGE1_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
    {
        if(uiEditBrewLockoutTimeS - STAGE1_VALUE > MIN_BREW_LOCKOUT_TIME_S)
        {
            uiEditBrewLockoutTimeS-=STAGE1_VALUE;
            updateLockoutBuffer();
            myUI->Screen->showMessageNow(&editLockoutMsg);
        }
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && betweenValues(holdTouchTimeMs, STAGE1_TIME_MS, STAGE2_TIME_MS) && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
    {
        if(uiEditBrewLockoutTimeS - STAGE2_VALUE > MIN_BREW_LOCKOUT_TIME_S)
        {
            uiEditBrewLockoutTimeS -= STAGE2_VALUE;
            updateLockoutBuffer();
            myUI->Screen->showMessageNow(&editLockoutMsg);
        }
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= STAGE2_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
    {
        if(uiEditBrewLockoutTimeS - STAGE3_VALUE > MIN_BREW_LOCKOUT_TIME_S)
        {
            uiEditBrewLockoutTimeS -= STAGE3_VALUE;
            updateLockoutBuffer();
            myUI->Screen->showMessageNow(&editLockoutMsg);
        }
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {   
        NvParm lockoutParm;
        lockoutParm.u.integer_parm = uiEditBrewLockoutTimeS;
        NvParmManager::setNvParm(PARM_INDEX_BREW_LOCKOUT_TIME_S, lockoutParm);
        DispenseManager::setBrewLockoutTime(uiEditBrewLockoutTimeS, PARMDEFAULT_BREW_LOCKOUT_TIME_S);  
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        myUI->Screen->resetScreen();        
        changeState(MENU_BROWSE);
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();        
        changeState(MENU_BROWSE);        
        return;
    }
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&editLockoutMsg);
    }    
}