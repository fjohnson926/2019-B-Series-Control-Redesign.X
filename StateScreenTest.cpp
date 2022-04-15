#include "SystemManager.h"
#include "RAMRecipes.h"
#include "ScreenMessages.h"

#define SCREEN_REPRINT_SPEED_MS 100

int screenRefreshCount = 0;
ScreenMessages ScreenTestMsg("---SCREENTEST---","++++STAND BY++++");

void SystemManager::stateScreenTest(void)
{
    screenRefreshCount += SYSTEM_MANAGER_TASK_RATE_MS;
    if(screenRefreshCount > SCREEN_REPRINT_SPEED_MS)
    {
        screenRefreshCount = 0;
        myUI->Screen->showMessageNow(&ScreenTestMsg);
    }
}