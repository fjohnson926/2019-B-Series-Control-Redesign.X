/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_usart_os.h>
#include <hal_adc_os.h>

#define EDBG_COM_BUFFER_SIZE 16

extern struct usart_os_descriptor EDBG_COM;
extern uint8_t                    EDBG_COM_buffer[];

extern struct i2c_m_os_desc I2C_0;

extern struct adc_os_descriptor ADC_0;

void EDBG_COM_PORT_init(void);
void EDBG_COM_CLOCK_init(void);
void EDBG_COM_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
