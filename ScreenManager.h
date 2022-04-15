/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Electrolux

  @File Name
    ScreenManage.h

  @Summary
    The screen manager is responsible for handling communications with the 2 line 16 character display on B Series. 

  @Description
    
 */
/* ************************************************************************** */

#ifndef SCREENMANAGER_H
#define	SCREENMANAGER_H

#include "TextLCD.h"
#include "rtos_port.h"
#include "ScreenMessages.h"
#include "gm_SystemPinMap.h"
#include "CommonScreenMessages.h"

#define TASK_SCREENMANAGE_STACK_SIZE (450 / sizeof(portSTACK_TYPE))
#define TASK_SCREENMANAGE_STACK_PRIORITY (tskIDLE_PRIORITY + 1)
#define HAL_WAIT_MS(time) os_sleep(time/portTICK_PERIOD_MS)
#define MAX_MESSAGE_QUEUE_SIZE 20

#define NO_CHARS_PER_LINE 16
#define NO_MEMORY_ADDRESSES_PER_LINE 40
#define CHARACTER_SHIFT_PERIOD_MS 300

#define LCD_BACKLIGHT_PIN PA09_PIN

class ScreenManager {
public:
    ScreenManager(TextLCD * display_p);
    void initialize();
    static void OS_Task(void * parm_p);
    
    void addMessageToQueue(const ScreenMessages * message);
    void showMessageNow(const ScreenMessages * message);    
    void manageQueue();
    bool pollImmediateMessageFlagAndWaitMs(unsigned int waitTimeMs);    
    void initializeBacklight();
    void turnOnBacklight();
    void turnOffBacklight();
    void resetScreen();
    
    bool isPrintingMessage;
    QueueHandle_t messageQueue;
    bool isBusy();
    bool isMsgQueueEmpty;
    bool oneMessageLoaded;    
    void clearBuffer();
       
    
private:
    TextLCD * myDisplay_p;        
    bool immediateMessage;
    
};

#endif	/* SCREENMANAGER_H */

