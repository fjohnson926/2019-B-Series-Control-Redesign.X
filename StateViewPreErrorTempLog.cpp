#include "SystemManager.h"
#include "sprintfoverride.h"

static char line1buffer[20];
static char line2buffer[20];

static ScreenMessages TemperatureMessage(line1buffer, line2buffer, 500);
static bool viewTemperatureInit = false;
static int readIndex = 0;
static int sampleNumber = 0;

void updateLineBuffers(int sample,int temperature)
{
    sprintf(line1buffer,"Sample %d", sample);  
    sprintf(line2buffer,"T=%dF", temperature);
}

void SystemManager::stateViewPreError4Temperatures()
{    
    if(!viewTemperatureInit)
    {
        viewTemperatureInit = true;        
        //Configure readIndex
        if(myTank->preErrorTempWriteIndex == 0)
        {
            readIndex = NUMBER_OF_PRE_ERROR_TEMPS-1;
            
        }
        else
        {
            readIndex = myTank->preErrorTempWriteIndex - 1;            
        }      
        sampleNumber = NUMBER_OF_PRE_ERROR_TEMPS;
        updateLineBuffers(sampleNumber, myTank->preErrorTempData[readIndex]);
        myUI->Screen->showMessageNow(&TemperatureMessage);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();
        viewTemperatureInit = false;
        changeState(MENU_BROWSE);
        return;
    }
    else if(releasedTouchValue == TOUCH_NEXT)
    {
        if(readIndex < NUMBER_OF_PRE_ERROR_TEMPS)
        {
            readIndex++;
        }
        else
        {
            readIndex = 0;
        }
        if(sampleNumber < NUMBER_OF_PRE_ERROR_TEMPS)
        {
            sampleNumber++;
        }
        else
        {
            sampleNumber = 1;
        }        
        updateLineBuffers(sampleNumber, myTank->preErrorTempData[readIndex]);
        myUI->Screen->showMessageNow(&TemperatureMessage);   
    }
    else if (releasedTouchValue == TOUCH_PREVIOUS)
    {
        if(readIndex > 0)
        {
            readIndex--;
        }
        else
        {
            readIndex = NUMBER_OF_PRE_ERROR_TEMPS-1;
        }
        if(sampleNumber == 1)
        {
            sampleNumber = NUMBER_OF_PRE_ERROR_TEMPS;
        }
        else
        {
            sampleNumber--;
        }
        updateLineBuffers(sampleNumber, myTank->preErrorTempData[readIndex]);
        myUI->Screen->showMessageNow(&TemperatureMessage);
    }
}