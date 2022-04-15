#include "SystemManager.h"
#include "stateMenuConfig.h"

int currentMenu = 0;
int currentMenuItems = 0;

bool menuVariablesLoaded = false;
bool menusLoaded = false;
bool buttonReleased = false;
bool initScreen = false;
menuItem_t menuArray[NUMBER_OF_MENUS];

void addMenu(systemState_t goToState, const ScreenMessages * MenuLabel)
{
    if(currentMenuItems < NUMBER_OF_MENUS)
    {
        menuArray[currentMenuItems].goToState = goToState;
        menuArray[currentMenuItems].MenuLabel = MenuLabel;
        currentMenuItems++;
    }
}

void setupMenus(void)
{
    if(!menusLoaded)
    {
        addMenu(EDIT_TIME, &EditTimeMsg);
        addMenu(EDIT_24_HOUR_FORMAT, &EditTimeFormatMsg);
        addMenu(EN_DIS_LOW_TEMP_NO_BREW, &LowTempNoBrewSettingsMsg);
        addMenu(EDIT_TANK_TEMP, &EditTankTempMsg);
        addMenu(EDIT_ENERGY_SAVINGS, &EditEnergySavingsMsg);
        addMenu(EDIT_COFFEE_RECIPE, &EditCoffeeRecipesMsg);
        addMenu(TEACH_MODE, &TeachModeMsg);
        addMenu(EDIT_DRIPOUT_TIME, &MenuBrewDripoutMsg);
        addMenu(SHOW_BREW_COUNTS, &ShowBrewCountMsg);
        addMenu(RESET_DEFAULT, &ResetToDefaultMsg);        
        addMenu(VIEW_TEMPERATURES, &ViewTemperaturesMsg);
#if ENGINEERING_BUILD
        addMenu(SYSTEM_STATE_AUTO_TEST, &AutomaticTestModeMsg);
        addMenu(VIEW_PRE_ERR4_TEMP_LOG, &PreError4TempsMsg);
        addMenu(VIEW_POST_ERR4_TEMP_LOG, &PostError4TempsMsg);
#endif
        addMenu(VIEW_WATER_LEVELS, &ViewWaterLevelMsg);        
        menusLoaded = true;
    }    
}

void SystemManager::stateMenuBrowse()
{
    myUI->LEDs->turnOffAllLEDs(false); 
    if(!menuVariablesLoaded)
    {
        currentMenu = 0;
        menuVariablesLoaded = true;
        buttonReleased = false;
        setupMenus();           
    }
    if(!initScreen)
    {
        myUI->Screen->showMessageNow(menuArray[currentMenu].MenuLabel);
        initScreen = true;
    }    
    //Logic to stop release touch status from being handled on entering menu.
    if(releasedTouchValue == TOUCH_ACCEPT && !buttonReleased)
    {
        buttonReleased = true;
        releasedTouchValue = TOUCH_NONE;
    }
    
    if(releasedTouchValue == TOUCH_NEXT)
    {
       currentMenu++;
       if(currentMenu > currentMenuItems-1)
       {
           currentMenu = 0;
       }
       myUI->Screen->showMessageNow(menuArray[currentMenu].MenuLabel);
    }
    else if(releasedTouchValue == TOUCH_PREVIOUS)
    {        
        currentMenu--;
        if(currentMenu < 0)
        {
            currentMenu = currentMenuItems-1;
        }
        myUI->Screen->showMessageNow(menuArray[currentMenu].MenuLabel);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {
        menuVariablesLoaded = false;
        changeState(menuArray[currentMenu].goToState);
        initScreen = false;
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {        
        changeState(NORMAL_MODE);
        menuVariablesLoaded = false;
        initScreen = false;
        return;
    }
}

