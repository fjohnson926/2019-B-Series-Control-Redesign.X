/* 
 * File:   Led.cpp
 * Author: JTurner
 * 
 * Created on May 6, 2019, 1:53 PM
 */

#include "Led.h"

Led::Led(uCPin pin) {
    myPin = pin;
    SET_PIN_FOR_GPIO_OUTPUT(myPin);
    turnOffLed();    
}

void Led::turnOffLed()
{
    LED_SET_GPIO_LOW(myPin);
}

void Led::turnOnLED()
{
    LED_SET_GPIO_HIGH(myPin);
}

void Led::toggleLED()
{
    LED_TOGGLE_GPIO_PIN(myPin);
}