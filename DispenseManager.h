/* 
 * File:   DispenseManager.h
 * Author: JTurner
 *
 * Created on May 20, 2019, 1:53 PM
 */

#ifndef DISPENSEMANAGER_H
#define	DISPENSEMANAGER_H

#include "stdint.h"
#include "hal_gpio.h"
#include "hal_rtos.h"
#include "CoffeeBeverage.h"

#include "WtchDog.h"
#define DISPENSE_TASK_STACK_SIZE 200 
#define TASK_DISPENSE_PRIORITY (tskIDLE_PRIORITY + 1)

#define EN_DEBUG_DISPENSE ( false )

#define MIN_LOCKOUT_TIME 0
#define MAX_LOCKOUT_TIME 255

class DispenseManager {
public:
    DispenseManager(uint8_t dispenseValvePin, uint8_t * taskName, WDogTaskIdx_e WdogFlag);
    
    uint8_t myDispenseValvePin;
    
    TaskHandle_t        dispenseTask;
    SemaphoreHandle_t   startDispSmpr;
    SemaphoreHandle_t   stopDispSmpr;
    
    uint8_t             TaskName[8];
    
    bool isDispensing;
    bool dispenseCompleteFlag;
    //If dispense was completed, set back to false when read.
    bool getDispenseCompleteStatus();
    CoffeeBeverage assignedRecipe;
    bool assignedRecipeLoaded;
    bool dripoutActive;
    static int brewLockoutTimeMs;
    static void setBrewLockoutTime(int seconds, int defaultTimeSeconds);
    
    void startCoffeeDispenseTask();
    void startCoffeeDispenseTask(uint8_t TaskName);
    static void coffeeDispenseTask(void * parm); 
    void stopDispense();
    bool isDispenseValveOpen();
    void openDispenseValve();
    void closeDispenseValve();
    void loadCoffeeRecipe(CoffeeBeverage recipe);
    bool taskCreationFailed;
    
    WDogTaskIdx_e WtchDog_Flag;
private:

};

#endif	/* DISPENSEMANAGER_H */

