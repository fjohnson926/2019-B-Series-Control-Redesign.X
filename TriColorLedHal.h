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

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


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

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
