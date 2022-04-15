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

#ifndef _TANK_MANAGER_HAL_H    /* Guard against multiple inclusion */
#define _TANK_MANAGER_HAL_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "hal_gpio.h"
#include "Arrays.h"
#include "temperatureCircuit.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t uCPin;
typedef int water_timer_t;
#define MAX_WATER_TIMER_T_VALUE ( 4294967295 )
typedef double temperature_values_t;
typedef uint16_t adc_values_t;

#define NUMBER_OF_PRE_ERROR_TEMPS 50
#define NUMBER_OF_POST_ERROR_TEMPS 30

#define MAX_NUMBER_OF_TANKS 1
#define MAX_NUMBER_OF_PROBES 2

#define TANK_INDEX 0
#define WATER_LEVEL_HIGH_PROBE_INDEX 0
#define WATER_LEVEL_LOW_PROBE_INDEX 1
    
#define SET_PIN_FOR_GPIO_OUTPUT(ucPin) gpio_set_pin_direction(ucPin, GPIO_DIRECTION_OUT); gpio_set_pin_function(ucPin, GPIO_PIN_FUNCTION_OFF);
#define SET_PIN_FOR_GPIO_INPUT(ucPin) gpio_set_pin_direction(ucPin, GPIO_DIRECTION_IN); gpio_set_pin_function(ucPin, GPIO_PIN_FUNCTION_OFF); gpio_set_pin_pull_mode(ucPin,GPIO_PULL_OFF);
#define SET_GPIO_HIGH(ucPin) gpio_set_pin_level(ucPin,1)
#define SET_GPIO_LOW(ucPin) gpio_set_pin_level(ucPin,0)
#define SET_GPIO_PIN(ucPin, level) gpio_set_pin_level(ucPin,level)
#define GET_GPIO_PIN_LEVEL(ucPin) gpio_get_pin_level(ucPin)
#define GET_AVERAGE_ADC_VALUE(data, size) averageU16(data, size)
#define GET_AVERAGE_TEMP_VALUE(data, size) averageU16(data,size)
#define INITIALIZE_TEMPS(data, value, size) initializeU16Array(data, value, size)
#define INITIALIZE_WATER_LEVEL(data,value,size) initializeU16Array(data, value, size)
#define WATER_FILL_LEVEL_LOW 1500
#define TANK_TASK_RATE_MS 100
#define TIME_WATER_FILL_DELAY_MS 1000
#define TIME_CONT_FILL_AFTER_SEE_WATER_MS 0
#define TIME_TURN_OFF_HEATER_DELAY_MS 10000
#define TIME_WATER_OVERFLOW_DEFAULT_MS 300000
#define LOWER_TEMP_LIMIT 35.0
#define UPPER_TEMP_LIMIT 210.0
//Recommended to use an even number here
#define COLD_WATER_RESET_GOAL_TEMP_F 5
#define HEATING_NEXT_GOAL_TEMP_F 2
#define TIME_NO_HEAT_TIMEOUT_COUNT_MS 600000
#define TIME_INLET_CLOSED_START_HEAT_TIMEOUT_COUNT 120000

#define PULSE_HEAT_ENABLE false
#define PULSE_HEAT_PERIOD_MS 10000
#define PULSE_HEAT_DUTY_CYCLE 50
#define PULSE_HEAT_TEMP_BEFORE_SETPOINT_F 5
#define OUTER_HYSTERSIS_OFF_F 0
#define OUTER_HYSTERSIS_ON_F 4

#define TASK_FILL_VALVE_MANAGE_STACK_SIZE 350
#define TASK_HEAT_TEMP_MANAGE_STACK_SIZE 350
#define TASK_TANK_MANAGE_PRIORITY (tskIDLE_PRIORITY + 1)
#define TEMP_COUNTS_BEFORE_STABLE 50

#define MIN_TANK_SETPOINT_F 140
#define MAX_TANK_SETPOINT_F 205

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _TANK_MANAGER_HAL_H */

/* *****************************************************************************
 End of File
 */
