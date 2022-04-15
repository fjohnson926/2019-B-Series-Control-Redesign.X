#include "SystemManager.h"
#include "stateMenuConfig.h"

static int currentMenu = 0;
static int currentMenuItems = 0;
static bool menusLoaded = false;
static menuItem_t menuArray[NUMBER_OF_MENUS];

static void addMenu(systemState_t goToState, const ScreenMessages * MenuLabel)
{
    if(currentMenuItems < NUMBER_OF_MENUS)
    {
        menuArray[currentMenuItems].goToState = goToState;
        menuArray[currentMenuItems].MenuLabel = MenuLabel;
        currentMenuItems++;
    }
}

static void setupMenus(void)
{
    if(!menusLoaded)
    {
        addMenu(FCT_READ_ON_BRD_USER_INPUTS, &ReadProgramSwitchMsg);
        addMenu(FCT_CTRL_TEA_OUTPUT, &ControlTeaOuputsMsg);
        addMenu(FCT_CTRL_COFFEE_OUTPUT, &ControlCoffeeOutputMsg);        
        addMenu(FCT_CTRL_TANK_OUTPUT, &ControlTankOutputsMsg);
        addMenu(FCT_CTRL_LEDS, &ControlLEDMsg);
        addMenu(FCT_READ_TANK_INPUTS, &ReadTankInputMsg);
        addMenu(FCT_READ_REED_SWITCH, &ReadReedInputMsg);
        addMenu(FCT_READ_DIP_SWITCH, &ReadDipSwitchMsg);        
        addMenu(FCT_READ_TOUCH_SENSOR, &TouchSensorTestMsg);
        addMenu(FCT_FLASH_RAM_TEST, &NVRAMTestMsg);
        addMenu(FCT_ALL_OUTPUTS_ON_TEST, &AllOutputsOnTest);
    }    
}

void SystemManager::stateFCTMenuBrowse()
{
    myUI->LEDs->turnOffAllLEDs(true); 
    if(changedState)
    {
        currentMenu = 0;             
        setupMenus();
        myTank->disableHeating();
        myTank->disableFilling();
        myTank->enableManualOutputControl();
        myUI->Screen->showMessageNow(menuArray[currentMenu].MenuLabel);
        changedState = false;
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
        changeState(menuArray[currentMenu].goToState);        
        return;
    }
}

