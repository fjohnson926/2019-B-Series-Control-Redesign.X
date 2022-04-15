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

#ifndef _SOFTSPIHAL_H    /* Guard against multiple inclusion */
#define _SOFTSPIHAL_H

#include "stdint.h"
#include "hal_gpio.h"
#include "rtos_port.h"
#include "osDelay.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    typedef uint8_t uCPin_t;
    #define CS_ASSERT_DEFAULT_US 10
    #define DELAY_US(timeUs) osSupportedDelayUs(timeUs) //os_sleep(timeMs/portTICK_PERIOD_MS);
    #define DIGITAL_WRITE(ucPin, pinLevel) gpio_set_pin_level(ucPin,pinLevel) 
    #define DIGITAL_READ(ucPin) gpio_get_pin_level(ucPin)

    #define SET_PIN_FOR_GPIO_OUTPUT(ucPin) gpio_set_pin_direction(ucPin, GPIO_DIRECTION_OUT); gpio_set_pin_function(ucPin, GPIO_PIN_FUNCTION_OFF);
    #define SET_PIN_FOR_GPIO_INPUT(ucPin) gpio_set_pin_direction(ucPin, GPIO_DIRECTION_IN); gpio_set_pin_function(ucPin, GPIO_PIN_FUNCTION_OFF); gpio_set_pin_pull_mode(ucPin,GPIO_PULL_OFF);
    #define SET_GPIO_HIGH(ucPin) gpio_set_pin_level(ucPin,1)
    #define SET_GPIO_LOW(ucPin) gpio_set_pin_level(ucPin,0)
    
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SOFTSPIHAL_H */

/* *****************************************************************************
 End of File
 */
