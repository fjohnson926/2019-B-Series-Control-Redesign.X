#include "SystemManager.h"
#include "NvParmManager.h"

#define FIRST_VALUE_HOLD_TIME_MS 200
#define FAST_VALUE_HOLD_TIME_MS 1500
#define SLOW_VALUE_DELTA_LTIME 1
#define FAST_VALUE_DELTA_LTIME 1
#define FAST_VALUE_UPDATE_SPEED_MS 1000


bool uiEditEnergySavingsEnable = true;
bool uiAutoExitEnable = false;
int uiEnergySavingsAutoWakeupHour = 9;
int uiEnergySavingsWakeupMin = 30;
int uiEnergySavingsWhenIdleMins = 300;

int EnergySavingsState = 0;
#define STATE_ENABLE_DISABLE_ENERGY_SAVINGS 0
#define STATE_ENERGY_SAVINGS_EDIT_IDLE_TIME 1
#define STATE_ENABLE_DISABLE_AUTO_WAKEUP 2
#define STATE_AUTO_WAKEUP_HOUR 3
#define STATE_WAKEUP_MIN 4


#define DELTA_IDLE_TIME_MINS 30

bool editEnergySavingsNewStateFlag = true;
char editEnergySavingsEnableString[20];
ScreenMessages editEnergySavingsMsg("ENERGY SAVINGS", editEnergySavingsEnableString, DEFAULT_STATIC_MENU_PRINT_TIME);
ScreenMessages editEnergySavingsHourMsg("WAKEUP HOUR", editEnergySavingsEnableString, DEFAULT_STATIC_MENU_PRINT_TIME);
ScreenMessages editEnergySavingsMinMsg("WAKEUP MIN", editEnergySavingsEnableString, DEFAULT_STATIC_MENU_PRINT_TIME);
ScreenMessages editAutoExitEnergySavingsMsg("AUTO EXIT", editEnergySavingsEnableString, DEFAULT_STATIC_MENU_PRINT_TIME);
ScreenMessages editIdleTimeMsg("IDLE TIME MINS", editEnergySavingsEnableString, DEFAULT_STATIC_MENU_PRINT_TIME);
bool handlerInitialized = false;


void SystemManager::energySavingsEnableDisableHandler(void)
{
    if(editEnergySavingsNewStateFlag)
    {
        NvParm energyEnableParm;
        energyEnableParm = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_ENABLE);        
        uiEditEnergySavingsEnable = energyEnableParm.u.bool_parm;
        editEnergySavingsNewStateFlag = false;
        if(uiEditEnergySavingsEnable)
        {
            sprintf(editEnergySavingsEnableString,"%s","ENABLED");
        }
        else
        {
            sprintf(editEnergySavingsEnableString,"%s","DISABLED");
        }
        myUI->Screen->showMessageNow(&editEnergySavingsMsg);
    }
    if(releasedTouchValue == TOUCH_NEXT)
    {
        uiEditEnergySavingsEnable = !uiEditEnergySavingsEnable;        
    }
    else if(releasedTouchValue == TOUCH_PREVIOUS)
    {
        uiEditEnergySavingsEnable = !uiEditEnergySavingsEnable;       
    }
    else if(releasedTouchValue == TOUCH_ACCEPT && !editEnergySavingsNewStateFlag)
    {   
        NvParm energyEnableParm;
        energyEnableParm.u.bool_parm = uiEditEnergySavingsEnable;
        NvParmManager::setNvParm(PARM_INDEX_ENERGY_SAVINGS_ENABLE, energyEnableParm);
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        myUI->Screen->resetScreen();       
        editEnergySavingsNewStateFlag = true;
        if(uiEditEnergySavingsEnable)
        {
            EnergySavingsState = STATE_ENERGY_SAVINGS_EDIT_IDLE_TIME;
        }
        else
        {
            systemState = MENU_BROWSE;
        }        
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {           
        changeState(MENU_BROWSE);
        editEnergySavingsNewStateFlag = true;       
        return;
    }
    if(releasedTouchValue == TOUCH_NEXT ||  releasedTouchValue == TOUCH_PREVIOUS)
    {
        if(uiEditEnergySavingsEnable)
        {
            sprintf(editEnergySavingsEnableString,"%s","ENABLED");
        }
        else
        {
            sprintf(editEnergySavingsEnableString,"%s","DISABLED");
        }
        myUI->Screen->showMessageNow(&editEnergySavingsMsg);
    }
}

void SystemManager::energySavingsIdleTimeHandler(void)
{
    if(editEnergySavingsNewStateFlag)
    {
        NvParm energySavingsIdleTimeParm;
        energySavingsIdleTimeParm = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_IDLE_MINS);
        uiEnergySavingsWhenIdleMins = energySavingsIdleTimeParm.u.integer_parm;
        editEnergySavingsNewStateFlag = false;
        sprintf(editEnergySavingsEnableString,"%d",uiEnergySavingsWhenIdleMins);
        myUI->Screen->showMessageNow(&editIdleTimeMsg);        
    }
    if(releasedTouchValue == TOUCH_NEXT)
    {
        if(uiEnergySavingsWhenIdleMins < MAX_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS)
        {
            uiEnergySavingsWhenIdleMins += DELTA_IDLE_TIME_MINS;
            sprintf(editEnergySavingsEnableString,"%d",uiEnergySavingsWhenIdleMins);
            myUI->Screen->showMessageNow(&editIdleTimeMsg);
        }
    }
    else if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0)
    {
        if(uiEnergySavingsWhenIdleMins < MAX_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS)
        {
            uiEnergySavingsWhenIdleMins += DELTA_IDLE_TIME_MINS;
            sprintf(editEnergySavingsEnableString,"%d",uiEnergySavingsWhenIdleMins);
            myUI->Screen->showMessageNow(&editIdleTimeMsg);
        }
    }    
    else if(releasedTouchValue == TOUCH_PREVIOUS)
    {
        if(uiEnergySavingsWhenIdleMins > MIN_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS)
        {
            uiEnergySavingsWhenIdleMins -= DELTA_IDLE_TIME_MINS;
            sprintf(editEnergySavingsEnableString,"%d",uiEnergySavingsWhenIdleMins);
            myUI->Screen->showMessageNow(&editIdleTimeMsg);
        }       
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0)
    {
        if(uiEnergySavingsWhenIdleMins > MIN_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS)
        {
            uiEnergySavingsWhenIdleMins -= DELTA_IDLE_TIME_MINS;
            sprintf(editEnergySavingsEnableString,"%d",uiEnergySavingsWhenIdleMins);
            myUI->Screen->showMessageNow(&editIdleTimeMsg);
        }       
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {   
        NvParm energySavingsIdleTimeParm;
        energySavingsIdleTimeParm.u.integer_parm = uiEnergySavingsWhenIdleMins;
        NvParmManager::setNvParm(PARM_INDEX_ENERGY_SAVINGS_IDLE_MINS, energySavingsIdleTimeParm);
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        myUI->Screen->resetScreen();       
        editEnergySavingsNewStateFlag = true;        
        EnergySavingsState = STATE_ENABLE_DISABLE_AUTO_WAKEUP;
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {               
        EnergySavingsState = STATE_ENABLE_DISABLE_ENERGY_SAVINGS;
        editEnergySavingsNewStateFlag = true;
        handlerInitialized = false;        
        return;
    }
}

void SystemManager::enableDisableAutoExitEnergySavings(void)
{
    if(editEnergySavingsNewStateFlag)
    {
        NvParm energyAutoExitParm;
        energyAutoExitParm = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_AUTO_EXIT_ENABLE);
        uiAutoExitEnable = energyAutoExitParm.u.bool_parm;
        editEnergySavingsNewStateFlag = false;
        if(uiAutoExitEnable)
        {
            sprintf(editEnergySavingsEnableString,"%s","ENABLED");
        }
        else
        {
            sprintf(editEnergySavingsEnableString,"%s","DISABLED");
        }
        myUI->Screen->showMessageNow(&editAutoExitEnergySavingsMsg);
    }
    if(releasedTouchValue == TOUCH_NEXT)
    {
        uiAutoExitEnable = !uiAutoExitEnable;        
    }
        else if(releasedTouchValue == TOUCH_PREVIOUS)
    {
        uiAutoExitEnable = !uiAutoExitEnable;       
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {   
        NvParm autoExitEnergySavingsParm;
        autoExitEnergySavingsParm.u.bool_parm = uiAutoExitEnable;
        NvParmManager::setNvParm(PARM_INDEX_ENERGY_SAVINGS_AUTO_EXIT_ENABLE, autoExitEnergySavingsParm);
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        myUI->Screen->resetScreen();       
        editEnergySavingsNewStateFlag = true;
        if(uiAutoExitEnable)
        {
            EnergySavingsState = STATE_AUTO_WAKEUP_HOUR;
        }
        else
        {
            EnergySavingsState = STATE_AUTO_WAKEUP_HOUR;
            changeState(MENU_BROWSE);
        }        
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();        
        changeState(MENU_BROWSE);
        editEnergySavingsNewStateFlag = true;
        handlerInitialized = false;        
        return;
    }
    if(releasedTouchValue == TOUCH_NEXT ||  releasedTouchValue == TOUCH_PREVIOUS)
    {
        if(uiAutoExitEnable)
        {
            sprintf(editEnergySavingsEnableString,"%s","ENABLED");
        }
        else
        {
            sprintf(editEnergySavingsEnableString,"%s","DISABLED");
        }
        myUI->Screen->showMessageNow(&editAutoExitEnergySavingsMsg);
    }
}

void SystemManager::energySavingsAutoWakeupHourHandler(void)
{
    if(editEnergySavingsNewStateFlag)
    {
        NvParm energyAutoExitHr;
        energyAutoExitHr = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_EXIT_HOUR);
        uiEnergySavingsAutoWakeupHour = energyAutoExitHr.u.integer_parm;
        editEnergySavingsNewStateFlag = false;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsAutoWakeupHour);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsHourMsg);
    }
    
    if(releasedTouchValue == TOUCH_NEXT && uiEnergySavingsAutoWakeupHour + 1 < 24)
    {        
        uiEnergySavingsAutoWakeupHour += SLOW_VALUE_DELTA_LTIME;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsAutoWakeupHour);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsHourMsg);
    }
    else if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEnergySavingsAutoWakeupHour + FAST_VALUE_DELTA_LTIME < 24)
    {
        uiEnergySavingsAutoWakeupHour += FAST_VALUE_DELTA_LTIME;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsAutoWakeupHour);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsHourMsg);
    }    
    
    else if(releasedTouchValue == TOUCH_PREVIOUS && uiEnergySavingsAutoWakeupHour - SLOW_VALUE_DELTA_LTIME >= 0)
    {
        uiEnergySavingsAutoWakeupHour -= SLOW_VALUE_DELTA_LTIME;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsAutoWakeupHour);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsHourMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEnergySavingsAutoWakeupHour - FAST_VALUE_DELTA_LTIME >= 0)
    {
        uiEnergySavingsAutoWakeupHour -= FAST_VALUE_DELTA_LTIME;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsAutoWakeupHour);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsHourMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {   
        NvParm autoWakeupHr;
        autoWakeupHr.u.integer_parm = uiEnergySavingsAutoWakeupHour;
        NvParmManager::setNvParm(PARM_INDEX_ENERGY_SAVINGS_EXIT_HOUR, autoWakeupHr);
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        myUI->Screen->resetScreen();       
        editEnergySavingsNewStateFlag = true;        
        EnergySavingsState = STATE_WAKEUP_MIN;
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {               
        EnergySavingsState = STATE_ENABLE_DISABLE_AUTO_WAKEUP;
        editEnergySavingsNewStateFlag = true;               
        return;
    }
}

void SystemManager::energySavingsAutoWakeupMinHandler(void)
{
    if(editEnergySavingsNewStateFlag)
    {
        NvParm energyAutoExitMin;
        energyAutoExitMin = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_EXIT_MINUTE);
        uiEnergySavingsWakeupMin = energyAutoExitMin.u.integer_parm;
        editEnergySavingsNewStateFlag = false;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsWakeupMin);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsMinMsg);
    }

    if(releasedTouchValue == TOUCH_NEXT && uiEnergySavingsWakeupMin + SLOW_VALUE_DELTA_LTIME < 60)
    {        
        uiEnergySavingsWakeupMin += SLOW_VALUE_DELTA_LTIME;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsWakeupMin);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsMinMsg);
    }
    else if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEnergySavingsWakeupMin + FAST_VALUE_DELTA_LTIME < 60)
    {
        uiEnergySavingsWakeupMin += FAST_VALUE_DELTA_LTIME;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsWakeupMin);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsMinMsg);
    }    
    
    else if(releasedTouchValue == TOUCH_PREVIOUS && uiEnergySavingsWakeupMin - SLOW_VALUE_DELTA_LTIME >= 0)
    {
        uiEnergySavingsWakeupMin -= SLOW_VALUE_DELTA_LTIME;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsWakeupMin);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsMinMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEnergySavingsWakeupMin - FAST_VALUE_DELTA_LTIME >= 0)
    {
        uiEnergySavingsWakeupMin -= FAST_VALUE_DELTA_LTIME;
        u32ToString(editEnergySavingsEnableString, uiEnergySavingsWakeupMin);
        editEnergySavingsEnableString[2] = 0;        
        myUI->Screen->showMessageNow(&editEnergySavingsMinMsg);
    }    
    
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {   
        NvParm autoWakeupMin;
        autoWakeupMin.u.integer_parm = uiEnergySavingsWakeupMin;
        NvParmManager::setNvParm(PARM_INDEX_ENERGY_SAVINGS_EXIT_MINUTE, autoWakeupMin);
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        myUI->Screen->resetScreen();       
        editEnergySavingsNewStateFlag = true;        
        handlerInitialized = false;
        changeState(MENU_BROWSE);
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {
        EnergySavingsState = STATE_AUTO_WAKEUP_HOUR;
        editEnergySavingsNewStateFlag = true;
        return;
    }
}

void SystemManager::stateEditEnergySavingsMainHandler()
{
    if(!handlerInitialized)
    {  
        EnergySavingsState = STATE_ENABLE_DISABLE_ENERGY_SAVINGS;
        handlerInitialized = true;
    }    
    switch(EnergySavingsState)
    {
        case STATE_ENABLE_DISABLE_ENERGY_SAVINGS:
            energySavingsEnableDisableHandler();
            break;
        case STATE_ENERGY_SAVINGS_EDIT_IDLE_TIME:
            energySavingsIdleTimeHandler();
            break;
        case STATE_ENABLE_DISABLE_AUTO_WAKEUP:
            enableDisableAutoExitEnergySavings();
            break;
        case STATE_AUTO_WAKEUP_HOUR:
            energySavingsAutoWakeupHourHandler();
            break;
        case STATE_WAKEUP_MIN:
            energySavingsAutoWakeupMinHandler();
            break;
    }
}