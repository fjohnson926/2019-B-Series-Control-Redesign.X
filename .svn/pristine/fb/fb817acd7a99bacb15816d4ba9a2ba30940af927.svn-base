#include "SystemManager.h"
#include "NvParmManager.h"

void updateLowTempNoBrewBuffer(bool enable, char * buffer)
{
    if(enable)
    {
        sprintf(buffer,"ENABLED");
    }
    else
    {
        sprintf(buffer,"DISABLED");
    }
}

void SystemManager::StateEditLowTempNoBrew(void)
{
    static bool initialized = false;     
    static NvParm lowTempNoBrewParm;
    static char lowTempNoBrewBuffer[20];
    static ScreenMessages LowTempNoBrewEnableMsg("LOW TEMP NO BREW", lowTempNoBrewBuffer);
    if(!initialized)
    {
        lowTempNoBrewParm = NvParmManager::getNvParm(PARMINDEX_LOW_TEMP_NO_BREW);        
        updateLowTempNoBrewBuffer(lowTempNoBrewParm.u.bool_parm, lowTempNoBrewBuffer);
        myUI->Screen->showMessageNow(&LowTempNoBrewEnableMsg);
        initialized = true;
    }
    
    if( (releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS))
    {
        lowTempNoBrewParm.u.bool_parm = !lowTempNoBrewParm.u.bool_parm;   
        updateLowTempNoBrewBuffer(lowTempNoBrewParm.u.bool_parm, lowTempNoBrewBuffer);        
        myUI->Screen->showMessageNow(&LowTempNoBrewEnableMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {           
        NvParmManager::setNvParm(PARMINDEX_LOW_TEMP_NO_BREW, lowTempNoBrewParm);
        lowTempNoBrewEnabled = lowTempNoBrewParm.u.bool_parm;
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
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


