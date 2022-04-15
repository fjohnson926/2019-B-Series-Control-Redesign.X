/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include "gm_SystemPinMap.h"
#include "hal_i2c_m_os.h"
#include "tcc_lite.h"
#include "pwm.h"
#include "hal_calendar.h"
#include "Calendar.h"

/* The priority of the peripheral should be between the low and high interrupt priority set by chosen RTOS,
 * Otherwise, some of the RTOS APIs may fail to work inside interrupts
 * In case of FreeRTOS,the Lowest Interrupt priority is set by configLIBRARY_LOWEST_INTERRUPT_PRIORITY and
 * Maximum interrupt priority by configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, So Interrupt Priority of the peripheral
 * should be between configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY and configLIBRARY_LOWEST_INTERRUPT_PRIORITY
 */
#define PERIPHERAL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1)

struct usart_os_descriptor EDBG_COM;
uint8_t                    EDBG_COM_buffer[EDBG_COM_BUFFER_SIZE];

struct i2c_m_os_desc I2C_0;



void EDBG_COM_PORT_init(void)
{

	//gpio_set_pin_function(UART_DEBUG_TX, PINMUX_PB25D_SERCOM2_PAD0);

	//gpio_set_pin_function(UART_DEBUG_RX, PINMUX_PB24D_SERCOM2_PAD1);
}

void EDBG_COM_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM2_bit(MCLK);
}

void EDBG_COM_init(void)
{

	EDBG_COM_CLOCK_init();
	uint32_t irq = SERCOM2_0_IRQn;
	for (uint32_t i = 0; i < 4; i++) {
		NVIC_SetPriority((IRQn_Type)irq, PERIPHERAL_INTERRUPT_PRIORITY);
		irq++;
	}
	usart_os_init(&EDBG_COM, SERCOM2, EDBG_COM_buffer, EDBG_COM_BUFFER_SIZE, (void *)NULL);
	usart_os_enable(&EDBG_COM);
	EDBG_COM_PORT_init();
}

void I2C_0_PORT_init(void)
{
#if 0
	gpio_set_pin_pull_mode(TOUCH_SCL_PIN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(TOUCH_SCL_PIN, PINMUX_PA23D_SERCOM5_PAD0);

	gpio_set_pin_pull_mode(TOUCH_SDA_PIN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(TOUCH_SDA_PIN, PINMUX_PA22D_SERCOM5_PAD1);
#endif
    
    //SDA Config
    gpio_set_pin_direction(PB20_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB20_PIN, GPIO_PIN_FUNCTION_OFF);
    
    //SCL Config
    gpio_set_pin_direction(PB21_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB21_PIN, GPIO_PIN_FUNCTION_OFF);
}

void I2C_0_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_SLOW, CONF_GCLK_SERCOM5_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM5_bit(MCLK);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	uint32_t irq = SERCOM5_0_IRQn;
	for (uint32_t i = 0; i < 4; i++) {
		NVIC_SetPriority((IRQn_Type)irq, PERIPHERAL_INTERRUPT_PRIORITY);
		irq++;
	}
	i2c_m_os_init(&I2C_0, SERCOM5);
	i2c_m_os_enable(&I2C_0);
	I2C_0_PORT_init();
}

/* The priority of the peripheral should be between the low and high interrupt priority set by chosen RTOS,
 * Otherwise, some of the RTOS APIs may fail to work inside interrupts
 * In case of FreeRTOS,the Lowest Interrupt priority is set by configLIBRARY_LOWEST_INTERRUPT_PRIORITY and
 * Maximum interrupt priority by configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, So Interrupt Priority of the peripheral
 * should be between configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY and configLIBRARY_LOWEST_INTERRUPT_PRIORITY
 */
#define PERIPHERAL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1)

/* The channel amount for ADC */
#define ADC_0_CH_AMOUNT 3

/* The buffer size for ADC */
#define ADC_0_BUFFER_SIZE 16

/* The maximal channel number of enabled channels */
#define ADC_0_CH_MAX 4

struct adc_os_descriptor         ADC_0;
struct adc_os_channel_descriptor ADC_0_ch[ADC_0_CH_AMOUNT];

static uint8_t ADC_0_buffer[ADC_0_BUFFER_SIZE];
static uint8_t ADC_0_Ch_1_buffer[ADC_0_BUFFER_SIZE];
static uint8_t ADC_0_Ch_2_buffer[ADC_0_BUFFER_SIZE];
static uint8_t ADC_0_map[ADC_0_CH_MAX + 1];

/**
 * \brief ADC initialization function
 *
 * Enables ADC peripheral, clocks and initializes ADC driver
 */
static void ADC_0_init(void)
{
	hri_mclk_set_APBDMASK_ADC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC0_GCLK_ID, CONF_GCLK_ADC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	uint32_t irq = ADC0_0_IRQn;
	for (uint32_t i = 0; i < 2; i++) {
		NVIC_SetPriority((IRQn_Type)irq, PERIPHERAL_INTERRUPT_PRIORITY);
		irq++;
	}
	adc_os_init(&ADC_0, ADC0, ADC_0_map, ADC_0_CH_MAX, ADC_0_CH_AMOUNT, &ADC_0_ch[0]);
	adc_os_register_channel_buffer(&ADC_0, 0, ADC_0_buffer, ADC_0_BUFFER_SIZE);
    adc_os_register_channel_buffer(&ADC_0, 1, ADC_0_Ch_1_buffer, ADC_0_BUFFER_SIZE);
    adc_os_register_channel_buffer(&ADC_0, 2, ADC_0_Ch_2_buffer, ADC_0_BUFFER_SIZE);

	// Disable digital pin circuitry
	gpio_set_pin_direction(WATER_LEVEL_DETECT_PIN, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(WATER_LEVEL_DETECT_PIN, PINMUX_PB00B_ADC0_AIN12);
    
  	// Disable digital pin circuitry
	gpio_set_pin_direction(WATER_LEVEL_2_PIN, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(WATER_LEVEL_2_PIN, PINMUX_PB09B_ADC0_AIN3);


	// Disable digital pin circuitry
	gpio_set_pin_direction(WATER_TEMP_SENSOR_PIN, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(WATER_TEMP_SENSOR_PIN, PINMUX_PB01B_ADC0_AIN13);
    
    //0x03 = 0x3 INTVCC1 VDDANA. pg 1641
    adc_os_set_reference(&ADC_0, 0x03);
    
}

void PWM_0_CLOCK_init(void)
{
	hri_mclk_set_APBBMASK_TCC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TCC0_GCLK_ID, CONF_GCLK_TCC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}


void PWM_0_PORT_init(void)
{    
    gpio_set_pin_direction(TANK_GROUND_SIGNAL, GPIO_DIRECTION_OUT);
    gpio_set_pin_function(TANK_GROUND_SIGNAL, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_function(TANK_GROUND_SIGNAL, PINMUX_PB31G_TCC0_WO7);    
}

#define COMPARE_CHANNEL 1
void pwm_1khz_example()
{
	tcc_disable(TCC0);
    tcc_set_period_value(TCC0, 0x217C);
	tcc_set_compare_value(TCC0, 0x10BE, COMPARE_CHANNEL);
	tcc_enable(TCC0);  
}

struct calendar_descriptor CALENDAR_0;

void CALENDAR_0_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_RTC_bit(MCLK);
}

void CALENDAR_0_init(void)
{
	CALENDAR_0_CLOCK_init();
    
    struct calendar_date_time date_time;    

	calendar_init(&CALENDAR_0, RTC);
}

void INIT_DATE_TIME(void)
{
    struct calendar_date_time date_time;
    
    calendar_get_date_time(&CALENDAR_0, &date_time);
    
    if(date_time.date.year < 2019)
    {
        date_time.date.month = 8;
        date_time.date.year = 2020;
        date_time.date.day = 05;
    
        date_time.time.hour = 12;
        date_time.time.min = 00;
    
        calendar_set_time(&CALENDAR_0 , &date_time.time);
        calendar_set_date(&CALENDAR_0 , &date_time.date);  
    }
}

//These GPIO are used for basket position switches on the Tea Brewers
void reed_switch_gpio_init(void)
{
    gpio_set_pin_direction(REED0_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(REED0_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(REED0_PIN,GPIO_PULL_OFF);
    
    gpio_set_pin_direction(REED1_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(REED1_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(REED1_PIN,GPIO_PULL_OFF);
    
    gpio_set_pin_direction(REED2_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(REED2_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(REED2_PIN,GPIO_PULL_OFF);
    
    gpio_set_pin_direction(REED3_PIN, GPIO_DIRECTION_IN); 
    gpio_set_pin_function(REED3_PIN, GPIO_PIN_FUNCTION_OFF); 
    gpio_set_pin_pull_mode(REED3_PIN,GPIO_PULL_OFF);
}

void system_init(void)
{
	init_mcu();
    
    ADC_0_init();
    
    PWM_0_CLOCK_init();

	PWM_0_PORT_init();

	PWM_0_init();
    
    pwm_1khz_example();   
    
    CALENDAR_0_init();    
    
    calendar_enable(&CALENDAR_0);
    
    INIT_DATE_TIME();
    
    reed_switch_gpio_init();    
}
