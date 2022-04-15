#include "SystemManager.h"

//bool initialSystemMessagesPrinted = false;
ScreenMessages KnownModelMsg("MODEL DETECTED", "CHANGE ME");

void SystemManager::systemStartupState()
{    
    if(changedState)
    {
        myUI->Screen->addMessageToQueue(&GrindmasterMsg);
        myUI->Screen->addMessageToQueue(&VersionMsg);
#if ENGINEERING_BUILD
        myUI->Screen->addMessageToQueue(&EngineeringBuildMsg);
#endif        
        if(myModel == MODEL_UNKNOWN)
        {
            myUI->Screen->addMessageToQueue(&UnknownModelMsg);
        }
        else
        {
            KnownModelMsg.bottomString_p = machineFeature->modelInfo;
            myUI->Screen->addMessageToQueue(&KnownModelMsg);
        }
        changedState = false;
    }    
    
    if (!myUI->Screen->isBusy())
    {
        if(myUI->TouchControls->touchTestState == WAITING_TO_START_TEST)
        {
            myUI->TouchControls->touchTestState = START_TOUCH_TESTING;
        }
        
        if(myUI->TouchControls->touchTestState != COMPLETED_TOUCH_TESTING || !myTank->isWaterLevelADCValuesFilled || !myTank->temperatureADCArrayLoaded)
        {
            myUI->Screen->addMessageToQueue(&TouchSetupMsg);
        }
    }
    
    if(myUI->TouchControls->controllerSetupComplete && myTank->isWaterLevelADCValuesFilled && myTank->temperatureADCArrayLoaded)
    {
        if(modelByte != MODEL_BYTE_FCT_MODE)
        {
            changeState(NORMAL_MODE);
        }
        else
        {
            changeState(FCT_MAIN_MENU);
        }
    }
}