#include "SystemManager.h"
#include "sprintfoverride.h"

char line1buffer[20];
char line2buffer[20];

ScreenMessages TemperatureMessage(line1buffer, line2buffer, 500);
bool viewTemperatureInit = false;

void updateLineBuffers(int adc, int resistance, int temperature)
{

    sprintf(line1buffer,"A = %d", adc);  
    sprintf(line2buffer,"R=%d,T=%dF", resistance, temperature);
}

void SystemManager::stateViewTemperatures(void)
{
    double ADCReading = myTank->conditionedADCValue;
    double resistance = getResistanceFromCount(ADCReading);
    int temperature = myTank->currentTemperatureF;
    
    if(!viewTemperatureInit)
    {
        viewTemperatureInit = true;
        updateLineBuffers(ADCReading, resistance, temperature);
        myUI->Screen->showMessageNow(&TemperatureMessage);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();
        viewTemperatureInit = false;
        changeState(MENU_BROWSE);
        return;
    }
    if(!myUI->Screen->isBusy())
    {   
        updateLineBuffers(ADCReading, resistance, temperature);
        myUI->Screen->addMessageToQueue(&TemperatureMessage);
    }    
}