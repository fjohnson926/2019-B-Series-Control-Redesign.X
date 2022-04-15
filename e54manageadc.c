#include "atmel_start.h"
#include "TankManager.h"
#include "e54manageadc.h"
#include "temperatureCircuit.h"
#include "math.h"
#include "WtchDog.h"

TaskHandle_t      xCreatedAdcConvTask;

#define SetTmpAdcIn()   adc_os_set_inputs(&ADC_0, ADC_TEMP_INPUT_VAL, ADC_GRND_INPUT_VAL, OS_ADC_CHANNEL)
#define SetLev1AdcIn()  adc_os_set_inputs(&ADC_0, ADC_LEV1_INPUT_VAL, ADC_GRND_INPUT_VAL, OS_ADC_CHANNEL)
#define SetLev2AdcIn()  adc_os_set_inputs(&ADC_0, ADC_LEV2_INPUT_VAL, ADC_GRND_INPUT_VAL, OS_ADC_CHANNEL)
#define SIMULT_COUNTS 2

void AdcConversionTask(void *p) {
    AdcTaskState_t  AcdConvStat = 0;
    
    uint8_t         SmplNum     = 0;
    uint8_t         WaitingSmpl = 0;
    uint8_t         WaitingTime = 0;
    uint8_t         adc_values[DEFAULT_ADC_SMPLE_TO_WAIT];
    uint16_t        rawValue = 0;
    uint8_t count = 0;
    (void)p;
    adc_os_enable_channel(&ADC_0, OS_ADC_CHANNEL);
    
    AcdConvStat = ADC_STATE_GET_TEMP;
    WaitingSmpl = DEFAULT_ADC_SMPLE_TO_WAIT;
    WaitingTime = DEFAULT_ADC_SLEEP_TIME_MS;
    SetTmpAdcIn();
     
    while(1){
        WDogSetFlg(TASK_ADC_CONVERS);
        
        SmplNum = ringbuffer_num(&ADC_0.descr_ch->convert);
        if(SmplNum >= WaitingSmpl){
            switch(AcdConvStat){
                case ADC_STATE_GET_TEMP:{
                    adc_os_read_channel(&ADC_0, OS_ADC_CHANNEL, adc_values, WaitingSmpl);            
                    rawValue = adc_values[1] << 8;
                    rawValue += adc_values[0];
                    saveTempAsADCValue__c(0, rawValue);
                    count++;
                    if(count > SIMULT_COUNTS)
                    {
                        AcdConvStat = ADC_STATE_GET_LEV1;
                        WaitingSmpl = DEFAULT_ADC_SMPLE_TO_WAIT;
                        WaitingTime = DEFAULT_ADC_SLEEP_TIME_MS;
                        SetLev1AdcIn();
                        count = 0;
                    }
                }break;
                
                case ADC_STATE_GET_LEV1:{                     
                    adc_os_read_channel(&ADC_0, OS_ADC_CHANNEL, adc_values, WaitingSmpl);
                    rawValue = adc_values[1] << 8;
                    rawValue += adc_values[0];
                    saveWaterLevelAsADCValue__c(0, WATER_LEVEL_HIGH_PROBE_INDEX, rawValue);
                    count++;
                    if(count > SIMULT_COUNTS)
                    {
                        AcdConvStat = ADC_STATE_GET_LEV2;
                        WaitingSmpl = DEFAULT_ADC_SMPLE_TO_WAIT;
                        WaitingTime = DEFAULT_ADC_SLEEP_TIME_MS;
                        SetLev2AdcIn();
                        count = 0;
                    }
                }break;
                
                case ADC_STATE_GET_LEV2:{                     
                    adc_os_read_channel(&ADC_0, OS_ADC_CHANNEL, adc_values, WaitingSmpl);
                    rawValue = adc_values[1] << 8;
                    rawValue += adc_values[0];
                    saveWaterLevelAsADCValue__c(0, WATER_LEVEL_LOW_PROBE_INDEX, rawValue);
                    count++;
                    if(count > SIMULT_COUNTS)
                    {
                        AcdConvStat = ADC_STATE_GET_TEMP;
                        WaitingSmpl = DEFAULT_ADC_SMPLE_TO_WAIT;
                        WaitingTime = DEFAULT_ADC_SLEEP_TIME_MS;
                        SetTmpAdcIn();
                        count = 0;
                    }
                }break;
            }
            os_sleep(WaitingTime/portTICK_PERIOD_MS);
        }
        else{
            adc_os_start_conversion(&ADC_0);
            os_sleep(MIN_ADC_SAMPLING_TIME_MS/portTICK_PERIOD_MS);
        }
    }
}

void launchADCStartConversionTasks(void)
{
    if (xTaskCreate(
        AdcConversionTask, "AdcConv", TASK_START_CONVERSION_SIZE, NULL, TASK_START_CONVERSION_STACK_PRIORITY, xCreatedAdcConvTask)
    != pdPASS) 
        {
            #if __DEBUG
                asm("bkpt");            
            #endif      
        }
}

void ADC0_RESRDY_Handler(void)
{
    struct adc_os_descriptor *descr = &ADC_0;
    struct _adc_async_device *device;
    device = &descr->device;
    uint16_t result = ADC0->RESULT.reg;  
    device->adc_async_ch_cb.convert_done(device, OS_ADC_CHANNEL, result);
}

void ADC0_OTHER_Hndler(void)
{
    

}
