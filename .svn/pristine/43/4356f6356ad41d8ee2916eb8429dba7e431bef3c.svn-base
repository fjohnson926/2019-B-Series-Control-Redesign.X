/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Electrolux Corporation

  @File Name
    CapsenseControllerHal.h

  @Summary
    User modifiable file for the capsense controller

  @Description
 In theory, this file should be used for any modifications dependent on the platform and application. The CapsenseController.cpp and CapsenseController.h files
 * Shouldn't be modified on a regular basis, especially for a specific application.
 */
/* ************************************************************************** */

#ifndef _CAPSENSEHAL_H    /* Guard against multiple inclusion */
#define _CAPSENSEHAL_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "stddef.h"
#include "stdint.h"
#include "hal_i2c_m_os.h"
#include "driver_init.h"
#include "SlowSoftI2CMaster.h"
#include "gm_SystemPinMap.h"
#include "Arrays.h"

#define TOUCH_TASK_STACK_SIZE (600 / sizeof(portSTACK_TYPE))
#define TOUCH_TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define TOUCH_TASK_LOOP_RATE_MS 50

#define MAX_SETUP_ATTEMPTS 3

const uint8_t configFile[128] = 
{
    0xFFu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x40u, 0x40u, 0x40u, 0x40u,
    0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
    0x80u, 0x7Fu, 0x7Fu, 0x7Fu, 0x03u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x80u,
    0x05u, 0x00u, 0x00u, 0x02u, 0x00u, 0x02u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Eu, 0x1Eu, 0x00u,
    0x00u, 0x1Eu, 0x1Eu, 0x00u, 0x00u, 0x00u, 0x01u, 0x01u,
    0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x03u, 0x01u, 0x40u,
    0x00u, 0x37u, 0x06u, 0x00u, 0x00u, 0x0Au, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x2Cu, 0xB7u
};

#define NUMBER_OF_TOUCH_CONTROLLERS 1
#define NUMBER_OF_STORED_TOUCHES 3
#define IDLE_NO_TOUCH_TESTS 100
#define IDLE_TOUCHES_FAILED 2

//From datasheet 001-91082_CY8CMBR3xxx_CapSense_Express_Controllers_Registers_TRM.pdf. These are bits sent from register 0xAA, the touch status register
#define CS00_BIT 0
#define CS01_BIT 1
#define CS02_BIT 2
#define CS03_BIT 3
#define CS04_BIT 4
#define CS05_BIT 5
#define CS06_BIT 6
#define CS07_BIT 7
#define CS08_BIT 8
#define CS09_BIT 9
#define CS10_BIT 10
#define CS11_BIT 11
#define CS12_BIT 12
#define CS13_BIT 13
#define CS14_BIT 14
#define CS15_BIT 15

#define TOUCH_NONE 0
#define TOUCH_POWER         (1<<CS04_BIT)
#define TOUCH_RIGHT_BREW    (1<<CS11_BIT)
#define TOUCH_WARM_1        (1<<CS05_BIT)
#define TOUCH_WARM_2        (1<<CS12_BIT)
#define TOUCH_WARM_3        (1<<CS07_BIT)
#define TOUCH_WARM_4        (1<<CS10_BIT)
#define TOUCH_WARM_5        (1<<CS08_BIT)
#define TOUCH_WARM_6        (1<<CS09_BIT)
#define TOUCH_LEFT_SINGLE_BREW     (1<<CS06_BIT)
#define TOUCH_NEXT          (1<<CS01_BIT)
#define TOUCH_PREVIOUS      (1<<CS03_BIT)
#define TOUCH_CANCEL        (1<<CS02_BIT)
#define TOUCH_ACCEPT        (1<<CS00_BIT)


#define HOLD_REGISTERED_TIME_MS 5000
#define MIN_BUTTON_TIME_MS 100

#define TOUCH_HAL_SLEEP_MS(timeMs) os_sleep(timeMs/portTICK_PERIOD_MS)

#define HW 0
#define SW 1
#define USE_THIS SW
#if USE_THIS == HW 
#define TOUCH_HAL_INITIALIZE_I2C_PARMS() struct io_descriptor *io; uint16_t data
#define TOUCH_HAL_I2C_INITIALIZE_FUNCTIONS() struct io_descriptor *io; uint16_t data; i2c_m_os_get_io(&I2C_0, &io); i2c_m_os_set_slaveaddr(&I2C_0, 0x37, I2C_M_SEVEN)
#define TOUCH_HAL_IO_WRITE(outputBuffer_p, size) I2C_0.io.write(&I2C_0.io, outputBuffer_p, size)  
#define TOUCH_HAL_IO_READ(inputBuffer_p, size) I2C_0.io.read(&I2C_0.io, inputBuffer_p, size);
#define TOUCH_HAL_SET_SLAVE_ADDRESS(device, slaveAddress, width) i2c_m_os_set_slaveaddr(&device, slaveAddress, width)
#elif USE_THIS == SW
#define I2C_7BITADDR 0x37
#define TOUCH_HAL_INITIALIZE_I2C_PARMS() SlowSoftI2CMaster si = SlowSoftI2CMaster(PB20_PIN, PB21_PIN, false) 
#define TOUCH_HAL_I2C_INITIALIZE_FUNCTIONS() ;
#define TOUCH_HAL_IO_WRITE(outputBuffer_p, size) si.i2c_start((I2C_7BITADDR<<1)|I2C_WRITE); si.i2c_write_multi_byte(outputBuffer_p, size); si.i2c_stop()
#define TOUCH_HAL_IO_READ(inputBuffer_p, size) si.i2c_start((I2C_7BITADDR<<1)|I2C_READ); si.i2c_read_multi_byte(inputBuffer_p, size); si.i2c_stop();
#define TOUCH_HAL_SET_SLAVE_ADDRESS(device, slaveAddress, width) ;
#else
#error Invalid Parm
#endif
 




/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _CAPSENSEHAL_H */

/* *****************************************************************************
 End of File
 */
