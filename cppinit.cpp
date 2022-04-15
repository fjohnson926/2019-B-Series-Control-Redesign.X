/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "gm_SystemPinMap.h"
#include "cppinit.h"
#include "CapsenseController.h"
#include "DispenseManager.h"
#include "TextLCD.h"
#include "ScreenManager.h"
#include "SystemManager.h"
#include "Led.h"
#include "LedManager.h"
#include "TankManager.h"
#include "UIManager.h"
#include "WarmerManager.h"
#include "Calendar.h"
#include "StringManipulation.h"
#include "SPIFlash.h"
#include "Blobs.h"
#include "NvParmManager.h"
#include "TeaDispenseManager.h"
#include "EnergySavingsManager.h"
#include "RAMRecipes.h"

TextLCD Display;
ScreenManager ScreenMgr(&Display);
LedManager LEDs;
CapsenseController Touch;
UIManager UI(&LEDs, &ScreenMgr, &Touch);
TankManager Tank(INLET_CONTROL_PIN, HEAT_CONTROL_PIN);

DispenseManager rightSingleHead (DISP_VALVE_RS_PIN, (uint8_t*)"RghDisp", TASK_RGH_DISPENSER);
DispenseManager leftHead        (DISP_VALVE_LEFT_PIN, (uint8_t*)"LftDsp", TASK_LFT_DISPENSER);

TeaDispenseManager rightSingleTeaHead(DISP_VALVE_LEFT_PIN,SUGAR0_PIN,SIDE_SELECT_PIN,LOCK_PIN);
WarmerManager Warmers;
Calendar SystemCalendar;
SPIFlash NVMemory;
SystemManager System(&Tank, &rightSingleHead, &UI, &Warmers);
EnergySavingsManager EnergySaver;

static void displaySetup(void)
{
    //Common machine configuration
    EightDataModePinOut_t displayPins;
    displayPins.E_PIN  = PC21_PIN;
    displayPins.RS_PIN = PB19_PIN;
    displayPins.RW_PIN = PB17_PIN;
    displayPins.D0     = PB10_PIN;
    displayPins.D1     = PA10_PIN;
    displayPins.D2     = PA08_PIN;
    displayPins.D3     = PC06_PIN;
    displayPins.D4     = PB08_PIN;
    displayPins.D5     = PB07_PIN;
    displayPins.D6     = PC05_PIN;
    displayPins.D7     = PC07_PIN;
    Display.initialize(displayPins);
}

static void setupDispenseHeads(void)
{
    if(System.machineFeature->brewerType == BREWER_TYPE_COFFEE)
    {
       System.myLeftHead =  &leftHead;    
    }
    else if(System.machineFeature->brewerType == BREWER_TYPE_TEA)
    {
        System.myRightSingleHead = &rightSingleTeaHead;
        System.myLeftHead =  NULL; 
    } 
}

static void mapAndLoadNVParmsFromNVRAM(void)
{
    NvParm parmRead;
    //(NVMemory is a SPIFlash Object)
    System.NVBlobs = &NVMemory; 
    NvParmManager::mapParm(PARMINDEX_LOW_TEMP_NO_BREW, PARMDEFAULT_LOW_TEMP_NO_BREW);
    NvParmManager::mapParm(PARMINDEX_TANK_SETPOINT_F, (float)PARMDEFAULT_TANK_SETPOINT_F);
    NvParmManager::mapParm(PARM_INDEX_ENERGY_SAVINGS_ENABLE, PARMDEFAULT_ENERGY_SAVINGS_ENABLE);
    NvParmManager::mapParm(PARM_INDEX_ENERGY_SAVINGS_IDLE_MINS, PARMDEFAULT_IDLE_MINS);
    NvParmManager::mapParm(PARM_INDEX_ENERGY_SAVINGS_AUTO_EXIT_ENABLE, PARMDEFAULT_ENERGY_SAVINGS_AUTO_EXIT);    
    NvParmManager::mapParm(PARM_INDEX_ENERGY_SAVINGS_EXIT_HOUR, PARMDEFAULT_ENERGY_SAVINGS_EXIT_HOUR);
    NvParmManager::mapParm(PARM_INDEX_ENERGY_SAVINGS_EXIT_MINUTE, PARMDEFAULT_ENERGY_SAVINGS_EXIT_MINUTE);
    NvParmManager::mapParm(PARM_INDEX_BREW_LOCKOUT_TIME_S, PARMDEFAULT_BREW_LOCKOUT_TIME_S);
    NvParmManager::mapParm(PARM_INDEX_24HR_TIME_FORMAT_ENABLE, PARMDEFAULT_24HR_TIME_FORMAT);    
    NvParmManager::mapParm(PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE, PARMDEFAULT_BREW_COUNT);
    NvParmManager::mapParm(PARM_INDEX_RESETABLEBREWCNT_LEFT, PARMDEFAULT_BREW_COUNT);
    NvParmManager::mapParm(PARM_INDEX_NONRESETABLEBREWCNT_RIGHTSINGLE, PARMDEFAULT_BREW_COUNT);
    NvParmManager::mapParm(PARM_INDEX_NONRESETABLEBREWCNT_LEFT, PARMDEFAULT_BREW_COUNT);
    NvParmManager::mapParm(PARM_INDEX_MODEL_LAST_MODEL_SWITCH, PARMDEFAULT_MODEL_SWITCH_CHANGE);
    
    //Assign a blob to the parms
    Blob ParmsBlob(PARMS_BANK_A_ADDRESS, PARMS_BANK_B_ADDRESS, NvParmManager::getParmTableSizeInBytes(), NvParmManager::getParmTableAddress(), PARMS_SIGNATURE);
    NVMemory.addBlob(ParmsBlob);
    
    bool validParmBank = false;
    validParmBank = NVMemory.isAtLeastOneBankValid(PARMS_READ_INDEX);    
    if(!validParmBank)
    {
        initializeParmTables__c();
        NVMemory.flushNvBlob(PARMS_READ_INDEX);
    }
    
    NVMemory.loadNvBlob(PARMS_READ_INDEX);    
    
    parmRead = NvParmManager::getNvParm(PARM_INDEX_24HR_TIME_FORMAT_ENABLE);
    System.myCalendar->setTimeFormat(parmRead.u.bool_parm);
    
    parmRead = NvParmManager::getNvParm(PARMINDEX_LOW_TEMP_NO_BREW);
    System.lowTempNoBrewEnabled = parmRead.u.bool_parm;
    
    parmRead = NvParmManager::getNvParm(PARMINDEX_TANK_SETPOINT_F);
    System.myTank->setTankSetpointF(parmRead.u.float_parm);
   
    parmRead = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_ENABLE);
    System.myEnergySaver->setEnergySavingsEnable(parmRead.u.bool_parm);
    
    parmRead = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_IDLE_MINS);
    System.myEnergySaver->setIdleTimeBeforeSleep(parmRead.u.integer_parm);
    
    parmRead = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_AUTO_EXIT_ENABLE);
    System.myEnergySaver->setAutoWakeUpEnable(parmRead.u.bool_parm);
    
    parmRead = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_EXIT_HOUR);
    System.myEnergySaver->setAutoWakeupHr(parmRead.u.integer_parm);
    
    parmRead = NvParmManager::getNvParm(PARM_INDEX_ENERGY_SAVINGS_EXIT_MINUTE);
    System.myEnergySaver->setAutoWakeupMin(parmRead.u.integer_parm);
    
    parmRead = NvParmManager::getNvParm(PARM_INDEX_BREW_LOCKOUT_TIME_S);
    DispenseManager::setBrewLockoutTime(parmRead.u.integer_parm, PARMDEFAULT_BREW_LOCKOUT_TIME_S);
    
    NvParmManager::clearResetOnDefaultParm(PARM_INDEX_NONRESETABLEBREWCNT_RIGHTSINGLE);
    NvParmManager::clearResetOnDefaultParm(PARM_INDEX_NONRESETABLEBREWCNT_LEFT);
}

static void loadCoffeeRecipesFromNVRAM(void)
{   
    bool validCoffeeRecipeBank = false;
    //Assign a blob for Coffee Recipes
    Blob CoffeeRecipeBlob(COFFEE_RECIPE_BANK_A_ADDRESS, COFFEE_RECIPE_BANK_B_ADDRESS, getCoffeeBeverageTableSizeInBytes(), getCoffeeBeverageTableAddress(), COFFEE_RECIPE_SIGNATURE);
    NVMemory.addBlob(CoffeeRecipeBlob);
    
    validCoffeeRecipeBank = NVMemory.isAtLeastOneBankValid(COFFEE_RECIPE_BLOB_INDEX);
    if(!validCoffeeRecipeBank)
    {
        setCoffeeBeverageRecipesToDefault();
        NVMemory.flushNvBlob(COFFEE_RECIPE_BLOB_INDEX);
    }
    
    NVMemory.loadNvBlob(COFFEE_RECIPE_BLOB_INDEX);
}

void checkModelParmResetSystemDataIfDifferent(void)
{
    NvParm lastModelNumberReadParm;    
    //If the model is different from the last read, reset the parameters    
    lastModelNumberReadParm = NvParmManager::getNvParm(PARM_INDEX_MODEL_LAST_MODEL_SWITCH);
    if(lastModelNumberReadParm.u.integer_parm != System.myModel)
    {
        initializeParmTables__c();
        NVMemory.flushNvBlob(PARMS_READ_INDEX);        
        setCoffeeBeverageRecipesToDefault();
        NVMemory.flushNvBlob(COFFEE_RECIPE_BLOB_INDEX);        
        lastModelNumberReadParm.u.integer_parm = System.myModel;
        NvParmManager::setNvParm(PARM_INDEX_MODEL_LAST_MODEL_SWITCH, lastModelNumberReadParm);
        NVMemory.flushNvBlob(PARMS_READ_INDEX);
    }
}

void initializeCppObjects(void)
{
    displaySetup();
    ScreenMgr.initialize(); 
    System.myCalendar = &SystemCalendar;    
    System.myEnergySaver = &EnergySaver;    
    LEDs.nonOSLightShow();    
    System.readModelInformation();
    mapAndLoadNVParmsFromNVRAM();
    loadCoffeeRecipesFromNVRAM();
    checkModelParmResetSystemDataIfDifferent();
    setupDispenseHeads();    
};
/* *****************************************************************************
 End of File
 */
