/* 
 * File:   CapsenseController.h
 * Author: JTurner
 *
 * Created on May 17, 2019, 11:45 AM
 */

#ifndef CAPSENSECONTROLLER_H
#define	CAPSENSECONTROLLER_H

#include "CapsenseControllerHal.h"

enum CAPSENSE_TOUCH_TEST_STATE
{
    WAITING_TO_START_TEST,
    START_TOUCH_TESTING,
    COMPLETED_TOUCH_TESTING,
};

typedef enum BUTTON_STATUS
{
    NOT_PRESSED,
    BUTTON_PRESSED,
    BUTTON_HELD,
    BUTTON_RELEASED,     
}ButtonStatus_t;

#define MAX_SIMULTANEOUS_ERRORS 100
#define TOUCH_COMM_ERROR -1
#define TOUCH_BUTTON_HELD -2

#define CONFIG_FILE_SIZE 128
#define RETRY_COUNT 3

#define REGISTER_TOUCH_STATUS 0xAA
#define REGISTER_DEVICE_ID    0x90
#define REGISTER_FAMILY_ID    0x8F
#define REGISTER_CMD_CTRL     0x86

#define CONFIG_REGISTER_FIRST 0x00
#define CONFIG_REGISTER_LAST  0x7E

#define CMD_CTRL_SAVE_NVRAM_VALUE 0x02
#define CMD_CTRL_RESET_CHIP_VALUE 0xFF
#define FAMILY_ID_VALUE       0x9A

#define SIMULTANEOUS_TOUCH_COUNT 5

#ifdef __cplusplus

class CapsenseController {
public:
    CapsenseController();
    
    //Main Task
    static void touch_chip_task(void *p);
    
    int touchTestState;
    bool buttonPressRead;
    bool buttonHoldRead;
    int buttonHoldTimeMs;
    //Container of Capsense controllers
    static CapsenseController * CapsenseControllers[NUMBER_OF_TOUCH_CONTROLLERS];
    int communicationErrorCount;
    bool communicationSuccessful;
    uint16_t currentButtonValue;
    ButtonStatus_t currentButtonStatus;
    TaskHandle_t myTaskHandle;
    bool phantomTouchesDetected;
    uint16_t previousButtonValue;
    bool controllerSetupComplete;
    uint16_t unreadReleasedButtonValue;
    
    void clearDebounceBuffer(); 
    //Read then clear the current touch status
    signed int getLastUnreadReleasedValue();
    ButtonStatus_t getCurrentButtonStatus();
    uint16_t getCurrentButtonValue();
    signed int grabCurrentTouchStatus();        
    void saveButtonStatus(uint16_t data, int saveRateMs);        
    void softwareReset();   
    
private:    
    static int capsenseWriteIndex;
    uint16_t debounceBuffer[SIMULTANEOUS_TOUCH_COUNT];
    int debounceWriteIndex;
        
    void initializeCapsenseControllerArray();
    void loadDebounceBuffer(uint16_t data);    
    uint16_t getDebouncedValue();
};
#endif
    
#endif	/* CAPSENSECONTROLLER_H */

