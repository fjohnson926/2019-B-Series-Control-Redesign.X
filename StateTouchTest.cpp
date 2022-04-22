#include "SystemManager.h"
#include "sprintfoverride.h"

ScreenMessages TouchMsg("TOUCH TEST", "MODE");
const int bufferSize = 7;
const int scrollSpeedMs = 1250;
char powerCountStr[bufferSize];
ScreenMessages PowerCountMsg("POWER TOUCH", powerCountStr);
char leftBrewCountStr[bufferSize];
ScreenMessages leftBrewCountMsg("LEFT BREW TOUCH", leftBrewCountStr);
char rightBrewCountStr[bufferSize];
ScreenMessages rightBrewCountMsg("RGHT BREW TOUCH", rightBrewCountStr);
char warmer0CountStr[bufferSize];
ScreenMessages warmer0CountMsg("WARMER 1 TOUCH", warmer0CountStr);
char warmer1CountStr[bufferSize];
ScreenMessages warmer1CountMsg("WARMER 2 TOUCH", warmer1CountStr);
char warmer2CountStr[bufferSize];
ScreenMessages warmer2CountMsg("WARMER 3 TOUCH", warmer2CountStr);
char warmer3CountStr[bufferSize];
ScreenMessages warmer3CountMsg("WARMER 4 TOUCH", warmer3CountStr);
char warmer4CountStr[bufferSize];
ScreenMessages warmer4CountMsg("WARMER 5 TOUCH", warmer4CountStr);
char warmer5CountStr[bufferSize];
ScreenMessages warmer5CountMsg("WARMER 6 TOUCH", warmer5CountStr);
char nextCountStr[bufferSize];
ScreenMessages upCountMsg("UP TOUCHES", nextCountStr);
char previousCountStr[bufferSize];
ScreenMessages downCountMsg("DOWN TOUCHES", previousCountStr);
char cancelCountStr[bufferSize];
ScreenMessages backCountMsg("CANCEL TOUCHES", cancelCountStr);
char acceptCountStr[bufferSize];
ScreenMessages acceptCountMsg("ACCEPT TOUCHES", acceptCountStr);
char unknownCountStr[bufferSize];
ScreenMessages unknownCountMsg("UNKNOWN TOUCHES", unknownCountStr);

ScreenMessages * allMsgs[] = 
{
    &TouchMsg,
    &PowerCountMsg,
    &leftBrewCountMsg,
    &rightBrewCountMsg,
    &warmer0CountMsg,
    &warmer1CountMsg,
    &warmer2CountMsg,
    &warmer3CountMsg,
    &warmer4CountMsg,
    &warmer5CountMsg,
    &upCountMsg,
    &downCountMsg,
    &backCountMsg,
    &acceptCountMsg,
    &unknownCountMsg,    
};
int powerCount = 0;
int leftBrewCount = 0;
int rightBrewCount = 0;
int warmer1Count = 0;
int warmer2Count = 0;
int warmer3Count = 0;
int warmer4Count = 0;
int warmer5Count = 0;
int warmer6Count = 0;
int nextCount = 0;
int previousCount = 0;
int acceptCount = 0;
int cancelCount  = 0;
int unknownTouchCount = 0;

void SystemManager::stateTouchTest()
{
    signed int currentTouchStatus = myUI->TouchControls->grabCurrentTouchStatus();  
    if(currentTouchStatus == TOUCH_COMM_ERROR)
    {
        myUI->Screen->addMessageToQueue(&TouchControlTouchErrorMsg);
    }
    int arraySize = sizeof(allMsgs)/sizeof(ScreenMessages *);
    for(int i = 0; i < arraySize;i++)
    {
        allMsgs[i]->screenTimeMs = scrollSpeedMs;
    }
      
    switch(currentTouchStatus)
    {
        case TOUCH_POWER:    
            powerCount++;
            snprintf(powerCountStr,bufferSize,"%d",powerCount);            
            break;
//        case TOUCH_RIGHT_BREW:
//            //rightBrewCount++;
//            snprintf(rightBrewCountStr,bufferSize,"%d",rightBrewCount);  
//            break;
//        case TOUCH_LEFT_SINGLE_BREW:
//            //leftBrewCount++;
//            snprintf(leftBrewCountStr,bufferSize,"%d",leftBrewCount);  
//            break;
        case TOUCH_WARM_1:
            warmer1Count++;
            snprintf(warmer0CountStr,bufferSize,"%d",warmer1Count);  
            break;
        case TOUCH_WARM_2:
            warmer2Count++;
            snprintf(warmer1CountStr,bufferSize,"%d",warmer2Count);  
            break;
        case TOUCH_WARM_3:
            warmer3Count++;
            snprintf(warmer2CountStr,bufferSize,"%d",warmer3Count);  
            break;
        case TOUCH_WARM_4:
            warmer4Count++;
            snprintf(warmer3CountStr,bufferSize,"%d",warmer4Count);  
            break;
        case TOUCH_WARM_5:
            warmer5Count++;
            snprintf(warmer4CountStr,bufferSize,"%d",warmer5Count);  
            break;
        case TOUCH_WARM_6:
            warmer6Count++;
            snprintf(warmer5CountStr,bufferSize,"%d",warmer6Count);  
            break;
        case TOUCH_NEXT:
            nextCount++;
            snprintf(nextCountStr,bufferSize,"%d",nextCount);  
            break;
        case TOUCH_PREVIOUS:
            previousCount++;
            snprintf(previousCountStr,bufferSize,"%d",previousCount);  
            break;
        case TOUCH_CANCEL:
            cancelCount++;
            snprintf(cancelCountStr,bufferSize,"%d",cancelCount); 
            break;
        case TOUCH_ACCEPT:
            acceptCount++;
            snprintf(acceptCountStr,bufferSize,"%d",acceptCount); 
            break;
        case TOUCH_NONE:          
            break;
        default:
            unknownTouchCount++;
            snprintf(unknownCountStr,bufferSize,"%d",unknownTouchCount);  
            break;
    }
    
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&TouchMsg);
        myUI->Screen->addMessageToQueue(&PowerCountMsg);        
        myUI->Screen->addMessageToQueue(&leftBrewCountMsg);
        myUI->Screen->addMessageToQueue(&rightBrewCountMsg);
        myUI->Screen->addMessageToQueue(&warmer0CountMsg);
        myUI->Screen->addMessageToQueue(&warmer1CountMsg);
        myUI->Screen->addMessageToQueue(&warmer2CountMsg);
        myUI->Screen->addMessageToQueue(&warmer3CountMsg);
        myUI->Screen->addMessageToQueue(&warmer4CountMsg);
        myUI->Screen->addMessageToQueue(&warmer5CountMsg);        
        myUI->Screen->addMessageToQueue(&upCountMsg);
        myUI->Screen->addMessageToQueue(&downCountMsg);
        myUI->Screen->addMessageToQueue(&backCountMsg);
        myUI->Screen->addMessageToQueue(&acceptCountMsg);
        myUI->Screen->addMessageToQueue(&unknownCountMsg);        
    }    
}
