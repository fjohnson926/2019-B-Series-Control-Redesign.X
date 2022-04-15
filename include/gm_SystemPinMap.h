/* ************************************************************************** */
/** Descriptive File Name

  @Company
 Electrolux Professional

  @File Name
    gm_SystemPinMap.h

  @Summary
 Low level file containing system wide pin configurations

  @Description
 Can easily change pin mappings in this centralized file.
 */
/* ************************************************************************** */

#ifndef _GM_SYSTEMPINMAP_H    /* Guard against multiple inclusion */
#define _GM_SYSTEMPINMAP_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include <hal_gpio.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define E54_XPLAINED_PRO 0
#define D51_APP_BOARD 1
#define SYSTEM_IS D51_APP_BOARD
    
#if (SYSTEM_IS == D51_APP_BOARD)
//PORT A
#define DIL0_PIN               GPIO(GPIO_PORTA, 2)
#define REED3_PIN              GPIO(GPIO_PORTA, 3)
#define SPI_FLASH_DI_PIN       GPIO(GPIO_PORTA, 4)
#define SPI_FLASH_CLK_PIN      GPIO(GPIO_PORTA, 5)
#define SPI_FLASH_CS_PIN       GPIO(GPIO_PORTA, 6)
#define SPI_FLASH_DO_PIN       GPIO(GPIO_PORTA, 7)
#define PA08_PIN               GPIO(GPIO_PORTA, 8)
#define PA09_PIN               GPIO(GPIO_PORTA, 9)
#define PA10_PIN               GPIO(GPIO_PORTA, 10)
#define PA11_PIN               GPIO(GPIO_PORTA, 11)
#define WARMER4_PIN            GPIO(GPIO_PORTA, 12)
#define WARMER5_PIN            GPIO(GPIO_PORTA, 13)
#define DEBUGLED_PIN           GPIO(GPIO_PORTA, 14)
#define PA16_PIN               GPIO(GPIO_PORTA, 16)
#define PA17_PIN               GPIO(GPIO_PORTA, 17)   
#define PA18_PIN               GPIO(GPIO_PORTA, 18)
#define PA19_PIN               GPIO(GPIO_PORTA, 19)
#define DISP_VALVE_LEFT_PIN    GPIO(GPIO_PORTA, 20)
#define DISP_VALVE_RS_PIN      GPIO(GPIO_PORTA, 21)
#define HEAT_CONTROL_PIN       GPIO(GPIO_PORTA, 22)
#define INLET_CONTROL_PIN      GPIO(GPIO_PORTA, 23)
#define MODEL_BIT6_PIN         GPIO(GPIO_PORTA, 27)


//PORT B
#define WATER_TEMP_SENSOR_PIN  GPIO(GPIO_PORTB, 0)
#define WATER_LEVEL_DETECT_PIN GPIO(GPIO_PORTB, 1)
#define LOCK_PIN               GPIO(GPIO_PORTB, 2)
#define BATTERY_PIN            GPIO(GPIO_PORTB, 3)
#define REED2_PIN              GPIO(GPIO_PORTB, 4)
#define REED1_PIN              GPIO(GPIO_PORTB, 5)
#define REED0_PIN              GPIO(GPIO_PORTB, 6)
#define PB07_PIN               GPIO(GPIO_PORTB, 7)
#define PB08_PIN               GPIO(GPIO_PORTB, 8)
#define WATER_LEVEL_2_PIN      GPIO(GPIO_PORTB, 9)
#define PB10_PIN               GPIO(GPIO_PORTB, 10)
#define PB11_PIN               GPIO(GPIO_PORTB, 11)
#define PB12_PIN               GPIO(GPIO_PORTB, 12)
#define PB13_PIN               GPIO(GPIO_PORTB, 13)
#define PB14_PIN               GPIO(GPIO_PORTB, 14)
#define PB15_PIN               GPIO(GPIO_PORTB, 15)
#define PB16_PIN               GPIO(GPIO_PORTB, 16)
#define PB17_PIN               GPIO(GPIO_PORTB, 17)
#define PB18_PIN               GPIO(GPIO_PORTB, 18)
#define PB19_PIN               GPIO(GPIO_PORTB, 19)
#define PB20_PIN               GPIO(GPIO_PORTB, 20)
#define PB21_PIN               GPIO(GPIO_PORTB, 21)
#define PROG_LOCKOUT_SW_PIN    GPIO(GPIO_PORTB, 24)
#define PROG_BUTTON            GPIO(GPIO_PORTB, 25)
//Default UART Debug Port. Will have to look into the driver to be sure which pin is tx or rx.    
//#define UART_DEBUG_RX GPIO(GPIO_PORTB, 24)
//#define UART_DEBUG_TX GPIO(GPIO_PORTB, 25)
#define TANK_GROUND_SIGNAL    GPIO(GPIO_PORTB, 31)

//PORT C
#define SIDE_SELECT_PIN       GPIO(GPIO_PORTC, 0)
#define SUGAR1_PIN            GPIO(GPIO_PORTC, 1)
#define SUGAR0_PIN            GPIO(GPIO_PORTC, 2)
#define DIL1_PIN              GPIO(GPIO_PORTC, 3) // Don't have one...yet
#define PC06_PIN              GPIO(GPIO_PORTC, 6)
#define PC05_PIN              GPIO(GPIO_PORTC, 5)
#define PC07_PIN              GPIO(GPIO_PORTC, 7)
#define PC10_PIN              GPIO(GPIO_PORTC, 10)
#define PC11_PIN              GPIO(GPIO_PORTC, 11)
#define WARMER0_PIN           GPIO(GPIO_PORTC, 12)
#define WARMER1_PIN           GPIO(GPIO_PORTC, 13)
#define WARMER2_PIN           GPIO(GPIO_PORTC, 14)
#define WARMER3_PIN           GPIO(GPIO_PORTC, 15)
#define PC16_PIN              GPIO(GPIO_PORTC, 16)  
#define PC17_PIN              GPIO(GPIO_PORTC, 17)  
#define PC18_PIN              GPIO(GPIO_PORTC, 18)
#define PC19_PIN              GPIO(GPIO_PORTC, 19)
#define PC20_PIN              GPIO(GPIO_PORTC, 20)
#define PC21_PIN              GPIO(GPIO_PORTC, 21)
#define MODEL_BIT1_PIN        GPIO(GPIO_PORTC, 24)
#define MODEL_BIT2_PIN        GPIO(GPIO_PORTC, 25)
#define MODEL_BIT3_PIN        GPIO(GPIO_PORTC, 26)
#define MODEL_BIT4_PIN        GPIO(GPIO_PORTC, 27)
#define MODEL_BIT5_PIN        GPIO(GPIO_PORTC, 28)
//PORT D 
    
#else
#error Unknown system
#endif   
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _GM_SYSTEMPINMAP_H */

/* *****************************************************************************
 End of File
 */
