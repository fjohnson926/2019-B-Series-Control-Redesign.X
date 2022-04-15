/* 
 * File:   XByYLineCharacterLCD.h
 * Author: JTurner
 *
 * Created on April 16, 2019, 11:06 AM
 */

#ifndef XBYYLINECHARACTERLCD_H
#define	XBYYLINECHARACTERLCD_H

#include "TextLCDHal.h"

#define BUSY_FLAG_MAX_WAIT_TIME_MS 5


typedef struct EIGHTPINDATAMODE
{
    uCPin RS_PIN;
    uCPin RW_PIN;
    uCPin E_PIN;
    uCPin D0;
    uCPin D1;
    uCPin D2;
    uCPin D3;
    uCPin D4;
    uCPin D5;
    uCPin D6;
    uCPin D7;        
} EightDataModePinOut_t;

class TextLCD {
public:
    void initialize(EightDataModePinOut_t Pins);
    TextLCD();   
    void cmdLvlSet8BitOperation();
    void cmdLvlTurnCursorOn();
    void cmdLvlWriteInstructionRegister(uint8_t instruction, bool checkBusyFlag);
    void cmdLvlWriteDataRegister(uint8_t data, bool checkBusyFlag);
    void cmdLvlClearDisplayReturnHome();
    void cmdLvlReturnHome();
    void cmdLvlChangeEntryMode(bool shiftRight=true, bool shiftDisplay=false);
    void cmdLvlDisplayOnOffControl(bool entireDisplayOn, bool turnCursorOn, bool blinkCursorOn);
    void cmdLvlSetCGAddress(uint8_t address);
    void cmdLvlSetCursorAddress(uint8_t address);
    void cmdLvlCursorDisplayShift(bool shiftDisplay, bool shiftLeft);
    uint8_t cmdLvlGetAddressCounter();
    void cmdLvlWriteCharacter(uint8_t data);
    void cmdLvlWriteCharacters(const char * data, int numchars);
    uint8_t cmdLvlReadDataLines();
    void userLvlResetDisplay();
    void userLvlTurnCursorOn();
    void usrLvlLCDinitialize();
    
private:
    EightDataModePinOut_t myLCDPins;
    uint8_t displayAddress;
    
    void pinLvlSetRSLow();
    void pinLvlSetRSHigh();
    void pinLvlSetRWLow();
    void pinLvlSetRWHigh();
    void pinLvlSetELow();
    void pinLvlSetEHigh();
    void pinLvlSetDataLines(uint8_t data);
    uint8_t pinLvlReadDataLines();
    void pinLvlPollBusyFlag();
    

    
    
};
#endif	/* XBYYLINECHARACTERLCD_H */

