/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Electrolux

  @File Name
    LedHal.h

  @Summary
    Since the LED class doesn't call any direct driver functions from it, driver calls are placed in here

  @Description
    
 */
/* ************************************************************************** */

#ifndef _LEDHAL_H    /* Guard against multiple inclusion */
#define _LEDHAL_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "hal_gpio.h"
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

//define type for gpio pins    
typedef uint8_t uCPin;

#define SET_PIN_FOR_GPIO_OUTPUT(ucPin) gpio_set_pin_direction(ucPin, GPIO_DIRECTION_OUT); gpio_set_pin_function(ucPin, GPIO_PIN_FUNCTION_OFF);
#define SET_PIN_FOR_GPIO_INPUT(ucPin) gpio_set_pin_direction(ucPin, GPIO_DIRECTION_IN); gpio_set_pin_function(ucPin, GPIO_PIN_FUNCTION_OFF); gpio_set_pin_pull_mode(ucPin,GPIO_PULL_OFF);
#define LED_SET_GPIO_HIGH(ucPin) gpio_set_pin_level(ucPin,1)
#define LED_SET_GPIO_LOW(ucPin) gpio_set_pin_level(ucPin,0)
#define LED_SET_GPIO_PIN(ucPin, level) gpio_set_pin_level(ucPin,level)
#define LED_TOGGLE_GPIO_PIN(ucPin) gpio_toggle_pin_level(ucPin)

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _LEDHAL_H */

/* *****************************************************************************
 End of File
 */
