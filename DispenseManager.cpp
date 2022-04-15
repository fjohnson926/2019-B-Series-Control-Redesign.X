/* 
 * File:   DispenseManager.cpp
 * Author: JTurner
 * 
 * Created on May 20, 2019, 1:53 PM
 */

#include "DispenseManager.h"
#include "CoffeeBeverage.h"
#include "rtos_port.h"
#include "WtchDog.h"

int DispenseManager::brewLockoutTimeMs = 0;

DispenseManager::DispenseManager(uint8_t dispenseValvePin, uint8_t * taskName, WDogTaskIdx_e WdogFlag) 
{
    myDispenseValvePin = dispenseValvePin;
    gpio_set_pin_direction(myDispenseValvePin, GPIO_DIRECTION_OUT); 
    gpio_set_pin_function(myDispenseValvePin, GPIO_PIN_FUNCTION_OFF);

    memcpy(TaskName, taskName, 8);
    
    isDispensing = false;
    assignedRecipeLoaded = false;
    dispenseCompleteFlag = false;
    WtchDog_Flag = WdogFlag;
    
    closeDispenseValve();
    
    startDispSmpr = xSemaphoreCreateBinary();
    stopDispSmpr = xSemaphoreCreateBinary();
    if( (startDispSmpr == NULL) || (stopDispSmpr == NULL) ){
        #if __DEBUG
           asm("bkpt");    
        #endif
    }
    else{
        if(xTaskCreate(coffeeDispenseTask, (const char*)TaskName, DISPENSE_TASK_STACK_SIZE, this, TASK_DISPENSE_PRIORITY, &dispenseTask) != pdPASS){
            #if __DEBUG
                asm("bkpt");    
            #endif
        } 
    }
}

void DispenseManager::loadCoffeeRecipe(CoffeeBeverage recipe)
{
    assignedRecipe = recipe;
    assignedRecipeLoaded = true;
}

void DispenseManager::startCoffeeDispenseTask()
{
    if(!isDispensing)   {   
        xSemaphoreGive( startDispSmpr); 
    }
}

void DispenseManager::coffeeDispenseTask(void * parm)
{
    DispenseManager * head = (DispenseManager * )parm;
    uint8_t stopTask = false;
    uint8_t pulseNumber;
    uint8_t timePulse;
    
    while(1)    {   
        if( xSemaphoreTake( head->startDispSmpr, ( TickType_t ) 100 ) == pdTRUE ){
            head->isDispensing = true;        
            stopTask = false;
            pulseNumber = head->assignedRecipe.getPulseCount();

            for(int i = 0; (i < pulseNumber) && !stopTask; i++)
            {
                head->openDispenseValve();
                timePulse = (head->assignedRecipe.size[0].pulseOnTimeSeconds[i]);
                #if (EN_DEBUG_DISPENSE == true)
                    timePulse = 1;
                #endif
                for(int j=0; (j < timePulse) && !stopTask; j++){    
                    WDogSetFlg((WDogTaskIdx_e)head->WtchDog_Flag);
                    //os_sleep((900)/portTICK_PERIOD_MS);
                    if( xSemaphoreTake( head->stopDispSmpr, ( TickType_t ) 1000 ) == pdTRUE ){
                        stopTask = true;
                    }
                }
                    
                head->closeDispenseValve();
                timePulse = (head->assignedRecipe.size[0].pulseOffTimeSeconds[i]);
                #if (EN_DEBUG_DISPENSE == true)
                    timePulse = 2;
                #endif
                for(int j=0; (j < timePulse) && !stopTask; j++){
                    WDogSetFlg((WDogTaskIdx_e)head->WtchDog_Flag); 
                    //os_sleep((900)/portTICK_PERIOD_MS);
                    if( xSemaphoreTake( head->stopDispSmpr, ( TickType_t ) 1000 ) == pdTRUE ){
                        stopTask = true;
                    }
                }                    
            }

            WDogSetFlg((WDogTaskIdx_e)head->WtchDog_Flag);
            head->isDispensing = false;
            
            if(!stopTask){
                head->dispenseCompleteFlag = true;
            }
        }
        else{
            head->isDispensing = false;
            WDogSetFlg((WDogTaskIdx_e)head->WtchDog_Flag);
        }
    }
}

bool DispenseManager::getDispenseCompleteStatus()
{
    if(dispenseCompleteFlag)
    {
        dispenseCompleteFlag = false;
        return true;
    }
    else
    {
        return false;
    }    
}

void DispenseManager::stopDispense()
{
    if(dispenseTask && isDispensing)    {
        xSemaphoreGive( stopDispSmpr);
    }
}

void DispenseManager::openDispenseValve()
{
    gpio_set_pin_level(myDispenseValvePin, 1);
}

void DispenseManager::closeDispenseValve()
{
    gpio_set_pin_level(myDispenseValvePin, 0);
}

bool DispenseManager::isDispenseValveOpen()
{
    return gpio_get_pin_level(myDispenseValvePin);
}

void DispenseManager::setBrewLockoutTime(int seconds, int defaultTimeSeconds)
{
    if(seconds <= MAX_LOCKOUT_TIME)
    {
        brewLockoutTimeMs = seconds*1000;
    }
    else
    {
        brewLockoutTimeMs = defaultTimeSeconds * 1000;
    }
}