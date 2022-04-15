/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "debugFunctions.h"

//Verify CPU clock speed by mapping the output of the clock to PB14.
//Be sure to set CONF_GCLK_GEN_0_OE found in config/hpl_gclk_config.h to 1 in order for this to work
void verifyCPUClockSpeed(void)
{
    gpio_set_pin_direction(PB14_PIN, GPIO_DIRECTION_OUT); 
    gpio_set_pin_function(PB14_PIN, PINMUX_PB14M_GCLK_IO0);
    while(1);
}

void trapOnWatchdogReset(void)
{
    if(hri_rstc_get_RCAUSE_WDT_bit(RSTC))
    {
        asm("bkpt");
    }
}


void monitorTasks(void)
{
    TaskStatus_t allTasks[MAX_TASKS];
    for(int i = 0; i < MAX_TASKS; i++)
    {
        allTasks[i].usStackHighWaterMark = 0xFFFF;
    }
    uint32_t runTime = 0;
    uxTaskGetSystemState(allTasks, MAX_TASKS, &runTime);
    for(int i = 0; i < MAX_TASKS; i++)
    {
        if(allTasks[i].usStackHighWaterMark < MIN_STACK_SIZE_WORDS)
        {
            char buffer[10];
            UBaseType_t currentHighWaterMark = allTasks[i].usStackHighWaterMark;
            sprintf(buffer, allTasks[i].pcTaskName);
            asm("nop");                
        }
    }
}

void TeaIOTest(int tickRateMs)
{
    static bool outputsInit = false;
    if(!outputsInit)
    {
        gpio_set_pin_direction(LOCK_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(LOCK_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_pin_direction(SIDE_SELECT_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(SIDE_SELECT_PIN, GPIO_PIN_FUNCTION_OFF);
        
        gpio_set_pin_direction(DIL0_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(DIL0_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_pin_direction(DIL1_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(DIL1_PIN, GPIO_PIN_FUNCTION_OFF);
        
        gpio_set_pin_direction(SUGAR0_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(SUGAR0_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_pin_direction(SUGAR1_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(SUGAR1_PIN, GPIO_PIN_FUNCTION_OFF);
        
        outputsInit = true;
    }
    static int LockSideCounter = 0;
    static int Sugar1Sugar0Counter = 0;
    static int Dilutor1Dilutor0Counter = 0;
    //static bool lastReed0State = true;
    static bool lastReed1State = true;
    static int startReed1Counter = false;
    static bool lastReed2State = true;
    static int startReed2Counter = false;
    static bool lastReed3State = true;
    static bool startReed3Counter = false;
    
    if(gpio_get_pin_level(REED1_PIN) != lastReed1State && LockSideCounter == 0)
    {
        if(gpio_get_pin_level(REED1_PIN))
        {
            gpio_set_pin_level(LOCK_PIN, 1);
            gpio_set_pin_level(SIDE_SELECT_PIN, 0);
        }
        else
        {
            gpio_set_pin_level(LOCK_PIN, 0);
            gpio_set_pin_level(SIDE_SELECT_PIN, 1);
        }
        startReed1Counter = true;
    }
    else if (LockSideCounter < TURNOFF_TIME_MS && startReed1Counter)
    {
        LockSideCounter += tickRateMs;
    }
    else if(LockSideCounter >= TURNOFF_TIME_MS)
    {
        gpio_set_pin_level(LOCK_PIN, 0);
        gpio_set_pin_level(SIDE_SELECT_PIN, 0);
        LockSideCounter = 0;
        startReed1Counter = false;
    }
    //Dilutor Control
    if(gpio_get_pin_level(REED2_PIN) != lastReed2State && Dilutor1Dilutor0Counter == 0)
    {
        if(gpio_get_pin_level(REED2_PIN))
        {
            gpio_set_pin_level(DIL0_PIN, 1);
            gpio_set_pin_level(DIL1_PIN, 0);
        }
        else
        {
            gpio_set_pin_level(DIL0_PIN, 0);
            gpio_set_pin_level(DIL1_PIN, 1);
        }
        startReed2Counter = true;
    }
    else if (Dilutor1Dilutor0Counter < TURNOFF_TIME_MS && startReed2Counter)
    {
        Dilutor1Dilutor0Counter += tickRateMs;
    }
    else if(Dilutor1Dilutor0Counter >= TURNOFF_TIME_MS)
    {
        gpio_set_pin_level(DIL0_PIN, 0);
        gpio_set_pin_level(DIL1_PIN, 0);
        Dilutor1Dilutor0Counter = 0;
        startReed2Counter = false;
    } 
    //Sugar Control
    if(gpio_get_pin_level(REED3_PIN) != lastReed3State && Sugar1Sugar0Counter == 0)
    {
        if(gpio_get_pin_level(REED3_PIN))
        {
            gpio_set_pin_level(SUGAR0_PIN, 1);
            gpio_set_pin_level(SUGAR1_PIN, 0);
        }
        else
        {
            gpio_set_pin_level(SUGAR0_PIN, 0);
            gpio_set_pin_level(SUGAR1_PIN, 1);
        }
        startReed3Counter = true;
    }
    else if (Sugar1Sugar0Counter < TURNOFF_TIME_MS && startReed3Counter)
    {
        Sugar1Sugar0Counter += tickRateMs;
    }
    else if(Sugar1Sugar0Counter >= TURNOFF_TIME_MS)
    {
        gpio_set_pin_level(SUGAR0_PIN, 0);
        gpio_set_pin_level(SUGAR1_PIN, 0);
        Sugar1Sugar0Counter = 0;
        startReed3Counter = false;
    }    
    lastReed1State = gpio_get_pin_level(REED1_PIN);
    lastReed2State = gpio_get_pin_level(REED2_PIN);
    lastReed3State = gpio_get_pin_level(REED3_PIN);
}

