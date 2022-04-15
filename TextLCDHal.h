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

#ifndef _XBYYLINECHARACTERLCDHAL_H    /* Guard against multiple inclusion */
#define _XBYYLINECHARACTERLCDHAL_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include "rtos_port.h"
#include "hal_gpio.h"
#include "osDelay.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define LCD_HAL_WAIT_MS(time) osSupportedDelayMs(time)
#define LCD_HAL_WAIT_US(time) osSupportedDelayUs(time)

#define SET_PIN_FOR_GPIO_OUTPUT(ucPin) gpio_set_pin_direction(ucPin, GPIO_DIRECTION_OUT); gpio_set_pin_function(ucPin, GPIO_PIN_FUNCTION_OFF);
#define SET_PIN_FOR_GPIO_INPUT(ucPin) gpio_set_pin_direction(ucPin, GPIO_DIRECTION_IN); gpio_set_pin_function(ucPin, GPIO_PIN_FUNCTION_OFF); gpio_set_pin_pull_mode(ucPin,GPIO_PULL_OFF);
#define LCD_SET_GPIO_HIGH(ucPin) gpio_set_pin_level(ucPin,1)
#define LCD_SET_GPIO_LOW(ucPin) gpio_set_pin_level(ucPin,0)
#define LCD_SET_GPIO_PIN(ucPin, level) gpio_set_pin_level(ucPin,level)
#define LCD_READ_GPIO_PIN(ucPin) gpio_get_pin_level(ucPin)
    
//define type for gpio pins    
typedef uint8_t uCPin;

#ifdef __cplusplus
}
#endif

#endif /* _XBYYLINECHARACTERLCDHAL_H */

/* *****************************************************************************
 End of File
 */
