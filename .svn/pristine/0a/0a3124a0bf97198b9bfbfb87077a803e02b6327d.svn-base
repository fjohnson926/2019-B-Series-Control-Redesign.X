#include "SystemManager.h"

static ScreenMessages TeaOutputMsg("FCT TEA","OUTPUTS");
static ScreenMessages TeaOutputMap3Msg("CS05","LCK");
static ScreenMessages TeaOutputMap4Msg("CS12","SIDE");
static ScreenMessages TeaOutputMap5Msg("CS07","SUG2");
static ScreenMessages TeaOutputMap6Msg("CS10","SUG1");
static ScreenMessages TeaOutputMap7Msg("CS08","DLT2");
static ScreenMessages TeaOutputMap8Msg("CS09","DLT1");

static bool outputActive = false;
static int outputCounter = 0;

void SystemManager::initTeaGPIO(void)
{
    static bool teaoutputsinit = false;
    if (!teaoutputsinit)
    {
        gpio_set_pin_direction(LOCK_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(LOCK_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_drvstr(LOCK_PIN);
        
        gpio_set_pin_direction(SIDE_SELECT_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(SIDE_SELECT_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_drvstr(SIDE_SELECT_PIN);
        
        gpio_set_pin_direction(SUGAR1_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(SUGAR1_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_drvstr(SUGAR1_PIN);
        
        gpio_set_pin_direction(SUGAR0_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(SUGAR0_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_drvstr(SUGAR0_PIN);
        
        gpio_set_pin_direction(DIL1_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(DIL1_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_drvstr(DIL1_PIN);
        
        gpio_set_pin_direction(DIL0_PIN, GPIO_DIRECTION_OUT); 
        gpio_set_pin_function(DIL0_PIN, GPIO_PIN_FUNCTION_OFF);
        gpio_set_drvstr(DIL0_PIN);
        
        teaoutputsinit = true;
    }
}

 void SystemManager::turnOffTeaOutputs(void)
{
    gpio_set_pin_level(LOCK_PIN, 0);
    gpio_set_pin_level(SIDE_SELECT_PIN, 0);
    gpio_set_pin_level(SUGAR1_PIN, 0);
    gpio_set_pin_level(SUGAR0_PIN, 0);
    gpio_set_pin_level(DIL1_PIN, 0);
    gpio_set_pin_level(DIL0_PIN, 0);
    myUI->LEDs->turnOffAllLEDs(true);     
    outputActive = false;
}

void SystemManager::stateManageTeaOutputs(void)
{
    if(changedState)
    {
        myUI->Screen->showMessageNow(&TeaOutputMsg);
        initTeaGPIO();
        changedState = false;
        outputActive = false;
        outputCounter = 0;         
    }
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&TeaOutputMsg);     
        myUI->Screen->addMessageToQueue(&TeaOutputMap3Msg);
        myUI->Screen->addMessageToQueue(&TeaOutputMap4Msg);
        myUI->Screen->addMessageToQueue(&TeaOutputMap5Msg);
        myUI->Screen->addMessageToQueue(&TeaOutputMap6Msg);
        myUI->Screen->addMessageToQueue(&TeaOutputMap7Msg);
        myUI->Screen->addMessageToQueue(&TeaOutputMap8Msg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        turnOffTeaOutputs();
        changeState(FCT_MAIN_MENU);
        return;
    }
    else if(releasedTouchValue == TOUCH_WARM_1)
    {
        if(outputActive)
        {
            turnOffTeaOutputs();
        }
        else
        {
            gpio_set_pin_level(LOCK_PIN, 1);
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer01Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_2)
    {
        if(outputActive)
        {
            turnOffTeaOutputs();
        }
        else
        {
            gpio_set_pin_level(SIDE_SELECT_PIN, 1);
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer02Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_3)
    {
        if(outputActive)
        {
            turnOffTeaOutputs();
        }
        else
        {
            gpio_set_pin_level(SUGAR1_PIN, 1);
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer03Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_4)
    {
        if(outputActive)
        {
            turnOffTeaOutputs();
        }
        else
        {
            gpio_set_pin_level(SUGAR0_PIN, 1);
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer04Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_5)
    {
        if(outputActive)
        {
            turnOffTeaOutputs();
        }
        else
        {
            gpio_set_pin_level(DIL1_PIN, 1);
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer05Led->turnOnLED();
        }
    }
    else if(releasedTouchValue == TOUCH_WARM_6)
    {
        if(outputActive)
        {
            turnOffTeaOutputs();
        }
        else
        {
            gpio_set_pin_level(DIL0_PIN, 1);
            outputCounter = 0;
            outputActive = true;
            myUI->LEDs->warmer06Led->turnOnLED();
        }
    }
    
    outputCounter += SYSTEM_MANAGER_TASK_RATE_MS; 
    if(outputCounter > FCT_OUTPUT_TIME_MS)
    {
        turnOffTeaOutputs();
    }
}
