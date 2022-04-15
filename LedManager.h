/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Electrolux

  @File Name
    LedManager.h

  @Summary
    This is a B-series class that simply holds all of the LEDs on the UI Board

  @Description
    
 */
/* ************************************************************************** */

#ifndef LEDMANAGER_H
#define	LEDMANAGER_H

#include "Led.h"
#include "TriColorLed.h"
#include "gm_SystemPinMap.h"

class LedManager {
public:
    LedManager();
    Led * powerLed;
    Led * basketLeftLed;
    Led * basketCenterLed;
    Led * basketRightLed;
    Led * warmer01Led;
    Led * warmer02Led;
    Led * warmer03Led;
    Led * warmer04Led;
    Led * warmer05Led;
    Led * warmer06Led;
    Led * debugLed;
    TriColorLed * leftSingleBrewLeds;
    TriColorLed * rightBrewLeds;
    void nonOSLightShow();
    void turnOffAllLEDs(bool includePowerLED);
    void turnOnAllLEDs();
private:
    
};

#endif	/* LEDMANAGER_H */

