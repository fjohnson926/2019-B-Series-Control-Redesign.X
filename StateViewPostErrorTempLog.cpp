#include "SystemManager.h"
#include "sprintfoverride.h"

static char line1buffer[20];
static char line2buffer[20];

static ScreenMessages TemperatureMessage(line1buffer, line2buffer, 500);
static bool viewTemperatureInit = false;
static int readIndex = 0;
static int sampleNumber = 0;

static void updateLineBuffers(int sample,int temperature)
{
    sprintf(line1buffer,"Sample %d", sample);  
    sprintf(line2buffer,"T=%dF", temperature);
}

void SystemManager::stateViewPostError4Temperatures()
{    
    if(!viewTemperatureInit)
    {
        viewTemperatureInit = true;        
        //Configure readIndex
        readIndex = 0;
        sampleNumber = 1;
        updateLineBuffers(sampleNumber, myTank->postErrorTempData[readIndex]);
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
        if(readIndex < NUMBER_OF_POST_ERROR_TEMPS-1)
        {
            readIndex++;
            sampleNumber++;
        }
        else
        {
            readIndex = 0;
            sampleNumber = 1;
        }
        updateLineBuffers(sampleNumber, myTank->postErrorTempData[readIndex]);
        myUI->Screen->showMessageNow(&TemperatureMessage);   
    }
    else if (releasedTouchValue == TOUCH_PREVIOUS)
    {
        if(readIndex > 0)
        {
            readIndex--;
            sampleNumber--;
        }
        else
        {
            readIndex = NUMBER_OF_POST_ERROR_TEMPS-1;
            sampleNumber = NUMBER_OF_POST_ERROR_TEMPS;
        }

        updateLineBuffers(sampleNumber, myTank->postErrorTempData[readIndex]);
        myUI->Screen->showMessageNow(&TemperatureMessage);
    }
}