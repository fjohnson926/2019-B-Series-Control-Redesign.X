#include "SystemManager.h"
#include "NvParmManager.h"

char timeString[12];
char heatingMessage[12];
ScreenMessages SystemTimeMsg("TIME IS", timeString);
int oldTankHeatProgressPercent = 0;
ScreenMessages SystemHeatMsg("LOW TEMPERATURE", heatingMessage);
ScreenMessages DispenseCompleteMsg("DISPENSE", "COMPLETE!");
ScreenMessages WaterReadyMsg("WATER TEMP", "OK");
ScreenMessages HalfBrewSelectedMsg("HALF BREW", "SELECTED");
ScreenMessages NormalBrewSelectedMsg("NORMAL BREW", "SELECTED");
extern int8_t recipeSelectIndex; //FJ added
extern int8_t recipeRightSelectIndex; //FJ added 4/20
uint8_t newRecipeIndex = 0;


char WarmerLowerBuffer[6][16];
char WarmerUpperBuffer[6][16];

ScreenMessages WarmerStatusMessage[6];

bool halfBrew = false;
int acceptButtonHeldCount = 0;

char Error2Buffer[16];
ScreenMessages TankError2TemperatureFoundMsg("ERROR 4 AT", Error2Buffer);

void scaleRecipe(CoffeeBeverage * recipe, float scaleFactor)
    {
    for (int i = 0; i < NUMBER_OF_PULSES; i++)
        {
        recipe->size[0].pulseOffTimeSeconds[i] *= scaleFactor;
        recipe->size[0].pulseOnTimeSeconds[i] *= scaleFactor;
        }
    }

void SystemManager::manageDispenseManagersInNormalMode(DispenseManager * dispense_p, TriColorLed * Leds_p, signed int dispTouchTag, const ScreenMessages * startMsg_p,
                                                       const ScreenMessages * stopMsg_p, uint8_t recipeIndex)
    {
    if (dispense_p == NULL)
        {
        return;
        }
    if (releasedTouchValue == dispTouchTag && !dispense_p->isDispensing && !majorErrorsDetected() && isWaterTemperatureReady() && !myTank->initialFill())
        {
        if(releasedTouchValue == TOUCH_LEFT_SINGLE_BREW) //FJ added 4/25
            {
            newRecipeIndex = recipeSelectIndex;
            }
        else if(releasedTouchValue == TOUCH_RIGHT_BREW)
            {
            newRecipeIndex = recipeRightSelectIndex;
            }
        CoffeeBeverage coffeeRecipe = *getCoffeeBeveragePointer(newRecipeIndex);
        if (halfBrew)
            {
            scaleRecipe(&coffeeRecipe, 0.5);
            dispense_p->loadCoffeeRecipe(coffeeRecipe);
            halfBrew = false;
            }
        else
            {
            dispense_p->loadCoffeeRecipe(coffeeRecipe);
            }

        dispense_p->startCoffeeDispenseTask();
        myUI->Screen->showMessageNow(startMsg_p);
        }
    else if (releasedTouchValue == dispTouchTag && dispense_p->isDispensing)
        {
        dispense_p->stopDispense();
        myUI->Screen->showMessageNow(stopMsg_p);
        }

    if (dispense_p->isDispensing)
        {
        Leds_p->setColorBlue();
        }
    else if (majorErrorsDetected() || !isWaterTemperatureReady() || myTank->initialFill())
        {
        Leds_p->setColorRed();
        }
    else if (halfBrew)
        {
        Leds_p->setColorCyan();
        }
    else
        {
        Leds_p->setColorGreen();
        }
        
    if (dispense_p->getDispenseCompleteStatus())
        {
        myUI->Screen->showMessageNow(&DispenseCompleteMsg);
        
        NVBlobs->loadNvBlob(PARMS_READ_INDEX);
        NvParm parm;
        parm = NvParmManager::getNvParm(PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE + recipeIndex); //(PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE + recipeIndex)
        parm.u.integer_parm++;
        NvParmManager::setNvParm(PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE + recipeIndex, parm); //(PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE + recipeIndex, parm)

        parm = NvParmManager::getNvParm(PARM_INDEX_NONRESETABLEBREWCNT_RIGHTSINGLE + recipeIndex);
        parm.u.integer_parm++;
        NvParmManager::setNvParm(PARM_INDEX_NONRESETABLEBREWCNT_RIGHTSINGLE + recipeIndex, parm);

        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        }

    }

void SystemManager::manageWarmersInNormalMode(Warmer * warmer_p, Led * warmerLED_p, signed int warmerTouchTag, const ScreenMessages * activeMsg_p, const ScreenMessages * inActiveMsg_p)
    {
    if (warmer_p == NULL)
        {
        return;
        }
    if (releasedTouchValue == warmerTouchTag)
        {
        if (!warmer_p->isWarmingAlgorithmActive)
            {
            warmer_p->startWarmingTask();
            myUI->Screen->showMessageNow(activeMsg_p);
            }
        else
            {
            warmer_p->stopWarming();
            myUI->Screen->showMessageNow(inActiveMsg_p);
            }
        }
    if (warmer_p->isWarmingAlgorithmActive)
        {
        warmerLED_p->turnOnLED();
        }
    else
        {
        warmerLED_p->turnOffLed();
        }
    }

void SystemManager::manageFeaturedWarmersInNormalMode()
    {
    const warmerConfig_t * systemWarmers_p[6] = {&machineFeature->mappedWarmer1, &machineFeature->mappedWarmer2, &machineFeature->mappedWarmer3,
                                                 &machineFeature->mappedWarmer4, &machineFeature->mappedWarmer5, &machineFeature->mappedWarmer6};
    char warmerLocationBuff[16];
    const warmerConfig_t * currWarmer_p = 0;


    for (int i = 0; i < 6; i++)
        {
        currWarmer_p = systemWarmers_p[i];

        if (currWarmer_p->warmer_p == NULL)
            {
            continue;
            }
        if (releasedTouchValue == currWarmer_p->touchValue)
            {
            if (!currWarmer_p->warmer_p->isWarmingAlgorithmActive)
                {
                currWarmer_p->warmer_p->startWarmingTask();
                }
            else
                {
                currWarmer_p->warmer_p->stopWarming();
                //myUI->Screen->showMessageNow(inActiveMsg_p);
                }
            }
        if (currWarmer_p->warmer_p->isWarmingAlgorithmActive)
            {
            currWarmer_p->LED_p->turnOnLED();
            }
        else
            {
            currWarmer_p->LED_p->turnOffLed();
            }
        }
    }

void SystemManager::turnOffWarmers(void)
    {
    const warmerConfig_t * systemWarmers_p[6] = {&machineFeature->mappedWarmer1, &machineFeature->mappedWarmer2, &machineFeature->mappedWarmer3,
                                                 &machineFeature->mappedWarmer4, &machineFeature->mappedWarmer5, &machineFeature->mappedWarmer6};
    const warmerConfig_t * currWarmer_p = 0;

    for (int i = 0; i < 6; i++)
        {
        currWarmer_p = systemWarmers_p[i];

        if (currWarmer_p->warmer_p == NULL)
            {
            continue;
            }
        currWarmer_p->LED_p->turnOffLed();
        }
    myWarmers->turnOffWarmers();
    }

void SystemManager::printWarmerOnMessages(void)
    {
    const warmerConfig_t * systemWarmers_p[6] = {&machineFeature->mappedWarmer1, &machineFeature->mappedWarmer2, &machineFeature->mappedWarmer3,
                                                 &machineFeature->mappedWarmer4, &machineFeature->mappedWarmer5, &machineFeature->mappedWarmer6};
    const warmerConfig_t * currWarmer_p = 0;

    for (int i = 0; i < 6; i++)
        {
        currWarmer_p = systemWarmers_p[i];

        if (currWarmer_p->warmer_p == NULL)
            {
            continue;
            }
        if (currWarmer_p->warmer_p->isWarmingAlgorithmActive)
            {
            sprintf(WarmerUpperBuffer[i], "%s", currWarmer_p->upperLabel);
            sprintf(WarmerLowerBuffer[i], "%s ON", currWarmer_p->lowerLabel);
            WarmerStatusMessage[i].topString_p = WarmerUpperBuffer[i];
            WarmerStatusMessage[i].bottomString_p = WarmerLowerBuffer[i];
            myUI->Screen->addMessageToQueue(&WarmerStatusMessage[i]);
            }
        }
    }

void SystemManager::stateNormalMode()
    {
    //monitorInletValveWhileDispensing();
    if (releasedTouchValue != TOUCH_NONE && releasedTouchValue != TOUCH_POWER)
        {
        myEnergySaver->clearCounter();
        }
    else if (releasedTouchValue == TOUCH_POWER)
        {
        turnOffWarmers();
        myLeftHead->stopDispense();
        myRightSingleHead->stopDispense();
        changeState(STANDBY_MODE);
        return;
        }
    else
        {
        myEnergySaver->incrementCounter(SYSTEM_MANAGER_TASK_RATE_MS / portTICK_PERIOD_MS);
        if (myEnergySaver->timeToSleep())
            {
            turnOffWarmers();
            changeState(ENERGY_SAVINGS_MODE);
            return;
            }
        }

    if (holdTouchValue == TOUCH_ACCEPT && holdTouchTimeMs >= HOLD_TIME_BEFORE_ENTER_MENU_MS)
        {
        if (!isProgramLockoutEnabled() && !myLeftHead->isDispensing && !myRightSingleHead->isDispensing)
            {
            changeState(MENU_BROWSE);
            return;
            }
        else if (isProgramLockoutEnabled())
            {
            myUI->Screen->showMessageNow(&ProgramLockoutMsg);
            }
        else if ((myLeftHead->isDispensing || myRightSingleHead->isDispensing))
            {
            myUI->Screen->showMessageNow(&DispenseLockoutMsgI);
            myUI->Screen->addMessageToQueue(&DispenseLockoutMsgII);
            }
        }

    if ((releasedTouchValue == TOUCH_PREVIOUS || releasedTouchValue == TOUCH_NEXT) && (isWaterTemperatureReady() && !majorErrorsDetected()))
        {
        halfBrew = !halfBrew;
        if (halfBrew)
            {
            myUI->Screen->showMessageNow(&HalfBrewSelectedMsg);
            }
        else
            {
            myUI->Screen->showMessageNow(&NormalBrewSelectedMsg);
            }
        }

    if (machineFeature->numberOfHeads == TWO_HEADS)
        {
        manageDispenseManagersInNormalMode(myLeftHead, myUI->LEDs->leftSingleBrewLeds, TOUCH_LEFT_SINGLE_BREW, &LeftHeadDispensingMsg, &LeftHeadCancelMsg, recipeSelectIndex); //FJ changed 4/20
        manageDispenseManagersInNormalMode(myRightSingleHead, myUI->LEDs->rightBrewLeds, TOUCH_RIGHT_BREW, &RightHeadDispensingMsg, &RightHeadCancelMsg, recipeRightSelectIndex); // FJ changed 4/20
        }
    else
        {
        manageDispenseManagersInNormalMode(myRightSingleHead, myUI->LEDs->leftSingleBrewLeds, TOUCH_LEFT_SINGLE_BREW, &SingleHeadDispensingMsg, &SingleHeadCancelMsg, recipeSelectIndex); //FJ changed from myRightSingleHead to myLeftHead
        }

    manageFeaturedWarmersInNormalMode();

    oldTankHeatProgressPercent = 0;

    if (!myUI->Screen->isBusy())
        {
        checkAndPrintSystemStatus();
        }
    //Since the control board is shared with the tea brewers, allow manual control of the tea brewers if dips are set to default
    //Have ten minutes to play around with the tea outputs for board testing
    if (myModel == MODEL_UNKNOWN && systemRunTimeMs <= 10 * 60 * 1000)
        {
        TeaIOTest(SYSTEM_MANAGER_TASK_RATE_MS);
        }
    else if (myModel == MODEL_UNKNOWN && systemRunTimeMs > 10 * 60 * 1000)
        {
        gpio_set_pin_level(LOCK_PIN, 0);
        gpio_set_pin_level(SIDE_SELECT_PIN, 0);
        gpio_set_pin_level(DIL0_PIN, 0);
        gpio_set_pin_level(DIL1_PIN, 0);
        gpio_set_pin_level(SUGAR0_PIN, 0);
        gpio_set_pin_level(SUGAR1_PIN, 0);
        }
    }

bool SystemManager::isWaterTemperatureReady()
    {
    if (lowTempNoBrewEnabled || systemState == SYSTEM_STATE_AUTO_TEST)
        {
        //In a low temp no brew state.
        if (lowTempNoBrewWaterReady == false)
            {
            if (myTank->currentTemperatureF < myTank->tankSetpointF - LTNBStopLimitF)
                {
                lowTempNoBrewWaterReady = false;
                showTempReadyMsg = true;
                }
            else
                {
                lowTempNoBrewWaterReady = true;
                }
            }
            //water temp is high enough for brewing
        else if (lowTempNoBrewWaterReady == true)
            {
            if (myTank->currentTemperatureF < myTank->tankSetpointF - LTNBStartLimitF)
                {
                lowTempNoBrewWaterReady = false;
                }
            else
                {
                lowTempNoBrewWaterReady = true;
                if (showTempReadyMsg)
                    {
                    showTempReadyMsg = false;
                    myUI->Screen->showMessageNow(&WaterReadyMsg);
                    }
                }
            }
        return lowTempNoBrewWaterReady;
        }
    else
        {
        return true;
        }
    }

void SystemManager::checkAndPrintSystemStatus()
    {
    printTimeStandardFormat(timeString, myCalendar->getHour(), myCalendar->getMinute(), myCalendar->is24TimeFormatEnabled());

    myUI->Screen->addMessageToQueue(&SystemTimeMsg);

    if (!isWaterTemperatureReady() && !majorErrorsDetected())
        {
        int newTankHeatProgressPercent = (myTank->currentTemperatureF / (myTank->tankSetpointF - LTNBStopLimitF)) * 100;
        sprintf(heatingMessage, "%d%c COMPLETE", newTankHeatProgressPercent, '%');
        oldTankHeatProgressPercent = newTankHeatProgressPercent;
        myUI->Screen->addMessageToQueue(&SystemHeatMsg);
        }
    if (myRightSingleHead->isDispensing)
        {
        if (machineFeature->numberOfHeads == TWO_HEADS)
            {
            myUI->Screen->addMessageToQueue(&RightHeadDispensingMsg);
            }
        else
            {
            myUI->Screen->addMessageToQueue(&SingleHeadDispensingMsg);
            }
        }
    if (!myRightSingleHead->isDispensing && isWaterTemperatureReady() && !majorErrorsDetected() && !myTank->initialFill())
        {
        if (machineFeature->numberOfHeads == TWO_HEADS)
            {
            myUI->Screen->addMessageToQueue(&RightHeadReadyMsg);
            }
        else
            {
            myUI->Screen->addMessageToQueue(&SingleHeadReadyMsg);
            }
        }
    if (machineFeature->numberOfHeads == TWO_HEADS)
        {
        if (myLeftHead->isDispensing)
            {
            myUI->Screen->addMessageToQueue(&LeftHeadDispensingMsg);
            }
        if (!myLeftHead->isDispensing && isWaterTemperatureReady() && !majorErrorsDetected() && !myTank->initialFill())
            {
            myUI->Screen->addMessageToQueue(&LeftHeadReadyMsg);
            }
        }
    if (!myUI->TouchControls->communicationSuccessful)
        {
        myUI->Screen->addMessageToQueue(&TouchControlCommErrorMsg);
        }
    if (myUI->TouchControls->phantomTouchesDetected)
        {
        myUI->Screen->addMessageToQueue(&TouchControlTouchErrorMsg);
        }

    printWarmerOnMessages();

    if (myTank->waterStatus != WATER_OK)
        {
        if (myTank->waterStatus == WATER_ER1)
            {
            myUI->Screen->addMessageToQueue(&TankFillTimeoutMsg);
            }
        if (myTank->waterStatus == WATER_ER2)
            {
            myUI->Screen->addMessageToQueue(&TankThermistorErrorMsg);
#if ENGINEERING_BUILD
            sprintf(Error2Buffer, "%dF", (int) myTank->faultTemperatureF);
            myUI->Screen->addMessageToQueue(&TankError2TemperatureFoundMsg);
#endif
            }
        if (myTank->waterStatus == WATER_ER4)
            {
            myUI->Screen->addMessageToQueue(&TankHeatErrorMsg);
            }
        }
    if (waterLevelControlFault)
        {
        myUI->Screen->addMessageToQueue(&WaterLevelControlFaultMsg);
        }
    if (myTank->initialFill() && !majorErrorsDetected() && myTank->isWaterLevelADCValuesFilled)
        {
        myUI->Screen->addMessageToQueue(&FillWaterTankMsg);
        }
    }
