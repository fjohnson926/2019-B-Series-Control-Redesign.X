#include "SystemManager.h"
#include "NvParmManager.h"

bool timeFormatStateInitialized = false;
bool TimeFormat24HrEnable;

char timeFormatBottomString[20]; 
ScreenMessages TimeFormatMsg("TIME FORMAT", timeFormatBottomString, DEFAULT_STATIC_MENU_PRINT_TIME);

void updateTimeFormatString()
{
    if(TimeFormat24HrEnable)
    {
        sprintf(timeFormatBottomString, "%s", "24 HOUR");
    }
    else
    {
        sprintf(timeFormatBottomString, "%s", "12 HOUR");
    }
}

void SystemManager::stateEditTimeFormat()
{    
    if(!timeFormatStateInitialized)
    {
        TimeFormat24HrEnable = NvParmManager::getNvParm(PARM_INDEX_24HR_TIME_FORMAT_ENABLE).u.bool_parm;
        updateTimeFormatString();
        myUI->Screen->showMessageNow(&TimeFormatMsg);
        timeFormatStateInitialized = true;
    }
    
    if(releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS)
    {
        TimeFormat24HrEnable = !TimeFormat24HrEnable;
        updateTimeFormatString();
        myUI->Screen->showMessageNow(&TimeFormatMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {   
        NvParm TimeFormat24HrEnableParm;
        TimeFormat24HrEnableParm.u.bool_parm = TimeFormat24HrEnable;
        NvParmManager::setNvParm(PARM_INDEX_24HR_TIME_FORMAT_ENABLE, TimeFormat24HrEnableParm);
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        myCalendar->setTimeFormat(TimeFormat24HrEnable);
        timeFormatStateInitialized = false;
        changeState(MENU_BROWSE);        
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {       
        timeFormatStateInitialized = false;
        changeState(MENU_BROWSE);
        return;
    }
}
