/* 
 * File:   LedManager.cpp
 * Author: JTurner
 * 
 * Created on May 6, 2019, 2:10 PM
 */

#include "LedManager.h"
#include "hal_delay.h"

//Silk screen label
Led LED01 = Led(PC16_PIN);
Led LED02 = Led(PC18_PIN);
Led LED03 = Led(PC20_PIN);
Led CSLED04 = Led(PB18_PIN);
Led CSLED07 = Led(PC17_PIN);
Led CSLED12 = Led(PA11_PIN);
Led CSLED05 = Led(PB16_PIN);
Led CSLED10 = Led(PB14_PIN);
Led CSLED08 = Led(PC19_PIN);
Led CSLED09 = Led(PB12_PIN);
Led DEBUGLED = Led(DEBUGLED_PIN);
TriColorLed CSRBG06 = TriColorLed(PA18_PIN, PA16_PIN, PA19_PIN);
TriColorLed CSRGB11 = TriColorLed(PC10_PIN, PB13_PIN, PB11_PIN);

LedManager::LedManager() {
    warmer01Led = &CSLED05;
    warmer02Led = &CSLED12;
    warmer03Led = &CSLED07;
    warmer04Led = &CSLED10;
    warmer05Led = &CSLED08;
    warmer06Led = &CSLED09;
    basketLeftLed = &LED01;
    basketCenterLed = &LED02;
    basketRightLed = &LED03;
    powerLed = &CSLED04;
    debugLed = &DEBUGLED;    
    rightBrewLeds = &CSRGB11;
    leftSingleBrewLeds = &CSRBG06;    
}

void LedManager::nonOSLightShow()
{
    int transitionTimeMs = 100;
    powerLed->turnOnLED();
    delay_ms(transitionTimeMs);
    leftSingleBrewLeds->setColorRed();
    delay_ms(2*transitionTimeMs);
    leftSingleBrewLeds->setColorGreen();
    delay_ms(2*transitionTimeMs);
    leftSingleBrewLeds->setColorBlue();
    delay_ms(2*transitionTimeMs);
    warmer01Led->turnOnLED();
    delay_ms(transitionTimeMs);
    warmer02Led->turnOnLED();
    delay_ms(transitionTimeMs);
    warmer03Led->turnOnLED();
    delay_ms(transitionTimeMs);
    rightBrewLeds->setColorRed();
    delay_ms(2*transitionTimeMs);
    rightBrewLeds->setColorGreen();
    delay_ms(2*transitionTimeMs);
    rightBrewLeds->setColorBlue();
    delay_ms(2*transitionTimeMs);
    basketLeftLed->turnOnLED();
    delay_ms(transitionTimeMs);
    basketCenterLed->turnOnLED();
    delay_ms(transitionTimeMs);
    basketRightLed->turnOnLED();
    delay_ms(transitionTimeMs);
    warmer04Led->turnOnLED();
    delay_ms(transitionTimeMs);
    warmer05Led->turnOnLED();
    delay_ms(transitionTimeMs);
    warmer06Led->turnOnLED();
    delay_ms(4*transitionTimeMs);
    turnOffAllLEDs(false);    
}

void LedManager::turnOffAllLEDs(bool includePowerLED)
{
    if(includePowerLED)
    {
        powerLed->turnOffLed();
    }
    leftSingleBrewLeds->TurnOffAllLeds();
    rightBrewLeds->TurnOffAllLeds();
    warmer01Led->turnOffLed();
    warmer02Led->turnOffLed();
    warmer03Led->turnOffLed();
    warmer04Led->turnOffLed();    
    warmer05Led->turnOffLed();    
    warmer06Led->turnOffLed();    
    basketLeftLed->turnOffLed();    
    basketCenterLed->turnOffLed();    
    basketRightLed->turnOffLed();
}


void LedManager::turnOnAllLEDs()
{      
    powerLed->turnOnLED();    
    leftSingleBrewLeds->setColorWhite();
    rightBrewLeds->setColorWhite();
    warmer01Led->turnOnLED();
    warmer02Led->turnOnLED();
    warmer03Led->turnOnLED();
    warmer04Led->turnOnLED();    
    warmer05Led->turnOnLED();    
    warmer06Led->turnOnLED();    
    basketLeftLed->turnOnLED();    
    basketCenterLed->turnOnLED();    
    basketRightLed->turnOnLED();
}

