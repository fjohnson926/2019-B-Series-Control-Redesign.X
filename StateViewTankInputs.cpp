#include "SystemManager.h"
#include "sprintfoverride.h"

static char line1buffer[20];
static char line2buffer[20];
static ScreenMessages TankStatusMsg(line1buffer, line2buffer, 500);
static bool viewTemperatureInit = false;

static void updateLineBuffers(int tankADC, int LVLADC, int LVL2ADC)
{

    sprintf(line1buffer,"TMP = %d", tankADC);  
    sprintf(line2buffer,"L=%d,H=%d", LVL2ADC, LVLADC);
}

void SystemManager::stateFCTViewTankInputs(void)
{
    double ADCReading = myTank->conditionedADCValue;
    int LVLADC = (int)myTank->getAverageFillProbeADCValue();    
    int LVL2ADC = (int)myTank->getAverageHeatWaterLevelProbeADCValue(); 
    
    if(!viewTemperatureInit)
    {
        viewTemperatureInit = true;
        updateLineBuffers(ADCReading, LVLADC, LVL2ADC);
        myUI->Screen->showMessageNow(&TankStatusMsg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();
        viewTemperatureInit = false;
        changeState(FCT_MAIN_MENU);
        return;
    }
    
    if(!myUI->Screen->isBusy())
    {   
        updateLineBuffers(ADCReading, LVLADC, LVL2ADC);
        myUI->Screen->addMessageToQueue(&TankStatusMsg);
    }    
}