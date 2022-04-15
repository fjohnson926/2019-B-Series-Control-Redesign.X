/* 
 * File:   WarmerManager.h
 * Author: JTurner
 *
 * Created on June 10, 2019, 9:35 AM
 */

#ifndef WARMERMANAGER_H
#define	WARMERMANAGER_H

#include "gm_SystemPinMap.h"
#include "Warmer.h"

class WarmerManager {
public:
    WarmerManager();
    Warmer * myWarmer1;
    Warmer * myWarmer2;
    Warmer * myWarmer3;
    Warmer * myWarmer4;
    Warmer * myWarmer5;
    Warmer * myWarmer6;

    void turnOffWarmers(void);
private:

};

#endif	/* WARMERMANAGER_H */

