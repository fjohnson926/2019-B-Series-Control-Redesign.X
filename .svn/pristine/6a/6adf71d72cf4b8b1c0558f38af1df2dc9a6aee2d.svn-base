/* 
 * File:   ScreenManager.cpp
 * Author: JTurner
 * 
 * Created on April 23, 2019, 1:30 PM
 */

#include "ScreenManager.h"
#include "string.h"
#include "StringManipulation.h"
#include "WtchDog.h"


ScreenManager::ScreenManager(TextLCD * display_p) {
    myDisplay_p = display_p;
    immediateMessage = false;    
    isPrintingMessage = false;
    oneMessageLoaded = false;
    isMsgQueueEmpty = true;
    immediateMessage = false;    
}

void ScreenManager::initialize()
{   
    initializeBacklight();
    turnOnBacklight();
    if(xTaskCreate(OS_Task, "PrntCtr", TASK_SCREENMANAGE_STACK_SIZE, this, TASK_SCREENMANAGE_STACK_PRIORITY, NULL) != pdPASS) 
    {
        #if __DEBUG
            asm("bkpt");    
        #endif
    }
    
    int * test = 0;
    int sizeOfTest = sizeof(test);  
    messageQueue = xQueueCreate(MAX_MESSAGE_QUEUE_SIZE, sizeOfTest);
    if( messageQueue == NULL )
    {
        #if __DEBUG
            asm("bkpt");    
        #endif
    }

}

void ScreenManager::addMessageToQueue(const ScreenMessages * message)
{   
    if(xQueueSend(messageQueue, (void*)&message, (TickType_t)0) == pdPASS)
    {
        isMsgQueueEmpty = false;
    }       
}

void ScreenManager::clearBuffer()
{
    xQueueReset(messageQueue);
}

void ScreenManager::showMessageNow(const ScreenMessages * message)
{
    isPrintingMessage = false;
    oneMessageLoaded = false;
    isMsgQueueEmpty = true;   
    immediateMessage = true;    
    clearBuffer();    
    addMessageToQueue(message);
}

void ScreenManager::resetScreen()
{
    isPrintingMessage = false;
    oneMessageLoaded = false;
    isMsgQueueEmpty = true;
    immediateMessage = true;
    clearBuffer();    
}

void ScreenManager::manageQueue()
{
    
    ScreenMessages * msg_p;    
    if(xQueueReceive(messageQueue, &msg_p, 0) == pdFALSE)
    {        
        if(!oneMessageLoaded)
        {
            myDisplay_p->cmdLvlWriteCharacters("  PLEASE  WAIT  ",NO_CHARS_PER_LINE);
            myDisplay_p->cmdLvlSetCursorAddress(0x40);
            myDisplay_p->cmdLvlWriteCharacters("     LOADING    ",NO_CHARS_PER_LINE);        
            pollImmediateMessageFlagAndWaitMs(100);
        }
        isMsgQueueEmpty = true;
        isPrintingMessage = false;
        return;
    }
    if(!msg_p)
    {
        return;
    }
    ScreenMessages currentMessage(*msg_p);    
    isPrintingMessage = true;
    oneMessageLoaded = true;
    //Figure out message type
    if(currentMessage.messageType == STATIC_MSG)
    {
        myDisplay_p->cmdLvlClearDisplayReturnHome();
        int leftOffset = getOffsetFromLeftForCenterJustification(currentMessage.topString_p,strlen(currentMessage.topString_p),NO_CHARS_PER_LINE);
        myDisplay_p->cmdLvlSetCursorAddress(0x00+leftOffset);
        myDisplay_p->cmdLvlWriteCharacters(currentMessage.topString_p,strlen(currentMessage.topString_p));        
        leftOffset = getOffsetFromLeftForCenterJustification(currentMessage.bottomString_p,strlen(currentMessage.bottomString_p),NO_CHARS_PER_LINE);
        myDisplay_p->cmdLvlSetCursorAddress(0x40+leftOffset);
        myDisplay_p->cmdLvlWriteCharacters(currentMessage.bottomString_p,strlen(currentMessage.bottomString_p));        
        pollImmediateMessageFlagAndWaitMs(currentMessage.screenTimeMs);
        isPrintingMessage = false;
    }
    else if(currentMessage.messageType == SCROLLING_MSG)
    {
        myDisplay_p->cmdLvlClearDisplayReturnHome();
        //Center justify upper portion of message.
        int leftOffset = getOffsetFromLeftForCenterJustification(currentMessage.topString_p,strlen(currentMessage.topString_p),NO_CHARS_PER_LINE);
        myDisplay_p->cmdLvlSetCursorAddress(0x00+leftOffset);
        myDisplay_p->cmdLvlWriteCharacters(currentMessage.topString_p,strlen(currentMessage.topString_p));     
                
        //Get botton line of display ready
        int msgSize = strlen(currentMessage.bottomString_p);
        int bufferSize = strlen(currentMessage.bottomString_p)+ 2*NO_CHARS_PER_LINE+1;
        char buffer[bufferSize];
        initializeString(buffer,bufferSize,' ');
        copyStringToBuffer(buffer,bufferSize,currentMessage.bottomString_p,NO_CHARS_PER_LINE,msgSize);
        myDisplay_p->cmdLvlSetCursorAddress(0x40);
        for(int i = 0; i < bufferSize-NO_CHARS_PER_LINE+1; i++)
        {
            myDisplay_p->cmdLvlWriteCharacters(&(buffer[i]),NO_CHARS_PER_LINE);
            myDisplay_p->cmdLvlSetCursorAddress(0x40);
            if(pollImmediateMessageFlagAndWaitMs(CHARACTER_SHIFT_PERIOD_MS))
            {
                break;
            }
        }
        isPrintingMessage = false;
    }
}

bool ScreenManager::pollImmediateMessageFlagAndWaitMs(unsigned int waitTimeMs)
{
    int approxWaitTimeMs = 0;
    immediateMessage = false;
    while(approxWaitTimeMs < waitTimeMs)
    {    
        HAL_WAIT_MS(1);
        if(immediateMessage)
        {
            immediateMessage = false;
            return true;
        }
        approxWaitTimeMs +=1;
    }
}

void ScreenManager::initializeBacklight()
{
    gpio_set_pin_direction(LCD_BACKLIGHT_PIN, GPIO_DIRECTION_OUT); 
    gpio_set_pin_function(LCD_BACKLIGHT_PIN, GPIO_PIN_FUNCTION_OFF);
}

void ScreenManager::turnOnBacklight()
{
    gpio_set_pin_level(LCD_BACKLIGHT_PIN,1);
}

void ScreenManager::turnOffBacklight()
{
    gpio_set_pin_level(LCD_BACKLIGHT_PIN,0);
}

bool ScreenManager::isBusy()
{
    if(isPrintingMessage || !isMsgQueueEmpty)
    {
        return true;
    }
    return false;
}

void ScreenManager::OS_Task(void * parm_p)
{
    
    ScreenManager * screenMgr_p =  (ScreenManager *)parm_p;
    TextLCD * display_p = screenMgr_p->myDisplay_p;
    display_p->usrLvlLCDinitialize();
    
    while(1)
    {           
        screenMgr_p->manageQueue();
        WDogSetFlg(TASK_OS_SCREEN_MNG);
        os_sleep(100/portTICK_PERIOD_MS);
    }
}

