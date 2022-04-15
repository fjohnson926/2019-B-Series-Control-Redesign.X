#include "osDelay.h"
#include "rtos_port.h"

void osSupportedDelayMs(uint32_t delayMs)
{
    if(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
    {
        os_sleep(delayMs/portTICK_PERIOD_MS);
    }
    else
    {
        delay_ms(delayMs);
    }
}

void osSupportedDelayUs(uint32_t delayUs)
{
    if(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING && delayUs > 26)
    {
        os_sleep(1/portTICK_PERIOD_MS);
    }
    else
    {
        delay_us(delayUs);
    }
}

