/* 
 * File:   WarmerManager.h
 * Author: JTurner
 *
 * Created on June 10, 2019, 8:35 AM
 */

#ifndef WARMER_H
#define	WARMER_H

#include "stdint.h"
#include "hal_gpio.h"
#include "hal_rtos.h"
#include "WtchDog.h"

#define WARMER_TASK_STACK_SIZE 200 
#define WARMING_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

// jtq 6/6/2018 Set times for testing on warmer plate cycle time 10 minutes
//  low = 50% 5min on; 5min off
//  med = 70% 7min on;  3min off
//  hi  = 90% 10min on;  0min off
#define SHUTTLE_ON_TIME_LOW_SETTING_MS      (05*1000*60)
#define SHUTTLE_OFF_TIME_LOW_SETTING_MS     (05*1000*60)

#define SHUTTLE_ON_TIME_MED_SETTING_MS      (07*1000*60)
#define SHUTTLE_OFF_TIME_MED_SETTING_MS     (03*1000*60)

#define SHUTTLE_ON_TIME_HI_SETTING_MS       (10*1000*60)
#define SHUTTLE_OFF_TIME_HI_SETTING_MS      (01*1000*01)    //off for 1 sec

#define SHUTTLE_ON_TIME_LOW_SETTING_SEC     (05 * 60)       // 5 minute
#define SHUTTLE_OFF_TIME_LOW_SETTING_SEC    (05 * 60)       // 5 minute

#define SHUTTLE_ON_TIME_MED_SETTING_SEC     (07 * 60)       // 7 minute
#define SHUTTLE_OFF_TIME_MED_SETTING_SEC    (03 * 60)       // 3 minute

#define SHUTTLE_ON_TIME_HI_SETTING_SEC      (10 * 60)       // 10 minute
#define SHUTTLE_OFF_TIME_HI_SETTING_SEC     (01 * 01)       // 1 second




class Warmer {
public:
    Warmer(uint8_t warmerPin, uint8_t * taskName, WDogTaskIdx_e WdogFlag) ;
    uint8_t myWarmerPin;
    
    TaskHandle_t        warmingTaskHandle;
    SemaphoreHandle_t   startWarmSmpr;
    SemaphoreHandle_t   stopWarmSmpr;
    
    bool isWarmingAlgorithmActive;
    void startWarmingTask();
    void stopWarming();
    static void warmingTask(void * parm);
    void turnOnElement();
    void turnOffElement();
    int getWarmerOnTimeMs();
    int getWarmerOffTimeMs();
    int getWarmerOnTimeSec();
    int getWarmerOffTimeSec();
    
    uint8_t             TaskName[8];
    WDogTaskIdx_e WtchDog_Flag;
private:

};

#endif	/* WARMER_H */

