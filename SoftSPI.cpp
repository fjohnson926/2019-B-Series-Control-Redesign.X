/* 
 * File:   SoftSPI.cpp
 * Author: JTurner
 * 
 * Created on June 26, 2019, 1:24 PM
 */

#include "SoftSPI.h"

SoftSPI::SoftSPI(uCPin_t MOSIPin, uCPin_t MISOPin, uCPin_t CSPin, uCPin_t CLKPin)
{
    _MOSIPin = MOSIPin;
    _MISOPin = MISOPin;
    _CSPin = CSPin;
    _CLKPin = CLKPin;
    delayCSAssertUs = CS_ASSERT_DEFAULT_US;
    SET_PIN_FOR_GPIO_OUTPUT(_MOSIPin);
    SET_PIN_FOR_GPIO_INPUT(_MISOPin);
    SET_PIN_FOR_GPIO_OUTPUT(_CSPin);
    SET_PIN_FOR_GPIO_OUTPUT(_CLKPin);
    DIGITAL_WRITE(_MOSIPin, 1);
    DIGITAL_WRITE(_CSPin, 1);
    DIGITAL_WRITE(_CLKPin, 1);    
}

void SoftSPI::transferData(uint8_t * outData, uint8_t * inData, int size)
{
    if(!outData){
        return;
    }
    
    if(inData){
        for(int i = 0; i < size; i++){
            inData[i] = 0;
        }
    }
    
    DIGITAL_WRITE(_CLKPin,0);
    DIGITAL_WRITE(_CSPin,0);
    DELAY_US(delayCSAssertUs);
    
    for(int i = 0; i < size; i++)   {
        uint8_t currentByte = outData[i];
        CRITICAL_SECTION_ENTER()
            for(int j = 0; j < 8; j++)  {
                if(currentByte & 0x80)
                    DIGITAL_WRITE(_MOSIPin,1);
                else
                    DIGITAL_WRITE(_MOSIPin,0);

                DIGITAL_WRITE(_CLKPin,1);
                currentByte <<= 1;
                
                if(inData){                
                    if(DIGITAL_READ(_MISOPin))
                        inData[i] |= 1;
                    if(j != 7)
                        inData[i] <<=1;               
                }
                DIGITAL_WRITE(_CLKPin,0);
            }
        CRITICAL_SECTION_LEAVE() 
    }
    
    SET_GPIO_HIGH(_CSPin);
    DIGITAL_WRITE(_MOSIPin,1);
}

