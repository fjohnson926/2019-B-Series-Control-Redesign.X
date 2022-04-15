/* 
 * File:   WarmerManager.cpp
 * Author: JTurner
 * 
 * Created on June 10, 2019, 9:35 AM
 */

#include "WarmerManager.h"
#include "WtchDog.h"

Warmer Warmer1(WARMER0_PIN, (uint8_t*)"WrmTsk0", TASK_WARM_0);
Warmer Warmer2(WARMER1_PIN, (uint8_t*)"WrmTsk1", TASK_WARM_1);
Warmer Warmer3(WARMER2_PIN, (uint8_t*)"WrmTsk2", TASK_WARM_2);
Warmer Warmer4(WARMER3_PIN, (uint8_t*)"WrmTsk3", TASK_WARM_3);
Warmer Warmer5(WARMER4_PIN, (uint8_t*)"WrmTsk4", TASK_WARM_4);
Warmer Warmer6(WARMER5_PIN, (uint8_t*)"WrmTsk5", TASK_WARM_5);

WarmerManager::WarmerManager() 
{
    myWarmer1 = &Warmer1;
    myWarmer2 = &Warmer2;
    myWarmer3 = &Warmer3;
    myWarmer4 = &Warmer4;
    myWarmer5 = &Warmer5;
    myWarmer6 = &Warmer6;
}

void WarmerManager::turnOffWarmers(void)
{
    if(myWarmer1)
    {
        myWarmer1->stopWarming();
    }
    if(myWarmer2)
    {
        myWarmer2->stopWarming();
    }
    if(myWarmer3)
    {
        myWarmer3->stopWarming();
    }
    if(myWarmer4)
    {
        myWarmer4->stopWarming();
    }
    if(myWarmer5)
    {
        myWarmer5->stopWarming();
    }
    if(myWarmer6)
    {
        myWarmer6->stopWarming();
    }
}

