/* 
 * File:   CapsenseController.cpp
 * Author: JTurner
 * 
 * Created on May 17, 2019, 11:45 AM
 */

#include "CapsenseController.h"
#include "WtchDog.h"

TOUCH_HAL_INITIALIZE_I2C_PARMS();

CapsenseController * CapsenseController::CapsenseControllers[NUMBER_OF_TOUCH_CONTROLLERS];
int CapsenseController::capsenseWriteIndex = 0;

CapsenseController::CapsenseController() 
{
    communicationSuccessful = false;
    communicationErrorCount = 0;
    previousButtonValue = TOUCH_NONE;
    currentButtonValue = TOUCH_NONE;
    currentButtonStatus = NOT_PRESSED;
    unreadReleasedButtonValue = TOUCH_NONE;
    touchTestState = WAITING_TO_START_TEST;
    buttonHoldTimeMs = 0;
    if(capsenseWriteIndex == 0)
    {
        initializeCapsenseControllerArray();
    }

    #if __DEBUG    
    if(capsenseWriteIndex + 1> NUMBER_OF_TOUCH_CONTROLLERS)
    {
        asm("bkpt");
    }    
    #endif
    
    CapsenseControllers[capsenseWriteIndex] = this;
    capsenseWriteIndex++;
    clearDebounceBuffer();
    controllerSetupComplete = false;
    phantomTouchesDetected = false;
    
    if(xTaskCreate(touch_chip_task, "TchChip", TOUCH_TASK_STACK_SIZE, this, TOUCH_TASK_PRIORITY, &myTaskHandle) != pdPASS) 
    {
        #if __DEBUG
              asm("bkpt");    
        #endif
    }
}

void CapsenseController::initializeCapsenseControllerArray()
{
    for(int i = 0; i < NUMBER_OF_TOUCH_CONTROLLERS; i++)
    {
        CapsenseControllers[i] = NULL;
    }
}

signed int CapsenseController::grabCurrentTouchStatus()
{
    if(communicationSuccessful)
    {
        uint16_t returnValue = currentButtonStatus;
        currentButtonValue = TOUCH_NONE;
        return returnValue;
    }
    else
    {
        return TOUCH_COMM_ERROR;
    }
}

signed int CapsenseController::getLastUnreadReleasedValue()
{
    signed int lastUnreadReleasedButtonValue = unreadReleasedButtonValue;
    unreadReleasedButtonValue = TOUCH_NONE;
    return lastUnreadReleasedButtonValue;
}

bool readRegister(uint8_t registerAddr, uint8_t * buffer)
{
    uint8_t outbuffer[2];    
    outbuffer[0] = registerAddr;
    if(buffer)
    {        
        TOUCH_HAL_IO_WRITE(outbuffer, 1);
        TOUCH_HAL_IO_READ(buffer, 2);
        return true;
    }
    return false;
}

void writeRegister(uint8_t registerAddr, uint8_t * buffer, int size)
{
    uint8_t outbuffer[size+1];
    outbuffer[0] = registerAddr;
    for(int i = 0; i < size; i++)
    {
        outbuffer[i+1] = buffer[i];
    }
    TOUCH_HAL_IO_WRITE(outbuffer, size+1);    
}

bool checkRegisterLSBByte(uint8_t registerAddr, uint8_t value, uint8_t maxTries)
{
    uint8_t buffer[2];
    for(int i = 0; i < maxTries; i++)
    {
        readRegister(registerAddr, buffer);
        if(buffer[0] == value)
        {
            return true;
        }
    }
    return false;
}

bool writeConfigFile(const uint8_t * data, int size)
{ 
    int i = 0;
    while(i < size)
    {
        uint8_t data[2] = {configFile[i], configFile[i+1]};
        writeRegister(i,data,2);            
        i+=2;
    }
}

bool compareWriteConfigFiles(const uint8_t * uCData)
{
    uint8_t offsetData[2];
    int i = 0;
    bool valid = false;
    int invalidCount = 0;
    int successCount = 0;
    while(i < 128)
    {
        bool readFailed = true;
        for(int j = 0; j < RETRY_COUNT; j++)
        {
            readRegister(i, offsetData);
            {
                if(offsetData[0] == uCData[i] && offsetData[1] == uCData[i+1] )
                {
                    uint8_t test[2] = {uCData[i], uCData[i+1]};
                    readFailed = false;
                    break;
                }  
            }
        }
        if(readFailed)
        {
            invalidCount++;
        }
        else
        {
            successCount++;
        }
        i += 2;
    }
    if(invalidCount)
    {
        valid = false;
    }
    else
    {
        valid = true;
    }
    return valid;
}


ButtonStatus_t CapsenseController::getCurrentButtonStatus()
{
    return currentButtonStatus;
}

uint16_t CapsenseController::getCurrentButtonValue()
{
    return currentButtonValue;
}

// testing
void CapsenseController::saveButtonStatus(uint16_t data, int saveRateMs)
{      
    loadDebounceBuffer(data);
    if(previousButtonValue == TOUCH_NONE && currentButtonValue != TOUCH_NONE)
    {
        buttonHoldTimeMs = 0;
        buttonPressRead = false;
        buttonHoldRead = false;
        currentButtonStatus = BUTTON_PRESSED;
    }
    else if( (currentButtonValue != TOUCH_NONE && previousButtonValue == currentButtonValue && currentButtonStatus == BUTTON_PRESSED)
            || (currentButtonValue == previousButtonValue && currentButtonStatus == BUTTON_HELD) )
    {
        buttonHoldTimeMs += saveRateMs;
        currentButtonStatus = BUTTON_HELD;        
    }
    else if(currentButtonValue == TOUCH_NONE && currentButtonStatus == BUTTON_HELD)
    {
        currentButtonStatus = BUTTON_RELEASED;
        unreadReleasedButtonValue = previousButtonValue;
    }
    previousButtonValue = currentButtonValue;
    currentButtonValue = getDebouncedValue();    
}

void CapsenseController::loadDebounceBuffer(uint16_t data)
{
    debounceBuffer[debounceWriteIndex] = data;
    debounceWriteIndex++;
    if(debounceWriteIndex >= SIMULTANEOUS_TOUCH_COUNT)
    {
        debounceWriteIndex = 0;
    }    
}

uint16_t CapsenseController::getDebouncedValue()
{
    return getHighestFrequencyValue(debounceBuffer, SIMULTANEOUS_TOUCH_COUNT);
}

void CapsenseController::clearDebounceBuffer()
{
    for(int i = 0; i < SIMULTANEOUS_TOUCH_COUNT; i++)
    {
        debounceBuffer[i] = 0;
    }
    debounceWriteIndex = 0;
}

void CapsenseController::softwareReset()
{
    uint8_t ctrl_cmd_save_cfg_nvram[1] = {CMD_CTRL_SAVE_NVRAM_VALUE};
    writeRegister(REGISTER_CMD_CTRL, ctrl_cmd_save_cfg_nvram,1);
    TOUCH_HAL_SLEEP_MS(300); //Delay definitely needed otherwise chip will not respond to touches
    uint8_t ctrl_cmd_reset_chip[1] = {CMD_CTRL_RESET_CHIP_VALUE};
    writeRegister(REGISTER_CMD_CTRL, ctrl_cmd_reset_chip, 1);
}

void CapsenseController::touch_chip_task(void *p)   {	
    uint8_t buf[10];
    int noTouchesInIdle = 0;
    int setupAttempts = 1;
    TOUCH_HAL_I2C_INITIALIZE_FUNCTIONS();
    CapsenseController * touchControl = (CapsenseController*)p;   
 
    //Load configuration array from EZ-Click software
    while(setupAttempts < MAX_SETUP_ATTEMPTS && !touchControl->communicationSuccessful)   
    {
        vTaskPrioritySet(touchControl->myTaskHandle, tskIDLE_PRIORITY + 3 );        
        for(int i = 0; i < RETRY_COUNT; i++)    
        {
            if(!compareWriteConfigFiles(configFile))    
            { 
                writeConfigFile(configFile, CONFIG_FILE_SIZE);        
                touchControl->softwareReset();
                if(compareWriteConfigFiles(configFile)) 
                {                       
                    vTaskPrioritySet(touchControl->myTaskHandle, TOUCH_TASK_PRIORITY);
                    touchControl->communicationSuccessful = true;                        
                    break;
                }
            }
            else    
            {
                touchControl->communicationSuccessful = true;  
                break;
            }
        }
        WDogSetFlg(TASK_TOUCH_CHIP);
        vTaskPrioritySet(touchControl->myTaskHandle, TOUCH_TASK_PRIORITY);
        setupAttempts++;
    }
    
    if(touchControl->communicationSuccessful)
    {
        while(touchControl->touchTestState == WAITING_TO_START_TEST)
        {
            TOUCH_HAL_SLEEP_MS(TOUCH_TASK_LOOP_RATE_MS);
            WDogSetFlg(TASK_TOUCH_CHIP);
        }
        //Detect any touches in a state where no user should be touching the UI.
        for(int i = 0; i < IDLE_NO_TOUCH_TESTS; i++)    
        {
            readRegister(REGISTER_TOUCH_STATUS, buf);            
            uint16_t touchValue = 0;
            touchValue = buf[1] << 8;
            touchValue |= buf[0];            
            if(touchValue) 
            {
               noTouchesInIdle++; 
            }
            TOUCH_HAL_SLEEP_MS(TOUCH_TASK_LOOP_RATE_MS);
            WDogSetFlg(TASK_TOUCH_CHIP);
        }

        //Set fault is touches exceed acceptable amount
        if(noTouchesInIdle > IDLE_TOUCHES_FAILED) 
        {
           touchControl->phantomTouchesDetected = true;            
        }

        touchControl->controllerSetupComplete = true;
        touchControl->touchTestState = COMPLETED_TOUCH_TESTING;    
    }
	while (1)    
    {   
        if (checkRegisterLSBByte(REGISTER_FAMILY_ID, FAMILY_ID_VALUE, RETRY_COUNT) && touchControl->communicationSuccessful)    
        {   
            TOUCH_HAL_SLEEP_MS(5);            
            readRegister(REGISTER_TOUCH_STATUS, buf);            
            uint16_t touchValue = 0;
            touchValue = buf[1] << 8;
            touchValue |= buf[0];            
            touchControl->saveButtonStatus(touchValue, TOUCH_TASK_LOOP_RATE_MS);
            touchControl->communicationErrorCount = 0;            
        }
        else    
        {            
            if(touchControl->communicationErrorCount >= MAX_SIMULTANEOUS_ERRORS)    
            {
                touchControl->communicationSuccessful = false;
            }
            else    
            {
                touchControl->communicationErrorCount += RETRY_COUNT;
            }
        }        
        WDogSetFlg(TASK_TOUCH_CHIP);
        TOUCH_HAL_SLEEP_MS(TOUCH_TASK_LOOP_RATE_MS);
	}
}


