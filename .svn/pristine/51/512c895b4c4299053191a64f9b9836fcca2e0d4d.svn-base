/* 
 * File:   WarmerManager.cpp
 * Author: JTurner
 * 
 * Created on June 10, 2019, 8:35 AM
 */

#include "WtchDog.h"
#include "string.h"

#include "WarmerManager.h"
#include "rtos_port.h"


Warmer::Warmer(uint8_t warmerPin, uint8_t * taskName, WDogTaskIdx_e WdogFlag)    {
    warmingTaskHandle   = NULL;
    startWarmSmpr       = NULL;
    stopWarmSmpr        = NULL;
    
    myWarmerPin = warmerPin;
    WtchDog_Flag = WdogFlag;
    isWarmingAlgorithmActive = false;
    
    memcpy(TaskName, taskName, 8);
     
    gpio_set_pin_direction(myWarmerPin, GPIO_DIRECTION_OUT); 
    gpio_set_pin_function(myWarmerPin, GPIO_PIN_FUNCTION_OFF);
    gpio_set_drvstr(myWarmerPin);
    
    turnOffElement();    
    
    startWarmSmpr   = xSemaphoreCreateBinary();
    stopWarmSmpr    = xSemaphoreCreateBinary();
    if( (startWarmSmpr == NULL) || (stopWarmSmpr == NULL) ){
        #if __DEBUG
           asm("bkpt");    
        #endif
    }
    else{
        if(xTaskCreate(warmingTask, (const char*)TaskName, WARMER_TASK_STACK_SIZE, this, WARMING_TASK_PRIORITY, &warmingTaskHandle) != pdPASS){
            #if __DEBUG
                asm("bkpt");    
            #endif
        } 
    }
}

void Warmer::startWarmingTask() {   
    if(!isWarmingAlgorithmActive)   {
        xSemaphoreGive( startWarmSmpr); 
    }
}

void Warmer::stopWarming()  {
    if(isWarmingAlgorithmActive)    {
        xSemaphoreGive( stopWarmSmpr);
    }
}

int Warmer::getWarmerOnTimeMs()
{
    return SHUTTLE_ON_TIME_MED_SETTING_MS;
}

int Warmer::getWarmerOffTimeMs()
{
    return SHUTTLE_OFF_TIME_MED_SETTING_MS;
}

int Warmer::getWarmerOnTimeSec()
{
    return SHUTTLE_ON_TIME_MED_SETTING_SEC;
}

int Warmer::getWarmerOffTimeSec()
{
    return SHUTTLE_OFF_TIME_MED_SETTING_SEC;
}

void Warmer::warmingTask(void * parm)   {
    uint8_t stopTask = false;
    Warmer * warmer_p = (Warmer *)parm;
    
    while(1)    {
        if( xSemaphoreTake( warmer_p->startWarmSmpr, ( TickType_t ) 100 ) == pdTRUE ){
            stopTask = false;
            
            while(!stopTask){
                warmer_p->isWarmingAlgorithmActive = true;
                WDogSetFlg    ( warmer_p->WtchDog_Flag );
                
                warmer_p->turnOnElement();
                for(int j=0; (j < warmer_p->getWarmerOnTimeSec()) && !stopTask; j++){
                    if( xSemaphoreTake( warmer_p->stopWarmSmpr, ( TickType_t ) 1000 ) == pdTRUE ){
                        warmer_p->turnOffElement();
                        stopTask = true;
                    }
                    WDogSetFlg    ( warmer_p->WtchDog_Flag );
                }

                warmer_p->turnOffElement();
                for(int j=0; (j < warmer_p->getWarmerOffTimeSec()) && !stopTask ; j++){ 
                    if( xSemaphoreTake( warmer_p->stopWarmSmpr, ( TickType_t ) 1000 ) == pdTRUE ){
                        warmer_p->turnOffElement();
                        stopTask = true;
                    }
                    WDogSetFlg    ( warmer_p->WtchDog_Flag );
                }
            }
            
            warmer_p->isWarmingAlgorithmActive = false;
            WDogSetFlg    ( warmer_p->WtchDog_Flag );
        }
        else{
            warmer_p->isWarmingAlgorithmActive = false;
            WDogSetFlg    ( warmer_p->WtchDog_Flag );
        }
    }
}




void Warmer::turnOffElement()
{
    gpio_set_pin_level(myWarmerPin, 0);
}

void Warmer::turnOnElement()
{
    gpio_set_pin_level(myWarmerPin, 1);
}

