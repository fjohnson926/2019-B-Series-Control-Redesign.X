#include "atmel_start.h"
#include "SystemManager.h"
#include "NvParmManager.h"
#include "RAMRecipes.h"

typedef enum BREW_COUNTER_SUBSTATES
    {
    STATE_SELECT_BREW_COUNTER,
    STATE_SELECT_AND_SHOW_RESETTABLE_HEAD_COUNT,
    STATE_SELECT_AND_SHOW_NONRESETTABLE_HEAD_COUNT,
    STATE_RESET_SELECTED_HEAD_COUNTER,
    } brewCountSubstates_t;

static int selectedHead = 1; //FJ changed 4/22 see issue# 2 for details
static bool isSelectBrewCounterMainInit = false;
static brewCountSubstates_t brewCounterState = STATE_SELECT_BREW_COUNTER;

void SystemManager::selectBrewCounter()
    {
    static ScreenMessages ViewResettableBrewCountMsg("RESETTABLE", "BREW COUNTS", DEFAULT_STATIC_MENU_PRINT_TIME);
    static ScreenMessages ViewNonResettableBrewCountMsg("NON RESETTABLE", "BREW COUNTS", DEFAULT_STATIC_MENU_PRINT_TIME);
    static bool showResetableBrewCount = true;
    static bool initialized = false;
    if (!initialized)
        {
        showResetableBrewCount = true;
        myUI->Screen->showMessageNow(&ViewResettableBrewCountMsg);
        initialized = true;
        }

    if (releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS)
        {
        showResetableBrewCount = !showResetableBrewCount;
        if (showResetableBrewCount)
            {
            myUI->Screen->showMessageNow(&ViewResettableBrewCountMsg);
            }
        else
            {
            myUI->Screen->showMessageNow(&ViewNonResettableBrewCountMsg);
            }
        }
    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        initialized = false;
        if (showResetableBrewCount)
            {
            brewCounterState = STATE_SELECT_AND_SHOW_RESETTABLE_HEAD_COUNT;
            }
        else
            {
            brewCounterState = STATE_SELECT_AND_SHOW_NONRESETTABLE_HEAD_COUNT;
            }
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        initialized = false;
        isSelectBrewCounterMainInit = false;
        changeState(MENU_BROWSE);
        return;
        }
    }

void SystemManager::selectHeadAndShowResetableCounter()
    {
    static bool initialized = false;
    static char RightHeadBuffer[20];
    static char LeftHeadBuffer[20];
    static ScreenMessages SelectedRightHeadMsg("RIGHT COUNT", RightHeadBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
    static ScreenMessages SelectedLeftHeadMsg("LEFT COUNT", LeftHeadBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
    static ScreenMessages SingleHeadMsg("COUNT", RightHeadBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
    if (!initialized)
        {
        NVBlobs->loadNvBlob(PARMS_READ_INDEX);
        NvParm rightSingleHeadParm = NvParmManager::getNvParm(PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE);
        NvParm leftHeadParm = NvParmManager::getNvParm(PARM_INDEX_RESETABLEBREWCNT_LEFT);

        sprintf(RightHeadBuffer, "%d", rightSingleHeadParm.u.integer_parm);
        sprintf(LeftHeadBuffer, "%d", leftHeadParm.u.integer_parm);

        selectedHead = 1; //FJ changed 4/22
        if (machineFeature->numberOfHeads != 1)
            {
            if (selectedHead == 1) //FJ changed 4/22
                {
                myUI->Screen->showMessageNow(&SelectedRightHeadMsg);
                }
            else
                {
                myUI->Screen->showMessageNow(&SelectedLeftHeadMsg);
                }
            }
        else
            {
            myUI->Screen->showMessageNow(&SingleHeadMsg);
            }
        initialized = true;
        }


    if ((releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS) && machineFeature->numberOfHeads != 1)
        {
        selectedHead = !selectedHead;
        if (selectedHead ==1)
            {
            myUI->Screen->showMessageNow(&SelectedRightHeadMsg);
            }
        else
            {
            myUI->Screen->showMessageNow(&SelectedLeftHeadMsg);
            }
        }

    // Change above if statement to below for issue#2
    
//    if (releasedTouchValue == TOUCH_NEXT && machineFeature->numberOfHeads != 1 && selectedHead == LEFT_SIDE_RECIPE_INDEX)
//        {
//        selectedHead = selectedHead + 3;
//        myUI->Screen->showMessageNow(&SelectedRightHeadMsg);
//        }
//    else if (releasedTouchValue == TOUCH_PREVIOUS && machineFeature->numberOfHeads != 1 && selectedHead == RIGHT_SIDE_RECIPE_INDEX)
//        {
//        selectedHead = selectedHead - 3;
//        myUI->Screen->showMessageNow(&SelectedLeftHeadMsg);
//        }

    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        brewCounterState = STATE_RESET_SELECTED_HEAD_COUNTER;
        initialized = false;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        initialized = false;
        brewCounterState = STATE_SELECT_BREW_COUNTER;
        return;
        }
    }

void SystemManager::selectHeadAndShowNonResetableCounter()
    {
    static bool initialized = false;
    static char RightHeadBuffer[20];
    static char LeftHeadBuffer[20];
    static ScreenMessages SelectedRightHeadMsg("RIGHT COUNT", RightHeadBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
    static ScreenMessages SelectedLeftHeadMsg("LEFT COUNT", LeftHeadBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
    static ScreenMessages SingleHeadMsg("COUNT", RightHeadBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
    if (!initialized)
        {
        NVBlobs->loadNvBlob(PARMS_READ_INDEX);
        NvParm rightSingleHeadParm = NvParmManager::getNvParm(PARM_INDEX_NONRESETABLEBREWCNT_RIGHTSINGLE);
        NvParm leftHeadParm = NvParmManager::getNvParm(PARM_INDEX_NONRESETABLEBREWCNT_LEFT);

        sprintf(RightHeadBuffer, "%d", rightSingleHeadParm.u.integer_parm);
        sprintf(LeftHeadBuffer, "%d", leftHeadParm.u.integer_parm);

        selectedHead = 1; //FJ changed 4/22
        if (machineFeature->numberOfHeads != 1)
            {
            if (selectedHead == 1) //FJ changed 4/22
                {
                myUI->Screen->showMessageNow(&SelectedRightHeadMsg);
                }
            else
                {
                myUI->Screen->showMessageNow(&SelectedLeftHeadMsg);
                }
            }
        else
            {
            myUI->Screen->showMessageNow(&SingleHeadMsg);
            }
        initialized = true;
        }

    if ((releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS) && machineFeature->numberOfHeads != 1)
        {
        selectedHead = !selectedHead;
        if (selectedHead) //Changed from if(selectedHead)
            {
            myUI->Screen->showMessageNow(&SelectedRightHeadMsg);
            }
        else
            {
            myUI->Screen->showMessageNow(&SelectedLeftHeadMsg);
            }
        }
    
//    if (releasedTouchValue == TOUCH_NEXT && machineFeature->numberOfHeads != 1 && selectedHead == LEFT_SIDE_RECIPE_INDEX)
//        {
//        selectedHead = selectedHead + 3;
//        myUI->Screen->showMessageNow(&SelectedRightHeadMsg);
//        }
//    else if (releasedTouchValue == TOUCH_PREVIOUS && machineFeature->numberOfHeads != 1 && selectedHead == RIGHT_SIDE_RECIPE_INDEX)
//        {
//        selectedHead = selectedHead - 3;
//        myUI->Screen->showMessageNow(&SelectedLeftHeadMsg);
//        }

    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        brewCounterState = STATE_SELECT_BREW_COUNTER;
        initialized = false;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        initialized = false;
        brewCounterState = STATE_SELECT_BREW_COUNTER;
        return;
        }
    }

void updateResetCounterMsg(bool yes, char * buffer)
    {
    if (yes)
        {
        sprintf(buffer, "YES");
        }
    else
        {
        sprintf(buffer, "NO");
        }
    }

void SystemManager::resetCounter()
    {
    static bool initialized = false;
    static bool resetCount = false;
    static char resetCountBuffer[20];
    static ScreenMessages ResetCountMsg("RESET COUNT?", resetCountBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);
    if (!initialized)
        {
        resetCount = false;
        initialized = true;
        updateResetCounterMsg(resetCount, resetCountBuffer);
        myUI->Screen->showMessageNow(&ResetCountMsg);
        }
    if (releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS)
        {
        resetCount = !resetCount;
        updateResetCounterMsg(resetCount, resetCountBuffer);
        myUI->Screen->showMessageNow(&ResetCountMsg);
        }
    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        if (resetCount)
            {
            NVBlobs->loadNvBlob(PARMS_READ_INDEX);
            NvParm parm;
            parm = NvParmManager::getNvParm(PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE + selectedHead);
            parm.u.integer_parm = 0;
            NvParmManager::setNvParm(PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE + selectedHead, parm);
            NVBlobs->flushNvBlob(PARMS_READ_INDEX);
            }

        initialized = false;
        brewCounterState = STATE_SELECT_AND_SHOW_RESETTABLE_HEAD_COUNT;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        initialized = false;
        brewCounterState = STATE_SELECT_AND_SHOW_RESETTABLE_HEAD_COUNT;
        return;
        }
    }

void SystemManager::StateBrewCountsMain()
    {
    switch (brewCounterState)
        {
        case STATE_SELECT_BREW_COUNTER:
            selectBrewCounter();
            break;
        case STATE_SELECT_AND_SHOW_RESETTABLE_HEAD_COUNT:
            selectHeadAndShowResetableCounter();
            break;
        case STATE_SELECT_AND_SHOW_NONRESETTABLE_HEAD_COUNT:
            selectHeadAndShowNonResetableCounter();
            break;
        case STATE_RESET_SELECTED_HEAD_COUNTER:
            resetCounter();
            break;
        }
    }
