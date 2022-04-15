#include "SystemManager.h"

#define EDIT_MONTH_STATE 0
#define EDIT_DAY_STATE   1
#define EDIT_YEAR_STATE  2

bool editVariablesLoaded = false;
bool initButtonReleased = false;
uint8_t uiEditMonth;
uint8_t uiEditDay;
int     uiEditYear;
uint8_t currentEditDateState = EDIT_MONTH_STATE;
char editBuffer[20];    
ScreenMessages EditMonthMsg("EDIT MONTH", editBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
ScreenMessages EditDayMsg ("EDIT DAY", editBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
ScreenMessages EditYearMsg("EDIT YEAR", editBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
uint8_t maxDaysInEachMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31} ;

void SystemManager::stateEditDate()
{    
    if(!editVariablesLoaded)
    {
        uiEditMonth = myCalendar->getMonth();
        uiEditDay = myCalendar->getDay();
        uiEditYear = myCalendar->getYear();
        currentEditDateState = EDIT_MONTH_STATE;
        editVariablesLoaded = true;
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->addMessageToQueue(&EditMonthMsg);
    }
    
    switch(currentEditDateState)
    {
        case EDIT_MONTH_STATE:
            editMonth();
            break;
        case EDIT_DAY_STATE:
            editDay();
            break;
        case EDIT_YEAR_STATE:
            editYear();
            break;
        default:
            break;
    }
}

void SystemManager::editMonth()
{    
    //Logic to stop release touch status from being handled on entering menu.
    if(releasedTouchValue == TOUCH_ACCEPT && !initButtonReleased)
    {
        initButtonReleased = true;        
        myUI->Screen->showMessageNow(&EditMonthMsg);
    }
    if(releasedTouchValue == TOUCH_NEXT && uiEditMonth < 12)
    {
        uiEditMonth++;
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->showMessageNow(&EditMonthMsg);
    }
    else if(releasedTouchValue == TOUCH_PREVIOUS && uiEditMonth > 1)
    {
        uiEditMonth--;
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->showMessageNow(&EditMonthMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {
        currentEditDateState = EDIT_DAY_STATE;
        myUI->Screen->resetScreen();
        initButtonReleased = false;
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {
        editVariablesLoaded = false;
        initButtonReleased = false;        
        changeState(MENU_BROWSE);
        myUI->Screen->resetScreen();
        return;
    }
    if(!myUI->Screen->isBusy())
    {
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->addMessageToQueue(&EditMonthMsg);
    }    
}

void SystemManager::editDay()
{    
    //Logic to stop release touch status from being handled on entering menu.
    if(releasedTouchValue == TOUCH_ACCEPT && !initButtonReleased)
    {
        initButtonReleased = true;        
    }
    if(releasedTouchValue == TOUCH_NEXT && uiEditDay < maxDaysInEachMonth[uiEditMonth-1])
    {
        uiEditDay++;
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->showMessageNow(&EditDayMsg);
    }
    else if(releasedTouchValue == TOUCH_PREVIOUS && uiEditDay > 1)
    {
        uiEditDay--;
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->showMessageNow(&EditDayMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {
        currentEditDateState = EDIT_YEAR_STATE;
        myUI->Screen->resetScreen();
        initButtonReleased = false;
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {
        currentEditDateState = EDIT_MONTH_STATE;
        myUI->Screen->resetScreen();
        return;
    }
    if(!myUI->Screen->isBusy())
    {
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->addMessageToQueue(&EditDayMsg);
    }    
}

void SystemManager::editYear()
{    
    //Logic to stop release touch status from being handled on entering menu.
    if(releasedTouchValue == TOUCH_ACCEPT && !initButtonReleased)
    {
        initButtonReleased = true;        
    }
    if(releasedTouchValue == TOUCH_NEXT && uiEditYear < 9999)
    {
        uiEditYear++;
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->showMessageNow(&EditYearMsg);
    }
    else if(releasedTouchValue == TOUCH_PREVIOUS && uiEditYear > 2019)
    {
        uiEditYear--;
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->showMessageNow(&EditYearMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {
        editVariablesLoaded = false;
        initButtonReleased = false;
        myCalendar->setDate(uiEditMonth, uiEditDay, uiEditYear);
        changeState(MENU_BROWSE);        
        return;
    }
        else if(releasedTouchValue == TOUCH_CANCEL)
    {
        currentEditDateState = EDIT_DAY_STATE;
        myUI->Screen->resetScreen();
        return;
    }
    if(!myUI->Screen->isBusy())
    {
        printDateStandardFormat(editBuffer, uiEditMonth, uiEditDay, uiEditYear);
        myUI->Screen->addMessageToQueue(&EditYearMsg);
    }    
}