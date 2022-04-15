#include "temperatureCircuit.h"

double get61128TemperatureC(double resistance)
{
    volatile double temperatureC = 0;
    //Can find these values from "5k THERM R VS T CALCULATOR.xls" 
    temperatureC = 1 / ( COEFFA + COEFFB*log(resistance) + COEFFC*( pow( log(resistance),3) ) ) -273.15 ;
    return temperatureC;
}

double getResistanceFromTempCircuit(double voltage)
{
    volatile double resistance = 0;
    resistance = ROUT*(MAX_VOLTAGE/voltage-1);
    if(resistance > MIN_RESISTANCE && resistance < MAX_RESISTANCE)
    {
        return resistance;
    }
    if(resistance < MIN_RESISTANCE)
    {
        return MIN_RESISTANCE;
    }
    if(resistance > MAX_RESISTANCE)
    {
        return MAX_RESISTANCE;
    }
}

double getResistanceFromCount(int count)
{    
    double currentCount = count;
    double resistance = 0;
    resistance = (MAX_COUNT/currentCount)*ROUT - ROUT;   
    return resistance;
}

double getTemperatureF(double tempC)
{
    double tempF = 0;
    tempF = ( tempC * 9/5) + 32;
    return tempF;
}

double getVoltage(uint16_t count)
{        
    double rawVoltage = count*MAX_VOLTAGE/MAX_COUNT;
    return rawVoltage;
}

