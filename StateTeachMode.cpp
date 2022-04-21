#include "SystemManager.h"
#include "RAMRecipes.h"
#include "ScreenMessages.h"


//This state allows a user to easily program a simple recipe. The recipe is a one pulse recipe

const int TEXT_BUFFER_SIZE = 16;
const int LOW_TEMP_NO_TEACH_DIFF_F = 3;
extern int8_t recipeSelectIndex;
extern int8_t recipeRightSelectIndex;
extern uint8_t headSelectIndex;

typedef enum TEACH_MODE_STATES
    {
    TEACH_MODE_MAIN_INIT,
    TEACH_MODE_CHECK_FOR_ERROR_STATE,
    TEACH_MODE_CHECK_WAIT_FOR_TANK_SETPOINT_STATE,
    TEACH_MODE_SELECT_HEAD_STATE,
    TEACH_MODE_TEACH_STATE,
    TEACH_MODE_ASK_TO_SAVE_STATE,

    } TeachModeState_t;

TeachModeState_t globalTeachModeState = TEACH_MODE_MAIN_INIT;
int teachModeHeadSelectIndex = 0;
DispenseManager * selectedDispenseHead;
int valveOpenTimeMs = 0;

void SystemManager::teachModeCheckForSystemErrorState()
    {
    static bool localStateInit = false;
    static ScreenMessages CheckSystemErrorMsg("CHECKING FOR", "ERRORS");
    static ScreenMessages CriticalErrorsMsg("SYSTEM ERRORS", "DETECTED");
    static ScreenMessages CriticalErrorsMsg2("CLEAR ERRORS", "RESTART MACHINE");

    if (!localStateInit)
        {
        myUI->Screen->showMessageNow(&CheckSystemErrorMsg);
        localStateInit = true;
        }

    if (majorErrorsDetected())
        {
        if (!myUI->Screen->isBusy())
            {
            myUI->Screen->addMessageToQueue(&CriticalErrorsMsg);
            myUI->Screen->addMessageToQueue(&CriticalErrorsMsg2);
            }
        if (releasedTouchValue == TOUCH_ACCEPT || releasedTouchValue == TOUCH_CANCEL)
            {
            localStateInit = false;
            globalTeachModeState = TEACH_MODE_MAIN_INIT;
            changeState(MENU_BROWSE);
            return;
            }
        }
    else
        {
        globalTeachModeState = TEACH_MODE_CHECK_WAIT_FOR_TANK_SETPOINT_STATE;
        localStateInit = false;
        }
    }

void SystemManager::teachModeCheckTankTemperature()
    {
    static bool localStateInit = false;
    static char WaterTempMsgBuffer[20];
    static ScreenMessages WaterTempLowMsg2("PROGRESS", WaterTempMsgBuffer);
    static ScreenMessages CheckWaterStateMsg("PLEASE WAIT", "CHECKING TEMP");
    static ScreenMessages WaterTempLowMsg("HEATING WATER", "PLEASE WAIT");

    if (!localStateInit)
        {
        myUI->Screen->showMessageNow(&CheckWaterStateMsg);
        localStateInit = true;
        }

    if (releasedTouchValue == TOUCH_CANCEL)
        {
        localStateInit = false;
        globalTeachModeState = TEACH_MODE_MAIN_INIT;
        changeState(MENU_BROWSE);
        return;
        }

    if (myTank->currentTemperatureF < myTank->tankSetpointF - LOW_TEMP_NO_TEACH_DIFF_F)
        {
        if (!myUI->Screen->isBusy())
            {
            myUI->Screen->addMessageToQueue(&WaterTempLowMsg);
            int newTankHeatProgressPercent = (myTank->currentTemperatureF / (myTank->tankSetpointF - LOW_TEMP_NO_TEACH_DIFF_F)) * 100;
            sprintf(WaterTempMsgBuffer, "%d%c COMPLETE", newTankHeatProgressPercent, '%');
            myUI->Screen->addMessageToQueue(&WaterTempLowMsg2);
            }
        globalTeachModeState = TEACH_MODE_CHECK_WAIT_FOR_TANK_SETPOINT_STATE;
        }
    else
        {
        globalTeachModeState = TEACH_MODE_SELECT_HEAD_STATE;
        localStateInit = false;
        }
    }

void teachModeUpdateHeadSelectMsg(char * buffer, int teachModeHeadSelectIndex)
    {
    if (teachModeHeadSelectIndex == RIGHT_SIDE_RECIPE_INDEX)
        {
        sprintf(buffer, "%s", "RIGHT SIDE");
        }
    else
        {
        sprintf(buffer, "%s", "LEFT SIDE");
        }
    }

void SystemManager::teachModeHeadSelectHandler()
    {
    static bool teachModeHeadSelectInit = false;
    static char teachModeHeadSelectBuffer[TEXT_BUFFER_SIZE];
    static ScreenMessages teachModeHeadSelectMsg("HEAD SELECTED", teachModeHeadSelectBuffer);

    if (!teachModeHeadSelectInit)
        {
        teachModeHeadSelectIndex = 0;
        teachModeUpdateHeadSelectMsg(teachModeHeadSelectBuffer, teachModeHeadSelectIndex);
        myUI->Screen->showMessageNow(&teachModeHeadSelectMsg);
        teachModeHeadSelectInit = true;
        }

    //Don't select a head if the machine has only one head
    if (machineFeature->numberOfHeads == 1)
        {
        globalTeachModeState = TEACH_MODE_TEACH_STATE;
        return;
        }

    if (releasedTouchValue == TOUCH_NEXT && teachModeHeadSelectIndex == LEFT_SIDE_RECIPE_INDEX) //FJ changed separated TOUCHES
        {
        teachModeHeadSelectIndex = teachModeHeadSelectIndex + 3;
        teachModeUpdateHeadSelectMsg(teachModeHeadSelectBuffer, teachModeHeadSelectIndex);
        myUI->Screen->showMessageNow(&teachModeHeadSelectMsg);
        }
    else if (releasedTouchValue == TOUCH_PREVIOUS && teachModeHeadSelectIndex == RIGHT_SIDE_RECIPE_INDEX)
        {
        teachModeHeadSelectIndex = teachModeHeadSelectIndex - 3;
        teachModeUpdateHeadSelectMsg(teachModeHeadSelectBuffer, teachModeHeadSelectIndex);
        myUI->Screen->showMessageNow(&teachModeHeadSelectMsg);
        }
    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        teachModeHeadSelectInit = false;
        globalTeachModeState = TEACH_MODE_TEACH_STATE;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        teachModeHeadSelectInit = false;
        globalTeachModeState = TEACH_MODE_MAIN_INIT;
        changeState(MENU_BROWSE);
        return;
        }
    }

void SystemManager::handleTeachMode(void)
    {

    typedef enum
        {
        TEACH_MODE_HANDLER_INIT,
        TEACH_MODE_READY,
        TEACH_MODE_LEARNING,
        TEACH_MODE_STOPPED
            } handleTeachModeStates_t;

    static handleTeachModeStates_t handleTeachModeState = TEACH_MODE_HANDLER_INIT;
    static ScreenMessages ReadyToLearnMsg("READY TO", "LEARN");
    static ScreenMessages ReadyToLearnMsg2("PLACE", "CONTAINER");
    static ScreenMessages ReadyToLearnMsg3("PRESS DISPENSE", "TO START");
    static ScreenMessages LearningMsg1("PRESS DISPENSE", "TO STOP");

    if (handleTeachModeState == TEACH_MODE_HANDLER_INIT)
        {
        myUI->Screen->showMessageNow(&ReadyToLearnMsg);
        myUI->Screen->addMessageToQueue(&ReadyToLearnMsg2);
        myUI->Screen->addMessageToQueue(&ReadyToLearnMsg3);

        if (machineFeature->numberOfHeads == 1)
            {
            selectedDispenseHead = myLeftHead; //FJ changed from myRightSingleHead to myLeftHead
            myUI->LEDs->leftSingleBrewLeds->setColorGreen();
            }
        else if (teachModeHeadSelectIndex == LEFT_SIDE_RECIPE_INDEX)
            {
            selectedDispenseHead = myLeftHead;
            myUI->LEDs->leftSingleBrewLeds->setColorGreen();
            }
        else
            {
            selectedDispenseHead = myRightSingleHead;
            myUI->LEDs->rightBrewLeds->setColorGreen();
            }
        valveOpenTimeMs = 0;
        handleTeachModeState = TEACH_MODE_READY;
        }

    //Ready to get started with dispensing state
    if (handleTeachModeState == TEACH_MODE_READY)
        {
        if (!myUI->Screen->isBusy())
            {
            myUI->Screen->addMessageToQueue(&ReadyToLearnMsg);
            myUI->Screen->addMessageToQueue(&ReadyToLearnMsg2);
            myUI->Screen->addMessageToQueue(&ReadyToLearnMsg3);
            }
        if (
            (releasedTouchValue == TOUCH_RIGHT_BREW && selectedDispenseHead == myRightSingleHead) ||
            (releasedTouchValue == TOUCH_LEFT_SINGLE_BREW && selectedDispenseHead == myLeftHead) ||
            (releasedTouchValue == TOUCH_LEFT_SINGLE_BREW && machineFeature->numberOfHeads == 1)
            )
            {
            selectedDispenseHead->openDispenseValve();
            handleTeachModeState = TEACH_MODE_LEARNING;
            myUI->Screen->showMessageNow(&LearningMsg1);
            myUI->LEDs->rightBrewLeds->TurnOffAllLeds();
            myUI->LEDs->leftSingleBrewLeds->TurnOffAllLeds();
            }
        else if (releasedTouchValue == TOUCH_CANCEL)
            {
            handleTeachModeState = TEACH_MODE_HANDLER_INIT;
            globalTeachModeState = TEACH_MODE_MAIN_INIT;
            changeState(MENU_BROWSE);
            return;
            }
        }
    else if (handleTeachModeState == TEACH_MODE_LEARNING)
        {
        if (machineFeature->numberOfHeads == 1 || selectedDispenseHead == myLeftHead)
            {
            myUI->LEDs->leftSingleBrewLeds->setColorBlue();
            }
        else if (selectedDispenseHead == myRightSingleHead)
            {
            myUI->LEDs->rightBrewLeds->setColorBlue();
            }
        if (!myUI->Screen->isBusy())
            {
            myUI->Screen->addMessageToQueue(&LearningMsg1);
            }
        if (releasedTouchValue == TOUCH_RIGHT_BREW || releasedTouchValue == TOUCH_LEFT_SINGLE_BREW)
            {
            selectedDispenseHead->closeDispenseValve();
            handleTeachModeState = TEACH_MODE_STOPPED;
            }
        valveOpenTimeMs += SYSTEM_MANAGER_TASK_RATE_MS / portTICK_PERIOD_MS;
        }
    else if (handleTeachModeState == TEACH_MODE_STOPPED)
        {
        globalTeachModeState = TEACH_MODE_ASK_TO_SAVE_STATE;
        handleTeachModeState = TEACH_MODE_HANDLER_INIT;
        }
    }

void updateSaveTeachModeDataMsg(char * buffer, bool saveData)
    {
    if (saveData)
        {
        sprintf(buffer, "YES");
        }
    else
        {
        sprintf(buffer, "NO");
        }
    }

void SystemManager::teachModeSaveState(void)
    {
    static char saveDataAnswerBuffer[20];
    static ScreenMessages SaveTeachModeDataMsg("SAVE DATA", saveDataAnswerBuffer);
    static bool teachModeSaveStateInit = false;
    static bool saveData = false;

    if (!teachModeSaveStateInit)
        {
        teachModeSaveStateInit = true;
        saveData = false;
        updateSaveTeachModeDataMsg(saveDataAnswerBuffer, saveData);
        myUI->Screen->showMessageNow(&SaveTeachModeDataMsg);
        myUI->LEDs->rightBrewLeds->TurnOffAllLeds();
        myUI->LEDs->leftSingleBrewLeds->TurnOffAllLeds();
        }

    if (releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS)
        {
        saveData = !saveData;
        updateSaveTeachModeDataMsg(saveDataAnswerBuffer, saveData);
        myUI->Screen->showMessageNow(&SaveTeachModeDataMsg);
        }
    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        if (saveData)
            {
            CoffeeBeverage* ramRecipes = (CoffeeBeverage*) getCoffeeBeverageTableAddress();
            CoffeeBeverage taughtRecipe;
            for (int i = 0; i < NUMBER_OF_PULSES; i++)
                {
                taughtRecipe.size[0].pulseOffTimeSeconds[i] = 0;
                taughtRecipe.size[0].pulseOnTimeSeconds[i] = 0;
                }
            taughtRecipe.size[0].pulseOnTimeSeconds[0] = float(valveOpenTimeMs) / float(1000);
            if (teachModeHeadSelectIndex == LEFT_SIDE_RECIPE_INDEX) //FJ added if statement 4/20
                {
                ramRecipes[recipeSelectIndex] = taughtRecipe; //FJ changed from teachModeHeadSelectIndex to recipeSelectIndex
                }
            else
                {
                ramRecipes[recipeRightSelectIndex] = taughtRecipe;
                }

            NVBlobs->flushNvBlob(COFFEE_RECIPE_BLOB_INDEX);
            }
        else
            {
            teachModeSaveStateInit = false;
            globalTeachModeState = TEACH_MODE_MAIN_INIT;
            return;
            }
        teachModeSaveStateInit = false;
        globalTeachModeState = TEACH_MODE_MAIN_INIT;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        teachModeSaveStateInit = false;
        globalTeachModeState = TEACH_MODE_MAIN_INIT;
        return;
        }
    }

//Main function for Teach Mode

void SystemManager::StateTeachModeMain(void)
    {
    if (globalTeachModeState == TEACH_MODE_MAIN_INIT)
        {
        globalTeachModeState = TEACH_MODE_CHECK_FOR_ERROR_STATE;
        }

    switch (globalTeachModeState)
        {
        case TEACH_MODE_CHECK_FOR_ERROR_STATE:
            teachModeCheckForSystemErrorState();
            break;
        case TEACH_MODE_CHECK_WAIT_FOR_TANK_SETPOINT_STATE:
            teachModeCheckTankTemperature();
            break;
        case TEACH_MODE_SELECT_HEAD_STATE:
            teachModeHeadSelectHandler();
            break;
        case TEACH_MODE_TEACH_STATE:
            handleTeachMode();
            break;
        case TEACH_MODE_ASK_TO_SAVE_STATE:
            teachModeSaveState();
            break;
        }
    }