#include "SystemManager.h"
#include "NvParmManager.h"

int uiEditTankTemperatureF = 195;
#define MIN_TANK_TEMP_NORMAL_OPERATION_F 170

#define FIRST_VALUE_HOLD_TIME_MS 200
#define FAST_VALUE_HOLD_TIME_MS 3000
#define FAST_VALUE_DELTA_TEMP_F 5
#define FAST_VALUE_UPDATE_SPEED_MS 1000

//for displaying temperatures in C
#define UITEMPCBUFFER 2
#define WHOLE_NUMBER_INDEX 0
#define TENTHS_NUMBER_INDEX WHOLE_NUMBER_INDEX + 1

static int uiEditTankTemperatureC[UITEMPCBUFFER];
bool editTankTempInitialized = false;
char editTempBuffer[20];
ScreenMessages editTankTemperatureMsg("EDIT TANK TEMP", editTempBuffer, DEFAULT_STATIC_MENU_PRINT_TIME);

void convertTempFToTempC (int TempF, int * result, int resultSize)
{
    if(resultSize < 2)
    {
        return;
    }
    //Convert tempF to a floating point for floating point math
    float tempF_float = TempF;
    //Convert to Temp C
    float tempC_float = (tempF_float - 32.0) * (5.0/9.0);
    //The whole number part is just a chopped integer
    int tempC_int = tempC_float;
    result[WHOLE_NUMBER_INDEX] = tempC_int;
    //Get rid of the whole number part of the number, and get two digits of the fraction
    int tempC_frac =  float(tempC_float - tempC_int) * 100;
    //Get the tenths digit
    uint8_t tengthsDigit = tempC_frac / 10;
    //Get the hundreths digit
    uint8_t hundrethsDigit = tempC_frac % 10;
    //Round the tenths digit
    if(hundrethsDigit >= 5)
    {
        tengthsDigit++;
    }
    result[TENTHS_NUMBER_INDEX] = tengthsDigit;
}

static void updateTemperatureBuffer()
{
    convertTempFToTempC(uiEditTankTemperatureF, uiEditTankTemperatureC, UITEMPCBUFFER);
    sprintf(editTempBuffer,"%d.%dC %dF", uiEditTankTemperatureC[WHOLE_NUMBER_INDEX], uiEditTankTemperatureC[TENTHS_NUMBER_INDEX], uiEditTankTemperatureF);
}

void SystemManager::stateEditTankTemperature()
{
    
    if(!editTankTempInitialized)
    {
        uiEditTankTemperatureF = NvParmManager::getNvParm(PARMINDEX_TANK_SETPOINT_F).u.float_parm;
        updateTemperatureBuffer();
        myUI->Screen->showMessageNow(&editTankTemperatureMsg);
        editTankTempInitialized = true;
    }        
  
    if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs == FIRST_VALUE_HOLD_TIME_MS  && uiEditTankTemperatureF < MAX_TANK_SETPOINT_F)
    {
        uiEditTankTemperatureF++;
        updateTemperatureBuffer();
        myUI->Screen->showMessageNow(&editTankTemperatureMsg);
    }
    else if(holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEditTankTemperatureF + FAST_VALUE_DELTA_TEMP_F <= MAX_TANK_SETPOINT_F)
    {
        uiEditTankTemperatureF += FAST_VALUE_DELTA_TEMP_F;
        updateTemperatureBuffer();
        myUI->Screen->showMessageNow(&editTankTemperatureMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs == FIRST_VALUE_HOLD_TIME_MS && uiEditTankTemperatureF > MIN_TANK_TEMP_NORMAL_OPERATION_F)
    {
        uiEditTankTemperatureF--;
        updateTemperatureBuffer();
        myUI->Screen->showMessageNow(&editTankTemperatureMsg);
    }
    else if(holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= FAST_VALUE_HOLD_TIME_MS && holdTouchTimeMs % FAST_VALUE_UPDATE_SPEED_MS == 0 && uiEditTankTemperatureF - FAST_VALUE_DELTA_TEMP_F >= MIN_TANK_TEMP_NORMAL_OPERATION_F)
    {
        uiEditTankTemperatureF -= FAST_VALUE_DELTA_TEMP_F;
        updateTemperatureBuffer();
        myUI->Screen->showMessageNow(&editTankTemperatureMsg);
    }
    else if(releasedTouchValue == TOUCH_ACCEPT)
    {   
        NvParm temperatureParm;
        temperatureParm.u.float_parm = uiEditTankTemperatureF;
        NvParmManager::setNvParm(PARMINDEX_TANK_SETPOINT_F, temperatureParm);
        myTank->setTankSetpointF(uiEditTankTemperatureF);
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);
        myUI->Screen->resetScreen();        
        changeState(MENU_BROWSE);
        editTankTempInitialized = false;
        return;
    }
    else if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();        
        changeState(MENU_BROWSE);
        editTankTempInitialized = false;
        return;
    }
    if(!myUI->Screen->isBusy())
    {
        myUI->Screen->addMessageToQueue(&editTankTemperatureMsg);
    }    
}