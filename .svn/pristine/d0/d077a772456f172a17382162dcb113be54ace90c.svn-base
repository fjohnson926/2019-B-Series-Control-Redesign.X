/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _TEMPERATURE_CIRCUIT_H    /* Guard against multiple inclusion */
#define _TEMPERATURE_CIRCUIT_H

#include "atmel_start.h"
#include "math.h"

#define MAX_COUNT 4095.00
#define MAX_VOLTAGE 3.285
#define VOLTS_PER_COUNT MAX_VOLTAGE/MAX_COUNT
#define NO_PROBE_CONN_OFFSET_COUNTS -100
#define ROUT 2430
//After a range of tests, can't seem to find why the resistance value is slightly off.
#define ROFFSET 0
#define MAX_RESISTANCE 15900
#define MIN_RESISTANCE 325

#define COEFFA 0.001215417
#define COEFFB 0.000250188
#define COEFFC 0.000000012



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    double get61128TemperatureC(double resistance);
    double getResistanceFromTempCircuit(double voltage);
    double getResistanceFromCount(int count);
    double getTemperatureF(double tempC);
    double getVoltage(uint16_t count);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _TEMPERATURE_CIRCUIT_H */

/* *****************************************************************************
 End of File
 */
