/* 
 * File:   XByYLineCharacterLCD.cpp
 * Author: JTurner
 * 
 * Created on April 16, 2019, 11:06 AM
 */

#include "TextLCD.h"




TextLCD::TextLCD()
{
    
}

void TextLCD::initialize(EightDataModePinOut_t Pins) 
{
 myLCDPins.E_PIN = Pins.E_PIN;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.E_PIN);
 myLCDPins.RS_PIN = Pins.RS_PIN;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.RS_PIN);
 myLCDPins.RW_PIN = Pins.RW_PIN;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.RW_PIN);
 myLCDPins.D0 = Pins.D0;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D0);
 myLCDPins.D1 = Pins.D1;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D1);
 myLCDPins.D2 = Pins.D2;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D2);
 myLCDPins.D3 = Pins.D3;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D3);
 myLCDPins.D4 = Pins.D4;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D4);
 myLCDPins.D5 = Pins.D5;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D5);
 myLCDPins.D6 = Pins.D6;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D6);
 myLCDPins.D7 = Pins.D7;
 SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D7);
 displayAddress = 0; 
}

void TextLCD::usrLvlLCDinitialize()
{
    pinLvlSetRSLow();
    pinLvlSetRWLow();
    cmdLvlWriteInstructionRegister(0x38, false);
    LCD_HAL_WAIT_MS(5);
    cmdLvlWriteInstructionRegister(0x0C, false);
    LCD_HAL_WAIT_MS(1);
    cmdLvlWriteInstructionRegister(0x06, false);
    cmdLvlWriteInstructionRegister(0x01, false);
}

#define STR_LCD_TIME            1
#define ACTION_LCD_DELAY_TIME   1

void TextLCD::cmdLvlWriteInstructionRegister(uint8_t instruction, bool checkBusyFlag)
{  
    if(checkBusyFlag)   {
        pinLvlPollBusyFlag();
    }
    CRITICAL_SECTION_ENTER()        
        pinLvlSetRSLow();
        pinLvlSetRWLow();
        pinLvlSetDataLines(instruction);
        
        pinLvlSetEHigh();
        pinLvlSetELow();
    CRITICAL_SECTION_LEAVE()
            
    if(instruction == 0x01){
        delay_us(1600); 
    }
}


void TextLCD::cmdLvlWriteDataRegister(uint8_t data, bool checkBusyFlag)
{  
    if(checkBusyFlag)   {
        pinLvlPollBusyFlag();
    }
    CRITICAL_SECTION_ENTER()
        pinLvlSetRSHigh();
        pinLvlSetRWLow();
        pinLvlSetDataLines(data); 
        
        pinLvlSetEHigh();
        pinLvlSetELow();
    CRITICAL_SECTION_LEAVE()
}

void TextLCD::userLvlResetDisplay()
{
    pinLvlSetRSLow();
    pinLvlSetRWLow();
    pinLvlSetDataLines(48);
    LCD_HAL_WAIT_MS(5);
    pinLvlSetDataLines(00);
    LCD_HAL_WAIT_MS(5);
    pinLvlSetRSHigh();
    pinLvlSetRWHigh();
}

void TextLCD::pinLvlSetRSLow()
{
    LCD_SET_GPIO_PIN(myLCDPins.RS_PIN,0);
}

void TextLCD::pinLvlSetRSHigh()
{
    LCD_SET_GPIO_PIN(myLCDPins.RS_PIN,1);
}

void TextLCD::pinLvlSetRWLow()
{
    LCD_SET_GPIO_LOW(myLCDPins.RW_PIN);
}

void TextLCD::pinLvlSetRWHigh()
{
    LCD_SET_GPIO_HIGH(myLCDPins.RW_PIN);
}

void TextLCD::pinLvlSetELow()
{
    LCD_SET_GPIO_LOW(myLCDPins.E_PIN);
}

void TextLCD::pinLvlSetEHigh()
{
    LCD_SET_GPIO_HIGH(myLCDPins.E_PIN);
}

void TextLCD::pinLvlSetDataLines(uint8_t data)
{    
    LCD_SET_GPIO_PIN(myLCDPins.D0, data & 0b00000001);
    LCD_SET_GPIO_PIN(myLCDPins.D1, data & 0b00000010);
    LCD_SET_GPIO_PIN(myLCDPins.D2, data & 0b00000100);
    LCD_SET_GPIO_PIN(myLCDPins.D3, data & 0b00001000);
    LCD_SET_GPIO_PIN(myLCDPins.D4, data & 0b00010000);
    LCD_SET_GPIO_PIN(myLCDPins.D5, data & 0b00100000);
    LCD_SET_GPIO_PIN(myLCDPins.D6, data & 0b01000000);
    LCD_SET_GPIO_PIN(myLCDPins.D7, data & 0b10000000);
}

void TextLCD::userLvlTurnCursorOn()
{ 
    cmdLvlDisplayOnOffControl(true,true,true);
}

void TextLCD::cmdLvlWriteCharacter(uint8_t data)
{
    cmdLvlWriteDataRegister(data, true);
}

void TextLCD::cmdLvlWriteCharacters(const char * data, int numchars)
{
    for(int i = 0; i < numchars; i++)
    {
        cmdLvlWriteCharacter(data[i]);
    }       
}

void TextLCD::pinLvlPollBusyFlag()
{
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D7);
    pinLvlSetRSLow();
    pinLvlSetRWHigh();
    pinLvlSetEHigh();
    int i = 0; 
    while(LCD_READ_GPIO_PIN(myLCDPins.D7) && i < BUSY_FLAG_MAX_WAIT_TIME_MS)
    {
        LCD_HAL_WAIT_US(5);
        i++;
    }
    pinLvlSetELow();
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D7);
    return;
}

uint8_t TextLCD::cmdLvlGetAddressCounter()
{    
    uint8_t address = 0;
    pinLvlSetRSLow();
    pinLvlSetRWHigh();
    pinLvlSetEHigh();
    LCD_HAL_WAIT_MS(1);
    address = pinLvlReadDataLines();    
    pinLvlSetELow();
    return address;
}

uint8_t TextLCD::pinLvlReadDataLines()
{
    uint8_t address = 0;
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D0);
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D1);
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D2);
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D3);
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D4);
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D5);
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D6);
    SET_PIN_FOR_GPIO_INPUT(myLCDPins.D7);
    if(LCD_READ_GPIO_PIN(myLCDPins.D0))
    {
        address |= 0x01;
    }
    if(LCD_READ_GPIO_PIN(myLCDPins.D1))
    {
        address |= 0x02;
    }
    if(LCD_READ_GPIO_PIN(myLCDPins.D2))
    {
        address |= 0x04;
    }
    if(LCD_READ_GPIO_PIN(myLCDPins.D3))
    {
        address |= 0x08;
    }
    if(LCD_READ_GPIO_PIN(myLCDPins.D4))
    {
        address |= 0x10;
    }
    if(LCD_READ_GPIO_PIN(myLCDPins.D5))
    {
        address |= 0x20;
    }
    if(LCD_READ_GPIO_PIN(myLCDPins.D6))
    {
        address |= 0x40;
    }
    if(LCD_READ_GPIO_PIN(myLCDPins.D7))
    {
        address |= 0x80;
    }
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D0);
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D1);
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D2);
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D3);
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D4);
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D5);
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D6);
    SET_PIN_FOR_GPIO_OUTPUT(myLCDPins.D7);
    
    return address;
}

uint8_t TextLCD::cmdLvlReadDataLines()
{
    pinLvlSetRSLow();
    pinLvlSetRWHigh();
    pinLvlSetEHigh();
    uint8_t address = 0;
    address = pinLvlReadDataLines();
    pinLvlSetELow();
    return address;
}

void TextLCD::cmdLvlClearDisplayReturnHome()
{
    cmdLvlWriteInstructionRegister(0x01,true);
}

void TextLCD::cmdLvlReturnHome()
{
    cmdLvlWriteInstructionRegister(0x02,true);
}

void TextLCD::cmdLvlChangeEntryMode(bool shiftRight, bool shiftDisplay)
{
    int command =  0b00000100;
    if(shiftRight)
    {
        command |= 0b00000010;
    }
    if(shiftDisplay)
    {
        command |= 0b00000001;
    }
    cmdLvlWriteInstructionRegister(command,true);
}

void TextLCD::cmdLvlDisplayOnOffControl(bool entireDisplayOn, bool turnCursorOn, bool blinkCursorOn)
{
    uint8_t command = 0x08;
    if(entireDisplayOn)
    {
       command |= 0x04;
    }
    if(turnCursorOn)
    {
       command |= 0x02;  
    }
    if(blinkCursorOn)
    {
       command |= 0x01; 
    }
}

void TextLCD::cmdLvlSetCGAddress(uint8_t address)
{
    if(address <= 0b111111)
    {
        cmdLvlWriteInstructionRegister(0b01000000 | address, true);
    }
}

//Appears to move the cursor to the address, but not the screen 
void TextLCD::cmdLvlSetCursorAddress(uint8_t address)
{
    if(address <= 0b1111111)
    {
        cmdLvlWriteInstructionRegister(0b10000000 | address, true);
    }
}

void TextLCD::cmdLvlCursorDisplayShift(bool shiftDisplay, bool shiftLeft)
{
    uint8_t command = 0b00010000;
    if(shiftDisplay)
    {
        command |= 0b00001000;
    }
    if (shiftLeft)
    {
        command |= 0b00000100;
    }
    cmdLvlWriteInstructionRegister(command, true);
    displayAddress = cmdLvlReadDataLines();
}