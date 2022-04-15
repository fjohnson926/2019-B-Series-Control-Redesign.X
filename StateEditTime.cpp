#include "SystemManager.h"

#define EDIT_HOUR 0
#define EDIT_MINUTE 1

#define FIRST_VALUE_HOLD_TIME_MS 200
#define FAST_VALUE_HOLD_TIME_MS 1500
#define SLOW_VALUE_DELTA_LTIME 1
#define FAST_VALUE_DELTA_LTIME 1
#define FAST_VALUE_UPDATE_SPEED_MS 1000


bool editTimeVariablesLoaded = false;


uint8_t uiEditHour;
uint8_t uiEditMinute;

uint8_t currentEditTimeState = EDIT_HOUR;

char editTimeBuffer[20];
ScreenMessages EditHourMsg("EDIT HOUR", editTimeBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
ScreenMessages EditMinuteMsg("EDIT MINUTE", editTimeBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);

void SystemManager::stateEditTime()
{
    if(!editTimeVariablesLoaded)
    {
        uiEditHour = myCalendar->getHour();
        uiEditMinute = myCalendar->getMinute();
        currentEditTimeState = EDIT_HOUR;
        editTimeVariablesLoaded = true;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());  
        myUI->Screen->showMessageNow(&EditHourMsg);              
    }
    switch(currentEditTimeState)
    {
        case EDIT_HOUR:
            editHour();
            break;   
        case EDIT_MINUTE:
            editMinute();
            break;
        default:
            break;
    }
}

void SystemManager::editHour()
{    
    if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs == FIRST_VALUE_HOLD_TIME_MS  && uiEditHour + SLOW_VALUE_DELTA_LTIME < 24)
    {
        uiEditHour+= SLOW_VALUE_DELTA_LTIME;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->showMessageNow(&EditHourMsg);
    }
    else if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEditHour + FAST_VALUE_DELTA_LTIME < 24)
    {
        uiEditHour+= FAST_VALUE_DELTA_LTIME;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->showMessageNow(&EditHourMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs == FIRST_VALUE_HOLD_TIME_MS && uiEditHour - SLOW_VALUE_DELTA_LTIME >= 0)
    {
        uiEditHour-= SLOW_VALUE_DELTA_LTIME;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->showMessageNow(&EditHourMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEditHour - FAST_VALUE_DELTA_LTIME >= 0)
    {
        uiEditHour-= FAST_VALUE_DELTA_LTIME;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->showMessageNow(&EditHourMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {
        currentEditTimeState = EDIT_MINUTE;
        myUI->Screen->resetScreen();
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {
        editTimeVariablesLoaded = false;        
        changeState(MENU_BROWSE);
    }
}

void SystemManager::editMinute()
{    
    if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs == FIRST_VALUE_HOLD_TIME_MS  && uiEditMinute + SLOW_VALUE_DELTA_LTIME < 60)
    {
        uiEditMinute+= SLOW_VALUE_DELTA_LTIME;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->showMessageNow(&EditMinuteMsg);
    }
    else if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEditMinute + FAST_VALUE_DELTA_LTIME < 60)
    {
        uiEditMinute+= FAST_VALUE_DELTA_LTIME;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->showMessageNow(&EditMinuteMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs == FIRST_VALUE_HOLD_TIME_MS && uiEditMinute - SLOW_VALUE_DELTA_LTIME >= 0)
    {
        uiEditMinute-= SLOW_VALUE_DELTA_LTIME;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->showMessageNow(&EditMinuteMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEditMinute - FAST_VALUE_DELTA_LTIME >= 0)
    {
        uiEditMinute-= FAST_VALUE_DELTA_LTIME;
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->showMessageNow(&EditMinuteMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {       
        editTimeVariablesLoaded = false;
        myCalendar->setTime(uiEditHour, uiEditMinute);
        changeState(MENU_BROWSE);
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {
        editTimeVariablesLoaded = false;
        currentEditTimeState = EDIT_HOUR;
        myUI->Screen->resetScreen();
        return;
    }
    if(!myUI->Screen->isBusy())
    {
        printTimeStandardFormat(editTimeBuffer, uiEditHour, uiEditMinute, myCalendar->is24TimeFormatEnabled());
        myUI->Screen->addMessageToQueue(&EditMinuteMsg);
    }    
}
