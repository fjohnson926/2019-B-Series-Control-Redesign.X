/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Electrolux

  @File Name
    Arrays.h

  @Summary
    Contains several functions needed for arrays. All functions are thread safe, so they shouldn't cause any problems with the operating systems

  @Description
    Needed this to initialize arrays without calling any library functions. Library functions are hit or miss when it comes to performing thread safe operations 
 */
/* ************************************************************************** */

#ifndef _ARRAYS_H    
#define _ARRAYS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "stdint.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
uint16_t getHighestFrequencyValue(uint16_t * array, int size);    
double averageDouble(double * data, int numberOfValues);
void copyDoubleArray(double * destination, double * source, int numberOfValues);
uint16_t averageU16(uint16_t * data, int numberOfValues);
void initializeDoubleArray(double * data, double value, int numberOfValues);
void initializeIntArray(int * data, int value, int numberOfValues);
void initializeU16Array(uint16_t * data, uint16_t value, int numberOfValues);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _ARRAYS_H */

/* *****************************************************************************
 End of File
 */
