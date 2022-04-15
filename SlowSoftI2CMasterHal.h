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

#ifndef _SLOWSOFTI2CMASTERHAL_H    /* Guard against multiple inclusion */
#define _SLOWSOFTI2CMASTERHAL_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
#include "hal_gpio.h"

#define LOW 0
#define HIGH 1    
#define digitalWrite(ucPin, pinLevel) gpio_set_pin_level(ucPin,pinLevel) 
#define digitalRead(ucPin) gpio_get_pin_level(ucPin)
#define delayMicroseconds(timeUs) for(int i = 0; i < timeUs; i++) asm("nop")
#define noInterrupts() ;
#define interrupts() ;
#define OUTPUT GPIO_DIRECTION_OUT
#define INPUT_PULLUP GPIO_DIRECTION_IN
#define INPUT GPIO_DIRECTION_IN
#define pinMode(uCPin, type) gpio_set_pin_direction(uCPin,type)
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
