/* 
 * File:   TriColorLed.h
 * Author: JTurner
 *
 * Created on May 6, 2019, 3:05 PM
 */

#ifndef TRICOLORLED_H
#define	TRICOLORLED_H

#include "TriColorLedHal.h"

class TriColorLed {
public:
    TriColorLed(uCPin red, uCPin green, uCPin blue);
    void TurnOffAllLeds();
    void setColorRed();
    void setColorGreen();
    void setColorBlue();
    void setColorCyan();
    void setColorWhite();
private:
    uCPin myRedPin;
    uCPin myGreenPin;
    uCPin myBluePin;
};

#endif	/* TRICOLORLED_H */

