/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Electrolux

  @File Name
    Led.h

  @Summary
    Led Class is a very simple class for turning an LED on and off

  @Description
    
 */
/* ************************************************************************** */

#ifndef LED_H
#define	LED_H

#include "LedHal.h"

class Led {
public:
    Led(uCPin pin);
    void turnOnLED();
    void turnOffLed();
    void toggleLED();
private:
    uCPin myPin;

};

#endif	/* LED_H */

