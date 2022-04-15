/* 
 * File:   TeaDispenseManager.h
 * Author: JTurner
 *
 * Created on August 27, 2019, 4:18 PM
 */

#ifndef TEADISPENSEMANAGER_H
#define	TEADISPENSEMANAGER_H

#include "DispenseManager.h"

class TeaDispenseManager : public DispenseManager
{
public:
    TeaDispenseManager(uint8_t dispenseValvePin, uint8_t sugarPumpPin, uint8_t sideSelectPin, uint8_t lockPin);
    uint8_t mySugarPumpPin;
    uint8_t mySideSelectPin;
    uint8_t myLockPin;
private:

};

#endif	/* TEADISPENSEMANAGER_H */

