#include "SystemManager.h"
#include "NvParmManager.h"

void updateResetStringBuffer(bool enable, char * buffer)
{
    if(enable)
    {
        sprintf(buffer,"YES");
    }
    else
    {
        sprintf(buffer,"NO");
    }
}

void SystemManager::StateResetSystemSettings(void)
{
    static bool initialized = false;   
    static char resetSystemSettingsBuffer[20];
    static bool resetSettings = false;
    static ScreenMessages ResetSystemSettingsMsg("RESET SETTINGS?", resetSystemSettingsBuffer);
    static ScreenMessages RestartSystemMsg("RESTART", "MACHINE");
    if(!initialized)
    { 
        resetSettings = false;
        updateResetStringBuffer(resetSettings, resetSystemSettingsBuffer);
        myUI->Screen->showMessageNow(&ResetSystemSettingsMsg);
        initialized = true;
    }
    
    if( (releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS))
    {
        resetSettings = !resetSettings;   
        updateResetStringBuffer(resetSettings, resetSystemSettingsBuffer);        
        myUI->Screen->showMessageNow(&ResetSystemSettingsMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {   
        if(resetSettings)
        {
            resetParmsToDefaultValues__c();        
            NVBlobs->flushNvBlob(PARMS_READ_INDEX);        
            setCoffeeBeverageRecipesToDefault();
            NVBlobs->flushNvBlob(COFFEE_RECIPE_BLOB_INDEX);
            myUI->Screen->showMessageNow(&RestartSystemMsg);
            while(myUI->Screen->isBusy())
            {
                osSupportedDelayMs(100);
            }
        }        
        initialized = false;
        changeState(MENU_BROWSE);
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {        
        initialized = false;
        changeState(MENU_BROWSE);
        return;
    }    
}


