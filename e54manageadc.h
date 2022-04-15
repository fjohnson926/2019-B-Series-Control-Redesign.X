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

#ifndef _E54MANAGEADC_H    /* Guard against multiple inclusion */
#define _E54MANAGEADC_H

#define TASK_START_CONVERSION_SIZE (256 / sizeof(portSTACK_TYPE))
#define TASK_START_CONVERSION_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

//Potential causes of thread hanging on a a read operation: stack too small, or if going to default handler, no conversion complete handler defined
#define TASK_READ_CONVERSION_SIZE (800 / sizeof(portSTACK_TYPE))
#define TASK_READ_CONVERSION_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define MIN_ADC_SAMPLING_TIME_MS    ( 1 )
#define DEFAULT_ADC_SMPLE_TO_WAIT   ( 2 )
#define DEFAULT_ADC_SLEEP_TIME_MS   ( 100 )

#define OS_ADC_CHANNEL              ( 0 )
    
#define ADC_TEMP_INPUT_VAL          ( 0x0C ) // 0x0C = AIN12 ( datasheet, pg 1638 )
#define ADC_LEV1_INPUT_VAL          ( 0x0D ) // 0x0D = AIN13 ( datasheet, pg 1638 ) 
#define ADC_LEV2_INPUT_VAL          ( 0x03 ) // 0x03 = AIN03 ( datasheet, pg 1638 )
#define ADC_GRND_INPUT_VAL          ( 0x18 ) // 0x18 = GND,  ( datasheet, pg 1637 )
    
typedef enum{
    ADC_STATE_GET_TEMP  = 0,
    ADC_STATE_GET_LEV1  = 1,
    ADC_STATE_GET_LEV2  = 2,
}AdcTaskState_t;

void launchADCStartConversionTasks(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _E54MANAGEADC_H */

/* *****************************************************************************
 End of File
 */
