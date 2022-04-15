/* 
 * File:   TriColorLed.cpp
 * Author: JTurner
 * 
 * Created on May 6, 2019, 3:05 PM
 */

#include "TriColorLed.h"
#include "LedHal.h"

TriColorLed::TriColorLed(uCPin red, uCPin green, uCPin blue) 
{
    myRedPin = red;
    SET_PIN_FOR_GPIO_OUTPUT(myRedPin);
    myGreenPin = green;
    SET_PIN_FOR_GPIO_OUTPUT(myGreenPin);
    myBluePin = blue;
    SET_PIN_FOR_GPIO_OUTPUT(myBluePin);
    TurnOffAllLeds();    
}

void TriColorLed::setColorRed()
{
    TurnOffAllLeds();
    LED_SET_GPIO_HIGH(myRedPin);
}

void TriColorLed::setColorGreen()
{
    TurnOffAllLeds();
    LED_SET_GPIO_HIGH(myGreenPin);
}

void TriColorLed::setColorBlue()
{
    TurnOffAllLeds();
    LED_SET_GPIO_HIGH(myBluePin);
}

void TriColorLed::setColorCyan()
{
    TurnOffAllLeds();
    LED_SET_GPIO_HIGH(myBluePin);
    LED_SET_GPIO_HIGH(myGreenPin);
}

void TriColorLed::setColorWhite()
{
    TurnOffAllLeds();
    LED_SET_GPIO_HIGH(myRedPin);
    LED_SET_GPIO_HIGH(myBluePin);
    LED_SET_GPIO_HIGH(myGreenPin);
}

void TriColorLed::TurnOffAllLeds()
{
    LED_SET_GPIO_LOW(myRedPin);
    LED_SET_GPIO_LOW(myGreenPin);
    LED_SET_GPIO_LOW(myBluePin);
}

