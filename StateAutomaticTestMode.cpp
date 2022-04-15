#include "SystemManager.h"
#include "NvParmManager.h"

const int NUM_CHARS_LINE = 16;

typedef struct AUTO_BREW_HEAD
{
    DispenseManager * DspMgr;
    TriColorLed * DspLEDs;
    uint16_t brewTouchValue;
    int volatileDispCount;
    int parmBrewCountResettableIndex;
    int parmBrewCountNonResettableIndex;
    int parmBrewCountIndex; 
    bool autoEnabled;
    int waitTimeMs;
    int waitTimeToReadyMs;
    char headLabel[NUM_CHARS_LINE];
    char * strCountBuf;
    ScreenMessages * EnableMsg;
    ScreenMessages * DisabledMsg;
    ScreenMessages * WaitingMsg;
    ScreenMessages * BrewCountMsg;
} AutoBrewHead_t;

ScreenMessages InAutomaticTestModeMsg("AUTOMATIC", "TESTING MODE");
ScreenMessages RightSideActiveMsg("RIGHT SIDE", "ENABLED");
ScreenMessages RightSideInActiveMsg("RIGHT SIDE", "DISABLED");
ScreenMessages LeftSideActiveMsg("LEFT SIDE", "ENABLED");
ScreenMessages LeftSideInActiveMsg("LEFT SIDE", "DISABLED");
ScreenMessages SingleSideActive("HEAD", "ENABLED");
ScreenMessages SingleSideInActive("HEAD", "DISABLED");
ScreenMessages AutoError1("FAULT","DETECTED");
ScreenMessages AutoError2("CLEAR FAULTS", "BEFORE USING");
ScreenMessages HeatingMsg("WAITING TO", "REACH BREW TEMP");
ScreenMessages SingleHeadWaitingMsg("HEAD","WAITING");
ScreenMessages LeftSideWaitingMsg("LEFT SIDE", "WAITING");
ScreenMessages RightSideWaitingMsg("RIGHT SIDE", "WAITING");
static char rightBrewCountStr[NUM_CHARS_LINE];
static ScreenMessages RightBrewCountMsg("RIGHT BREW COUNT", rightBrewCountStr);
static char leftBrewCountStr[NUM_CHARS_LINE];
static ScreenMessages leftBrewCountMsg("LEFT BREW COUNT", leftBrewCountStr);
static char singleBrewCountStr[NUM_CHARS_LINE];
static ScreenMessages singleBrewCountMsg("BREW COUNT", singleBrewCountStr);
const int HR_IN_MS = 3600000;
const int DESIRED_DISP_PER_HOUR = 10;
bool initialized = false;
bool errorMessagesShown = false;
bool brewHeadsInitialized = false;
static AutoBrewHead_t leftHead, rightHead, singleHead;
const int MAX_AUTOMATIC_HEADS = 2;
AutoBrewHead_t * AutoBrewHeadManager[MAX_AUTOMATIC_HEADS] = { NULL, NULL };
int brewManagerWriteIndex = 0;

void addAutomaticHead (AutoBrewHead_t * head)
{
    if(brewManagerWriteIndex < MAX_AUTOMATIC_HEADS)
    {
        AutoBrewHeadManager[brewManagerWriteIndex] = head;
        brewManagerWriteIndex++;
    }
}

int getEstimatedDispenseTimeMs(CoffeeBeverage * recipe)
{
    int estDispTimeSeconds = 0;
    if(recipe)
    {
        
        int pulse = 0;
        while(pulse < NUMBER_OF_PULSES && recipe->size[0].pulseOnTimeSeconds[pulse])
        {
            estDispTimeSeconds = recipe->size[0].pulseOnTimeSeconds[pulse];
            pulse++;
        }
        pulse = 0;
        while(pulse < NUMBER_OF_PULSES && recipe->size[0].pulseOffTimeSeconds[pulse])
        {
            estDispTimeSeconds = recipe->size[0].pulseOnTimeSeconds[pulse];
            pulse++;
        }
    }
    return estDispTimeSeconds*1000;
}

void configureAutoBrewHeads(SystemManager * systemObject)
{
    if(systemObject)
    {
        if(systemObject->machineFeature->numberOfHeads == 1)
        {
            singleHead.DspMgr = systemObject->myRightSingleHead;
            singleHead.DspLEDs = systemObject->myUI->LEDs->leftSingleBrewLeds;
            singleHead.brewTouchValue = TOUCH_LEFT_SINGLE_BREW;
            singleHead.volatileDispCount = 0;
            singleHead.parmBrewCountResettableIndex = PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE; 
            singleHead.parmBrewCountNonResettableIndex = PARM_INDEX_NONRESETABLEBREWCNT_RIGHTSINGLE; 
            singleHead.autoEnabled = false;
            //Wraparound prevention
            if((HR_IN_MS/DESIRED_DISP_PER_HOUR) - getEstimatedDispenseTimeMs(getCoffeeBeveragePointer(0)) - singleHead.DspMgr->brewLockoutTimeMs >= 0)
            {
                    singleHead.waitTimeToReadyMs = (HR_IN_MS/DESIRED_DISP_PER_HOUR) - getEstimatedDispenseTimeMs(getCoffeeBeveragePointer(0)) - singleHead.DspMgr->brewLockoutTimeMs;
            }
            else
            {
              singleHead.waitTimeToReadyMs = 0;
            }
            //Set the wait time 1 value higher, so when the user presses start, a dispense will immediately take place
            singleHead.waitTimeMs = singleHead.waitTimeToReadyMs+1; 
            sprintf(singleHead.headLabel, "HEAD");
            singleHead.strCountBuf = singleBrewCountStr;
            sprintf(singleHead.strCountBuf, "0");
            singleHead.EnableMsg = &SingleSideActive;
            singleHead.DisabledMsg = &SingleSideInActive;
            singleHead.WaitingMsg = &SingleHeadWaitingMsg;
            singleHead.BrewCountMsg = &singleBrewCountMsg;
            addAutomaticHead(&singleHead);
        }
        else if(systemObject->machineFeature->numberOfHeads == 2)
        {
            leftHead.DspMgr = systemObject->myLeftHead;
            leftHead.DspLEDs = systemObject->myUI->LEDs->leftSingleBrewLeds;
            leftHead.brewTouchValue = TOUCH_LEFT_SINGLE_BREW;
            leftHead.volatileDispCount = 0;
            leftHead.parmBrewCountResettableIndex = PARM_INDEX_RESETABLEBREWCNT_LEFT; 
            leftHead.parmBrewCountNonResettableIndex = PARM_INDEX_NONRESETABLEBREWCNT_LEFT; 
            leftHead.autoEnabled = false;
            //Wraparound prevention
            if((HR_IN_MS/DESIRED_DISP_PER_HOUR) - getEstimatedDispenseTimeMs(getCoffeeBeveragePointer(1)) - leftHead.DspMgr->brewLockoutTimeMs >= 0)
            {
                    leftHead.waitTimeToReadyMs = (HR_IN_MS/DESIRED_DISP_PER_HOUR) - getEstimatedDispenseTimeMs(getCoffeeBeveragePointer(1)) - leftHead.DspMgr->brewLockoutTimeMs;
            }
            else
            {
              leftHead.waitTimeToReadyMs = 0;
            }
            //Set the wait time 1 value higher, so when the user presses start, a dispense will immediately take place
            leftHead.waitTimeMs = leftHead.waitTimeToReadyMs+1; 
            sprintf(leftHead.headLabel, "LEFT HEAD");
            leftHead.strCountBuf = leftBrewCountStr;
            sprintf(leftHead.strCountBuf, "0");
            leftHead.EnableMsg = &LeftSideActiveMsg;
            leftHead.DisabledMsg = &LeftSideInActiveMsg;
            leftHead.WaitingMsg = &LeftSideWaitingMsg;
            leftHead.BrewCountMsg = &leftBrewCountMsg;
            
            rightHead.DspMgr = systemObject->myRightSingleHead;
            rightHead.DspLEDs = systemObject->myUI->LEDs->rightBrewLeds;
            rightHead.brewTouchValue = TOUCH_RIGHT_BREW;
            rightHead.volatileDispCount = 0;
            rightHead.parmBrewCountResettableIndex = PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE; 
            rightHead.parmBrewCountNonResettableIndex = PARM_INDEX_NONRESETABLEBREWCNT_RIGHTSINGLE; 
            rightHead.autoEnabled = false;
            //Wraparound prevention
            if((HR_IN_MS/DESIRED_DISP_PER_HOUR) - getEstimatedDispenseTimeMs(getCoffeeBeveragePointer(0)) - rightHead.DspMgr->brewLockoutTimeMs >= 0)
            {
                    rightHead.waitTimeToReadyMs = (HR_IN_MS/DESIRED_DISP_PER_HOUR) - getEstimatedDispenseTimeMs(getCoffeeBeveragePointer(0)) - rightHead.DspMgr->brewLockoutTimeMs;
            }
            else
            {
              rightHead.waitTimeToReadyMs = 0;
            }
            //Set the wait time 1 value higher, so when the user presses start, a dispense will immediately take place
            rightHead.waitTimeMs = rightHead.waitTimeToReadyMs+1; 
            sprintf(rightHead.headLabel, "RIGHT HEAD");
            rightHead.strCountBuf = rightBrewCountStr;
            sprintf(rightHead.strCountBuf, "0");
            rightHead.EnableMsg = &RightSideActiveMsg;
            rightHead.DisabledMsg = &RightSideInActiveMsg;
            rightHead.WaitingMsg = &RightSideWaitingMsg;
            rightHead.BrewCountMsg = &RightBrewCountMsg;
            
            addAutomaticHead(&rightHead);
            addAutomaticHead(&leftHead);
        }
        
    }
}

void turnOffAutomaticBrewHeads()
{
    for(int i = 0; i < brewManagerWriteIndex; i++)
    {
        AutoBrewHeadManager[i]->DspMgr->stopDispense();        
        AutoBrewHeadManager[i]->autoEnabled = false;
    }
}

void manageAutomaticHeads(SystemManager * systemObject)
{
    if(systemObject)
    {
        for(int i = 0; i < brewManagerWriteIndex; i++)
        {
            if(systemObject->majorErrorsDetected())
            {
                turnOffAutomaticBrewHeads();
                AutoBrewHeadManager[i]->DspLEDs->setColorRed();
            }
            else
            {
                if(systemObject->releasedTouchValue == AutoBrewHeadManager[i]->brewTouchValue)
                {
                    AutoBrewHeadManager[i]->autoEnabled = !AutoBrewHeadManager[i]->autoEnabled;
                    if(AutoBrewHeadManager[i]->autoEnabled)
                    {
                        systemObject->myUI->Screen->showMessageNow(AutoBrewHeadManager[i]->EnableMsg);
                    }
                    else
                    {
                        systemObject->myUI->Screen->showMessageNow(AutoBrewHeadManager[i]->DisabledMsg);
                    }
                }        
                
                if(AutoBrewHeadManager[i]->autoEnabled && !AutoBrewHeadManager[i]->DspMgr->isDispensing && systemObject->isWaterTemperatureReady() 
                        && AutoBrewHeadManager[i]->waitTimeMs >= AutoBrewHeadManager[i]->waitTimeToReadyMs)
                {
                    AutoBrewHeadManager[i]->DspMgr->startCoffeeDispenseTask();                    
                    AutoBrewHeadManager[i]->waitTimeMs = 0;                    
                }
                else if (!AutoBrewHeadManager[i]->autoEnabled)
                {
                    AutoBrewHeadManager[i]->DspMgr->stopDispense();
                    AutoBrewHeadManager[i]->waitTimeMs += SYSTEM_MANAGER_TASK_RATE_MS;
                }
                else if(!systemObject->isWaterTemperatureReady() || AutoBrewHeadManager[i]->waitTimeMs < AutoBrewHeadManager[i]->waitTimeToReadyMs)
                {
                    AutoBrewHeadManager[i]->waitTimeMs += SYSTEM_MANAGER_TASK_RATE_MS;
                }

                if(AutoBrewHeadManager[i]->DspMgr->getDispenseCompleteStatus())
                {
                    NvParm parm;
                    systemObject->NVBlobs->loadNvBlob(PARMS_READ_INDEX);                    
                    
                    parm = NvParmManager::getNvParm(AutoBrewHeadManager[i]->parmBrewCountResettableIndex);
                    parm.u.integer_parm++;
                    NvParmManager::setNvParm(AutoBrewHeadManager[i]->parmBrewCountResettableIndex, parm);                       
                   
                    parm = NvParmManager::getNvParm(AutoBrewHeadManager[i]->parmBrewCountNonResettableIndex);
                    parm.u.integer_parm++;
                    NvParmManager::setNvParm(AutoBrewHeadManager[i]->parmBrewCountNonResettableIndex, parm);
                    
                    systemObject->NVBlobs->flushNvBlob(PARMS_READ_INDEX);
                    
                    AutoBrewHeadManager[i]->volatileDispCount++;
                }
                
                if(AutoBrewHeadManager[i]->DspMgr->isDispensing)
                {
                    AutoBrewHeadManager[i]->DspLEDs->setColorBlue();
                }
                else if(!AutoBrewHeadManager[i]->autoEnabled)
                {
                    AutoBrewHeadManager[i]->DspLEDs->setColorRed();
                }
                else
                {
                    AutoBrewHeadManager[i]->DspLEDs->setColorCyan();
                }
            }
        }
    }
}

void displayBrewMessages(SystemManager * systemObject)
{
    for(int i = 0; i < brewManagerWriteIndex; i++)
    {
        if(!AutoBrewHeadManager[i]->DspMgr->isDispensing && AutoBrewHeadManager[i]->autoEnabled) 
        {
            systemObject->myUI->Screen->addMessageToQueue(AutoBrewHeadManager[i]->WaitingMsg);
        }
        else if(!AutoBrewHeadManager[i]->autoEnabled)
        {
            systemObject->myUI->Screen->addMessageToQueue(AutoBrewHeadManager[i]->DisabledMsg);
        }
        else if(AutoBrewHeadManager[i]->autoEnabled)
        {
            systemObject->myUI->Screen->addMessageToQueue(AutoBrewHeadManager[i]->EnableMsg);
        }
        
        sprintf(AutoBrewHeadManager[i]->strCountBuf,"%d", AutoBrewHeadManager[i]->volatileDispCount);
        systemObject->myUI->Screen->addMessageToQueue(AutoBrewHeadManager[i]->BrewCountMsg);
    }
    
}


void SystemManager::AutoTestModeSystemLedHandler()
{
    if(myTank->isHeating())
    {
        myUI->LEDs->basketLeftLed->turnOnLED();
    }
    else
    {
        myUI->LEDs->basketLeftLed->turnOffLed();
    }
    
    if(myTank->isFilling())
    {
        myUI->LEDs->basketRightLed->turnOnLED();
    }
    else
    {
        myUI->LEDs->basketRightLed->turnOffLed();
    }
}

void SystemManager::AutoTestModeTouchHandler()
{
    switch(releasedTouchValue)
    {
        case TOUCH_CANCEL:
            myLeftHead->stopDispense();
            myLeftHead->dispenseCompleteFlag = false;
            myRightSingleHead->stopDispense();
            myRightSingleHead->dispenseCompleteFlag = false;
            myUI->LEDs->turnOffAllLEDs(false);
            initialized = false;
            changeState(MENU_BROWSE);
            break;       
    }
    if(releasedTouchValue != TOUCH_NONE)
    {
        myUI->LEDs->basketCenterLed->toggleLED();
    }
}
const int WHOLE_NUMBER_INDEX = 0;
const int TENTHS_NUMBER_INDEX = WHOLE_NUMBER_INDEX + 1;
void convertMsToHrs (uint64_t systemRunTimeMs, int * result, int resultSize)
{
    if(resultSize < 2)
    {
        return;
    }
    //Convert tempF to a floating point for floating point math
    float runTimeMs_float = systemRunTimeMs;
    //Convert to minutes
    float runTimeMins_float = runTimeMs_float/60000;
    //Convert to Hours
    float runTimeHrs_float = runTimeMins_float/60;    
    //The whole number part is just a chopped integer
    int runTimeHrsWhole = runTimeHrs_float;
    result[WHOLE_NUMBER_INDEX] = runTimeHrsWhole;
    //Get rid of the whole number part of the number, and get two digits of the fraction
    int runTimeHrs_frac =  float(runTimeHrs_float - runTimeHrsWhole) * 100;
    //Get the tenths digit
    int tengthsDigit = runTimeHrs_frac / 10;
    //Get the hundreths digit
    int hundrethsDigit = runTimeHrs_frac % 10;
    //Round the tenths digit
    if(hundrethsDigit >= 5)
    {
        tengthsDigit++;
        if(tengthsDigit > 9)
        {
            result[WHOLE_NUMBER_INDEX]++;
            tengthsDigit = 0;
        }
    }
    result[TENTHS_NUMBER_INDEX] = tengthsDigit;
}

void SystemManager::AutoTestModeScreenHandler()
{
    static char setPointTempF[NUM_CHARS_LINE];    
    static ScreenMessages SetpointMsg("SETPOINT TEMP", setPointTempF);
    static char currentTempF[NUM_CHARS_LINE];
    static ScreenMessages CurrentTempMsg("CURRENT TEMP", currentTempF);
    static ScreenMessages WaitingMsg("MACHINE IN", "WAIT STATE");
    static char brewCountRightSingleBuffer[20];
    static char brewCountLeftBuffer[20];
    static ScreenMessages brewCountRightSingleMsg("RIGHT BREW COUNT", brewCountRightSingleBuffer);
    static ScreenMessages brewCountLeftMsg("LEFT BREW COUNT", brewCountLeftBuffer);
    static ScreenMessages brewCountSingleMsg("BREW COUNT", brewCountRightSingleBuffer);
    static char systemRuntimeBuffer[NUM_CHARS_LINE];
    static ScreenMessages SystemRuntimeMsg("SYSTEM RUNTIME", systemRuntimeBuffer);
    static int systemRunTimeHrs[2];
    
    convertMsToHrs(systemRunTimeMs, systemRunTimeHrs, 2);
    
    sprintf(setPointTempF, "%d", (int)myTank->tankSetpointF);
    sprintf(currentTempF, "%d", (int)myTank->currentTemperatureF);
    sprintf(systemRuntimeBuffer, "%d.%dHrs", systemRunTimeHrs[WHOLE_NUMBER_INDEX], systemRunTimeHrs[TENTHS_NUMBER_INDEX]);
    
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&InAutomaticTestModeMsg);
        myUI->Screen->addMessageToQueue(&SetpointMsg);
        myUI->Screen->addMessageToQueue(&CurrentTempMsg);
        myUI->Screen->addMessageToQueue(&SystemRuntimeMsg);
        if(majorErrorsDetected())
        {
            myUI->Screen->addMessageToQueue(&AutoError1);
            myUI->Screen->addMessageToQueue(&AutoError2);
        }
        else if(!isWaterTemperatureReady())
        {
            myUI->Screen->addMessageToQueue(&HeatingMsg);
        }
        displayBrewMessages(this);
    }
}

void SystemManager::StateAutomaticTestMode(void)
{    
    static bool activeDispenses = 0;
    if(!initialized)
    {
        myUI->Screen->showMessageNow(&InAutomaticTestModeMsg);        
        initialized = true;
        errorMessagesShown = false;
        myRightSingleHead->loadCoffeeRecipe(*getCoffeeBeveragePointer(0));
        myLeftHead->loadCoffeeRecipe(*getCoffeeBeveragePointer(1));
        if(!brewHeadsInitialized)
        {
            configureAutoBrewHeads(this);
            brewHeadsInitialized = true;
        }
    }
    //monitorInletValveWhileDispensing();
    AutoTestModeSystemLedHandler();    
    AutoTestModeScreenHandler();
    manageAutomaticHeads(this);    
    AutoTestModeTouchHandler();
    manageFeaturedWarmersInNormalMode();
}
