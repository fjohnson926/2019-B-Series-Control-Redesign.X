#include "SystemManager.h"
#include "sprintfoverride.h"

static char upline1buffer[20];
static char upline2buffer[20];
static char lowline1buffer[20];
static char lowline2buffer[20];
static char noWaterDetectStr[] = "(NOWTR)";
static char waterDetectStr[]= "(WTR)";

ScreenMessages UpperProbeMsg(upline1buffer, upline2buffer, 1000);
ScreenMessages LowerProbeMsg(lowline1buffer, lowline2buffer, 1000);
bool viewLevelProbeInit = false;

static void updateUpperLineBuffers(int adc, bool waterDetected)
{
    char * waterDetect;
    if(waterDetected)
    {
        waterDetect = waterDetectStr;
    }
    else
    {
        waterDetect = noWaterDetectStr;
    }
    sprintf(upline1buffer,"Upper Probe");
    sprintf(upline2buffer,"A=%d %s", adc, waterDetect);    
}

static void updateLowerLineBuffers(int adc, bool waterDetected)
{
    char * waterDetect;
    if(waterDetected)
    {
        waterDetect = waterDetectStr;
    }
    else
    {
        waterDetect = noWaterDetectStr;
    }
    sprintf(lowline1buffer,"Lower Probe");
    sprintf(lowline2buffer,"A=%d %s", adc, waterDetect);
}

void SystemManager::stateViewWaterLevels(void)
{    
    if(!viewLevelProbeInit)
    {
        int upperProbeADC = (int)myTank->getAverageFillProbeADCValue();
        int lowerProbeADC = (int)myTank->getAverageHeatWaterLevelProbeADCValue();
        viewLevelProbeInit = true;
        updateUpperLineBuffers(upperProbeADC, myTank->highProbeWaterDetected());
        myUI->Screen->showMessageNow(&UpperProbeMsg);
        updateLowerLineBuffers(lowerProbeADC, myTank->lowProbeWaterDetected());
        myUI->Screen->addMessageToQueue(&LowerProbeMsg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();
        viewLevelProbeInit = false;
        changeState(MENU_BROWSE);
        return;
    }
    
    if(!myUI->Screen->isBusy())
    {  
        int upperProbeADC = (int)myTank->getAverageFillProbeADCValue();
        int lowerProbeADC = (int)myTank->getAverageHeatWaterLevelProbeADCValue();        
        updateUpperLineBuffers(upperProbeADC, myTank->highProbeWaterDetected());
        myUI->Screen->showMessageNow(&UpperProbeMsg);
        updateLowerLineBuffers(lowerProbeADC, myTank->lowProbeWaterDetected());
        myUI->Screen->addMessageToQueue(&LowerProbeMsg);
    }    
}