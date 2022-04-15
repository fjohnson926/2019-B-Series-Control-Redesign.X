/* 
 * File:   SoftSPI.h
 * Author: JTurner
 *
 * Created on June 26, 2019, 1:24 PM
 */

#ifndef SOFTSPI_H
#define	SOFTSPI_H

#include "SoftSPIHal.h"



class SoftSPI {
public:
    SoftSPI(uCPin_t MOSIPin, uCPin_t MISOPin, uCPin_t CSPin, uCPin_t CLKPin);
    void transferData(uint8_t * outData, uint8_t * inData, int size);
    int delayCSAssertUs;
private:
    uCPin_t _MOSIPin;
    uCPin_t _MISOPin;
    uCPin_t _CSPin;
    uCPin_t _CLKPin;
};

#endif	/* SOFTSPI_H */

