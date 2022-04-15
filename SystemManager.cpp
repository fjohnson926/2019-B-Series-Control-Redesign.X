/* 
 * File:   SystemManager.cpp
 * Author: JTurner
 * 
 * Created on May 20, 2019, 11:33 AM
 */

#include "SystemManager.h"
#include "CommonScreenMessages.h"
#include "rtos_port.h"
#include "debugFunctions.h"
#include "NvParmManager.h"
#include "WtchDog.h"

SystemManager::SystemManager(TankManager * tank_p, DispenseManager * rightSingleHead_p, UIManager * UI_p, WarmerManager * WarmerManager_p) 
{
    myModel = MODEL_UNKNOWN;
    modelByte = 0;
    systemState = SYSTEM_START;
    myTank = tank_p;
    myRightSingleHead = rightSingleHead_p;
    myUI = UI_p;
    myWarmers = WarmerManager_p;
    releasedTouchValue = TOUCH_NONE;
    holdTouchValue = TOUCH_NONE;
    holdTouchTimeMs = 0;    
    lowTempNoBrewEnabled = true;
    lowTempNoBrewWaterReady = false;
    systemRunTimeMs = 0;
    changedState = true;
    approximateWaterVolForDispensingML = 2;
    waterLevelControlFault = false;
    setProgramSwitchForInput();
 
    if (xTaskCreate( systemManagementTask, "SystMng", SYSTEM_MANAGER_TASK_STACK_SIZE, this, SYSTEM_MANAGER_TASK_PRIORITY, NULL) != pdPASS) 
    {
        #if __DEBUG
            asm("bkpt");    
        #endif
    }
}

void SystemManager::systemManagementTask(void * parm)
{
    SystemManager * system = (SystemManager *)parm;
    int toggleCount = 0;
    while(1)
    {    
        if(system->myUI->TouchControls->getCurrentButtonStatus() == BUTTON_RELEASED)
        {
            system->releasedTouchValue = system->myUI->TouchControls->getLastUnreadReleasedValue();
        }
        else
        {
            system->releasedTouchValue = 0;
        }
        if (system->myUI->TouchControls->getCurrentButtonStatus() == BUTTON_HELD)
        {
            system->holdTouchValue = system->myUI->TouchControls->getCurrentButtonValue();
            system->holdTouchTimeMs = system->myUI->TouchControls->buttonHoldTimeMs;
        }
        else
        {
            system->holdTouchValue = 0;
            system->holdTouchTimeMs = 0;
        }
        system->currentTouchValue = system->myUI->TouchControls->getCurrentButtonValue();
        
        switch(system->systemState)
        {
            case SYSTEM_START:
                system->systemStartupState();
                break;
            case DEBUG_MODE:
                system->stateNormalMode();                
                break;
            case NORMAL_MODE:
                system->stateNormalMode();
                break;
            case ENERGY_SAVINGS_MODE:
                system->stateEnergySavingsMode();
                break;
            case MENU_BROWSE:
                system->stateMenuBrowse();
                break;
            case EDIT_DATE:
                system->stateEditDate();
                break;
            case EDIT_TIME:
                system->stateEditTime();
                break;
            case EDIT_24_HOUR_FORMAT:
                system->stateEditTimeFormat();
                break;
            case EDIT_TANK_TEMP:
                system->stateEditTankTemperature();
                break;
            case EDIT_ENERGY_SAVINGS:
                system->stateEditEnergySavingsMainHandler();
                break;
            case EDIT_COFFEE_RECIPE:
                system->stateEditCoffeeRecipeMain();
                break;
            case TEACH_MODE:
                system->StateTeachModeMain();
                break;
            case SCREEN_TEST:
                system->stateScreenTest();
                break;
            case SHOW_BREW_COUNTS:
                system->StateBrewCountsMain();
                break;
            case EN_DIS_LOW_TEMP_NO_BREW:
                system->StateEditLowTempNoBrew();
                break;
            case RESET_DEFAULT:
                system->StateResetSystemSettings();
                break;
            case SYSTEM_STATE_AUTO_TEST:
                system->StateAutomaticTestMode();
                break;
            case VIEW_TEMPERATURES:
                system->stateViewTemperatures();
                break;
            case VIEW_PRE_ERR4_TEMP_LOG:
                system->stateViewPreError4Temperatures();
                break;
            case VIEW_POST_ERR4_TEMP_LOG:
                system->stateViewPostError4Temperatures();
                break;
            case VIEW_WATER_LEVELS:
                system->stateViewWaterLevels();
                break;
            case STANDBY_MODE:
                system->stateStandbyMode();
                break;
            case EDIT_DRIPOUT_TIME:
                system->stateEditBrewLockoutTime();
                break;
            case FCT_MAIN_MENU:
                system->stateFCTMenuBrowse();
                break;
            case FCT_CTRL_COFFEE_OUTPUT:
                system->stateManageCoffeeOutputs();
                break;
            case FCT_CTRL_TEA_OUTPUT:
                system->stateManageTeaOutputs();
                break;
            case FCT_CTRL_TANK_OUTPUT:
                system->stateManageTankOutputs();
                break;
            case FCT_CTRL_LEDS:
                system->stateManageLEDOutputs();
                break;
            case FCT_READ_TANK_INPUTS:
                system->stateFCTViewTankInputs();
                break;
            case FCT_READ_REED_SWITCH:
                system->stateFCTViewReedInputs();
                break;
            case FCT_READ_DIP_SWITCH:
                system->stateFCTViewDipSwitchInputs();
                break;
            case FCT_READ_ON_BRD_USER_INPUTS:
                system->stateFCTViewOnBoardUserInputs();
                break;
            case FCT_READ_TOUCH_SENSOR:
                system->stateFCTViewTouchSensorStatus();
                break;
            case FCT_FLASH_RAM_TEST:
                system->stateFCTSPIFlashTest();
                break;
            case FCT_ALL_OUTPUTS_ON_TEST:
                system->stateFCTAllOutputsOnTest();
                break;
        }
        toggleCount += SYSTEM_MANAGER_TASK_RATE_MS;
        system->systemRunTimeMs += SYSTEM_MANAGER_TASK_RATE_MS;
        if(toggleCount >= POWER_LED_FLASH_RATE_MS)
        {
            toggleCount = 0;
            system->myUI->LEDs->debugLed->toggleLED();
        }        
        
        WDogSetFlg(TASK_SYS_MANAGER);
        os_sleep(SYSTEM_MANAGER_TASK_RATE_MS/portTICK_PERIOD_MS);
    }
}

void SystemManager::setModelPinsForInput()
{    
    gpio_set_pin_direction(MODEL_BIT1_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(MODEL_BIT1_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(MODEL_BIT1_PIN,GPIO_PULL_OFF);
    
    gpio_set_pin_direction(MODEL_BIT2_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(MODEL_BIT2_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(MODEL_BIT2_PIN,GPIO_PULL_OFF);
    
    gpio_set_pin_direction(MODEL_BIT3_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(MODEL_BIT3_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(MODEL_BIT3_PIN,GPIO_PULL_OFF);
    
    gpio_set_pin_direction(MODEL_BIT4_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(MODEL_BIT4_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(MODEL_BIT4_PIN,GPIO_PULL_OFF);
    
    gpio_set_pin_direction(MODEL_BIT5_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(MODEL_BIT5_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(MODEL_BIT5_PIN,GPIO_PULL_OFF);
    
    gpio_set_pin_direction(MODEL_BIT6_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(MODEL_BIT6_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(MODEL_BIT6_PIN,GPIO_PULL_OFF);    
}

void SystemManager::checkModelByte()
{
    //Current support (5/20/19) up to 64 different distinct models
    switch(modelByte)
    {
        case 1:
            myModel = MODEL_BID;
            machineFeature = &machineFeaturesBID;
            break;
        case 2:
            myModel = MODEL_B3;
            machineFeature = &machineFeaturesB3;
            break;
        case 3:
            myModel = MODEL_B3WL;
            machineFeature = &machineFeaturesB3WL;
            break;
        case 4:
            myModel = MODEL_B3WR;
            machineFeature = &machineFeaturesB3WR;
            break;
        case 5:
            myModel = MODEL_B6;
            machineFeature = &machineFeaturesB6;
            break;
        case 6:
            myModel = MODEL_BSAP;
            machineFeature = &machineFeaturesBSAP;
            break;           
        case 7:
            myModel = MODEL_BSGP;
            machineFeature = &machineFeaturesBSGP;
            break;           
        case 8:
            myModel = MODEL_BDAP;
            machineFeature = &machineFeaturesBDAP;
            break;            
        case 9:
            myModel = MODEL_BDGP;
            machineFeature = &machineFeaturesBDGP;
            break;            
        case 20:
            myModel = MODEL_TEA_NO_SWEET;
            machineFeature = &machineFeaturesNonSweetTea;
            break;            
        default:
            myModel = MODEL_UNKNOWN;
            machineFeature = &machineFeaturesB6;
            break;
    }
}

void SystemManager::readModelInformation()
{
    modelByte = 0;
    setModelPinsForInput(); 
    if (!gpio_get_pin_level(MODEL_BIT1_PIN))
    {
        modelByte |= 1<<0;
    }   
    if (!gpio_get_pin_level(MODEL_BIT2_PIN))
    {
        modelByte |= 1<<1;
    }
    if (!gpio_get_pin_level(MODEL_BIT3_PIN))
    {
        modelByte |= 1<<2;
    }
    if (!gpio_get_pin_level(MODEL_BIT4_PIN))
    {
        modelByte |= 1<<3;
    }
    if (!gpio_get_pin_level(MODEL_BIT5_PIN))
    {
        modelByte |= 1<<4;
    }
    if (!gpio_get_pin_level(MODEL_BIT6_PIN))
    {
        modelByte |= 1<<5;
    }
    checkModelByte();
}

void SystemManager::monitorInletValveWhileDispensing(void)
{
    static bool initialFillCompleted = false;
    if(myTank->initialFill())
    {
        return;
    }
    else if (!initialFillCompleted && myTank->hasTankBeenFilledOnce)
    {
        initialFillCompleted = true;
        if(machineFeature->tankType == TANK_TYPE_B3)
        {
            approximateWaterVolForDispensingML = WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B3_TANK;
        }
        else
        {
            approximateWaterVolForDispensingML = WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B6_TANK;
        }
        return;
    }
    //If fill completed, assume tank is full
    if(myTank->readTankFilledEventFlag())
    {
        if(machineFeature->numberOfHeads == 2)
        {
            approximateWaterVolForDispensingML = WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B6_TANK;
        }
        else if(machineFeature->numberOfHeads == 1)
        {
            approximateWaterVolForDispensingML = WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B3_TANK;
        }
    }
    
    //If dispense valve open, the tank is losing water
    if(machineFeature->numberOfHeads == 2)
    {
        if(myLeftHead->isDispenseValveOpen())
        {
            if(approximateWaterVolForDispensingML - DISPENSE_VALVE_FLOW_RATE_ML_MS*SYSTEM_MANAGER_TASK_RATE_MS > 0)
            {
                approximateWaterVolForDispensingML -= DISPENSE_VALVE_FLOW_RATE_ML_MS*SYSTEM_MANAGER_TASK_RATE_MS;
            }
            else
            {
                approximateWaterVolForDispensingML = 0;
            }            
        }        
    }
    
    //If dispense valve open, the tank is losing water
    if(myRightSingleHead->isDispenseValveOpen())
    {
        if(approximateWaterVolForDispensingML - DISPENSE_VALVE_FLOW_RATE_ML_MS*SYSTEM_MANAGER_TASK_RATE_MS > 0)
        {
            approximateWaterVolForDispensingML -= DISPENSE_VALVE_FLOW_RATE_ML_MS*SYSTEM_MANAGER_TASK_RATE_MS;
        }
        else
        {
            approximateWaterVolForDispensingML = 0;
        }
    }
    //If the inlet valve is open, then water is going into the tank
    if(myTank->isFilling())
    {        
        approximateWaterVolForDispensingML += INLET_VALVE_FLOW_RATE_ML_MS*SYSTEM_MANAGER_TASK_RATE_MS;
        if(machineFeature->tankType == TANK_TYPE_B3)
        {
            if(approximateWaterVolForDispensingML > WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B3_TANK)
            {
                approximateWaterVolForDispensingML = WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B3_TANK;
            }
        }
        else
        {
            if(approximateWaterVolForDispensingML > WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B6_TANK)
            {
                approximateWaterVolForDispensingML = WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B6_TANK;
            }            
        }
    }    
    if(approximateWaterVolForDispensingML < 1.0)
    {
        waterLevelControlFault = true;
        myTank->disableHeating();
    }    
}

void SystemManager::setProgramSwitchForInput()
{
    gpio_set_pin_direction(PROG_LOCKOUT_SW_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(PROG_LOCKOUT_SW_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(PROG_LOCKOUT_SW_PIN,GPIO_PULL_OFF);
}

bool SystemManager::isProgramLockoutEnabled()
{
    return !gpio_get_pin_level(PROG_LOCKOUT_SW_PIN);
}


void SystemManager::changeState(systemState_t newState)
{
    systemState = newState;
    myUI->Screen->resetScreen();
    changedState = true;
}

bool SystemManager::majorErrorsDetected()
{
    if(myTank->waterStatus != WATER_OK || !myUI->TouchControls->communicationSuccessful || waterLevelControlFault || myUI->TouchControls->phantomTouchesDetected)
    {
        return true;
    }
    return false;
}