/* 
 * File:   TeaDispenseManager.cpp
 * Author: JTurner
 * 
 * Created on August 27, 2019, 4:18 PM
 */

#include "TeaDispenseManager.h"

TeaDispenseManager::TeaDispenseManager(uint8_t dispenseValvePin, 
        uint8_t sugarPumpPin, 
        uint8_t sideSelectPin, 
        uint8_t lockPin
) : DispenseManager(dispenseValvePin, (uint8_t *)"TeaDisp", TASK_TEA_DISPENSER)
{
    mySugarPumpPin = sugarPumpPin;
    mySideSelectPin = sideSelectPin;
    myLockPin = lockPin;
}
