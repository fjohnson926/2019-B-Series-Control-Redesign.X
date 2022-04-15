/* 
 * File:   SystemManager.h
 * Author: JTurner
 *
 * Created on May 20, 2019, 11:33 AM
 */

#include "gm_SystemPinMap.h"
#include "DispenseManager.h"
#include "TankManager.h"
#include "UIManager.h"
#include "CoffeeBeverageDefaults.h"
#include "WarmerManager.h"
#include "Calendar.h"
#include "StringManipulation.h"
#include "MachineConfiguration.h"
#include "SPIFlash.h"
#include "Blobs.h"
#include "EnergySavingsManager.h"
#include "RAMRecipes.h"
#include "debugFunctions.h"

#ifndef SYSTEMMANAGER_H
#define	SYSTEMMANAGER_H

#define ENGINEERING_BUILD 0

//Quickly calculated this value from dispensing water in a B3 unit 1520mL in 120s
#define DISPENSE_VALVE_FLOW_RATE_ML_MS .01267
//Empirically determined. Drained tank to disp valve. Timed refill. Determined to be around 140mL
#define INLET_VALVE_FLOW_RATE_ML_MS .140
//Empirically determined. Let tank completely fill. shorted sensors, started two dispenses on B6. Got about 2400mL max of water during dispenses
#define WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B6_TANK  2400
//Empirically determined. Let tank completely fill. shorted sensors. started dispense. Got on average about 1150mL water during dispense
#define WATER_VOL_BETWEEN_SENSOR_AND_DISP_VALVE_B3_TANK 1100

#define MODEL_BYTE_FCT_MODE 63
#define FCT_OUTPUT_TIME_MS 5000

#define SYSTEM_MANAGER_TASK_STACK_SIZE 5000
#define SYSTEM_MANAGER_TASK_PRIORITY 4
#define SYSTEM_MANAGER_TASK_RATE_MS 50
#define HOLD_TIME_BEFORE_ENTER_MENU_MS 5000
#define POWER_LED_FLASH_RATE_MS 1000

#define RIGHT_SINGLE_HEAD_INDEX 0
#define LEFT_HEAD_INDEX 1

typedef enum SUPPORT_MODELS
{
    MODEL_UNKNOWN,
    MODEL_BID,
    MODEL_B3,
    MODEL_B3WL,
    MODEL_B3WR,
    MODEL_B6,
    MODEL_BSAP,
    MODEL_BDAP,
    MODEL_BSGP,
    MODEL_BDGP,
    MODEL_RESERVED01,
    MODEL_RESERVED02,
    MODEL_RESERVED03,
    MODEL_RESERVED04,
    MODEL_RESERVED05,
    MODEL_RESERVED06,
    MODEL_RESERVED07,
    MODEL_RESERVED08,
    MODEL_RESERVED09,
    MODEL_RESERVED10,
    MODEL_TEA_SWEET,
    MODEL_TEA_NO_SWEET,
}systemModel_t;

typedef enum SYSTEM_STATE
{
    SYSTEM_START,
    MENU_BROWSE,
    SLEEP_MODE,
    NORMAL_MODE,
    STANDBY_MODE,
    ENERGY_SAVINGS_MODE,
    DEBUG_MODE,
    EDIT_DATE,
    EDIT_TIME,
    EDIT_24_HOUR_FORMAT,
    EDIT_TANK_TEMP,
    EDIT_ENERGY_SAVINGS,
    EDIT_COFFEE_RECIPE,
    TEACH_MODE,
    SCREEN_TEST,
    SHOW_BREW_COUNTS,
    EN_DIS_LOW_TEMP_NO_BREW,
    RESET_DEFAULT,
    SYSTEM_STATE_AUTO_TEST,
    VIEW_TEMPERATURES,
    VIEW_PRE_ERR4_TEMP_LOG,
    VIEW_POST_ERR4_TEMP_LOG,
    VIEW_WATER_LEVELS,
    EDIT_DRIPOUT_TIME,
    FCT_MAIN_MENU,
    FCT_CTRL_COFFEE_OUTPUT,
    FCT_CTRL_TEA_OUTPUT,
    FCT_CTRL_TANK_OUTPUT,
    FCT_READ_TANK_INPUTS,
    FCT_CTRL_LEDS,
    FCT_READ_DIP_SWITCH,
    FCT_READ_REED_SWITCH,
    FCT_READ_ON_BRD_USER_INPUTS,
    FCT_READ_TOUCH_SENSOR,
    FCT_FLASH_RAM_TEST,
    FCT_ALL_OUTPUTS_ON_TEST,
            
}systemState_t;

class SystemManager {
public:
    SystemManager(TankManager * tank_p, DispenseManager * rightSingleHead_p, UIManager * UI_p, WarmerManager * WarmerManager_p);
    signed int releasedTouchValue;
    signed int holdTouchValue;
    signed int currentTouchValue;
    int holdTouchTimeMs;
    void setModelPinsForInput();
    void readModelInformation();
    void checkModelByte();
    systemModel_t myModel;
    uint8_t modelByte;
    systemState_t systemState;
    static void systemManagementTask(void * parm);
    TankManager * myTank;
    DispenseManager * myRightSingleHead;
    DispenseManager * myLeftHead;
    UIManager * myUI;
    WarmerManager * myWarmers;
    Calendar * myCalendar;
    EnergySavingsManager * myEnergySaver;
    const machineConfig_t * machineFeature;
    SPIFlash * NVBlobs;
    uint64_t systemRunTimeMs;
    float approximateWaterVolForDispensingML;
    bool waterLevelControlFault;
    
    void initTeaGPIO(void);
    
    void stateNormalMode();
    void stateEnergySavingsMode();
    void systemStartupState();
    void checkAndPrintSystemStatus();
    void buildSystem();
    void stateTouchTest();
    void stateEditDate();
    void stateEditTimeFormat();
    void stateEditBrewLockoutTime();
    void stateMenuBrowse();
    void stateFCTMenuBrowse();
    void stateManageCoffeeOutputs();
    void stateManageTeaOutputs();
    void stateManageTankOutputs();
    void stateManageLEDOutputs();
    void stateFCTViewTankInputs();
    void stateFCTViewReedInputs();
    void stateFCTViewDipSwitchInputs();
    void stateFCTViewOnBoardUserInputs();
    void stateFCTViewTouchSensorStatus();
    void stateFCTSPIFlashTest();
    void stateFCTAllOutputsOnTest(void);

    void editMonth();
    void editDay();
    void editYear();
    void stateEditTime();
    void editHour();
    void editMinute();
    void stateEditTankTemperature();
    void stateViewTemperatures();
    void stateViewWaterLevels();
    
    void energySavingsEnableDisableHandler(void);
    void energySavingsIdleTimeHandler(void);
    void enableDisableAutoExitEnergySavings(void);
    void energySavingsAutoWakeupHourHandler(void);
    void energySavingsAutoWakeupMinHandler(void);
    void stateEditEnergySavingsMainHandler();
    void stateStandbyMode();
    
    
    void StateTeachModeMain(void);
    void teachModeCheckForSystemErrorState();
    void teachModeCheckTankTemperature();
    bool majorErrorsDetected();
    void teachModeHeadSelectHandler();
    void handleTeachMode(void);
    void teachModeSaveState(void);
    
    
    void headSelectHandler();
    void pulseSelectHandler(void);
    void stateEditCoffeeRecipeMain(void);
    void stateEditCoffeeRecipeInitialize(void);
    void editEntireRecipeHandler(void);
    void editSelectedPulseOnHandler(void);
    void editSelectedPulseOffHandler(void);
    
    void selectBrewCounter();
    void selectHeadAndShowResetableCounter();
    void selectHeadAndShowNonResetableCounter();
    void resetCounter();
    void StateBrewCountsMain();
    
    
    //Normal Mode
    void manageWarmersInNormalMode(Warmer * warmer_p, Led * warmerLED_p, signed int warmerTouchTag, const ScreenMessages * activeMsg_p, const ScreenMessages * inActiveMsg_p);
    void manageDispenseManagersInNormalMode(DispenseManager * dispense_p, TriColorLed * Leds_p, signed int dispTouchTag, const ScreenMessages * startMsg_p, const ScreenMessages * stopMsg_p, uint8_t recipeIndex);
    void manageFeaturedWarmersInNormalMode();
    void turnOffWarmers(void);
    void printWarmerOnMessages(void);
    bool isWaterTemperatureReady();
    const float LTNBStopLimitF = 5;
    const float LTNBStartLimitF = 7;
    bool lowTempNoBrewEnabled;
    bool lowTempNoBrewWaterReady;
    bool showTempReadyMsg = false;
    
    //common features
    void changeState(systemState_t newState);
    bool changedState;
    
    void stateScreenTest(void);
    
    void setProgramSwitchForInput();
    bool isProgramLockoutEnabled();
    
    void StateEditLowTempNoBrew(void);    
    
    void StateResetSystemSettings(void);
    
    void StateAutomaticTestMode(void);
    void AutoTestModeSystemLedHandler(void);
    void AutoTestModeTouchHandler(void);
    void AutoTestModeScreenHandler(void);
    void AutoTestModeDispenseHandler(void);
    
    void stateViewPreError4Temperatures();
    void stateViewPostError4Temperatures();
    
    void turnOffCoffeeOutputs(void);
    void turnOffTeaOutputs(void);
    void turnOffTankOutputs(void);
    void turnOffLEDOutputs(void);
    void turnOnAllOutputs(void);
    void turnOffAllOutputs(void);
    
    void monitorInletValveWhileDispensing();
    
    
private:

};

#endif	/* SYSTEMMANAGER_H */

