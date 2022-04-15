
/* 
 * File:   TankManager.cpp
 * Author: JTurner
 * 
 * Created on May 15, 2019, 9:15 AM
 */

#include "TankManager.h"
#include "rtos_port.h"
#include "WtchDog.h"

//Static initializations
TankManager * TankManager::TankManagers[MAX_NUMBER_OF_TANKS];
int TankManager::TankManagersWriteIndex = 0;

TankManager::TankManager(uCPin inletValvePin, uCPin heaterRelayPin)
{
  preErrorTempWriteIndex = 0;
  postErrotTempWriteIndex = 0;
  currentTemperatureC = 96.1111;
  currentTemperatureF = getTemperatureF(currentTemperatureC);
  fillTaskCreationFailed = false;
  heatTaskCreationFailed = false;
  INITIALIZE_TEMPS(storedTempsInADCCounts, INIT_TEMP_ARRAY_VALUE, MAX_TEMP_STORE_COUNT);
  tankSetpointF = 195.0;
  faultTemperatureF = 0.0;
  waterStatus = WATER_OK;
  manualOutputControl = false;
  doesTankNeedHeat = false;
  hasTankBeenFilledOnce = false;
  isWaterLevelADCValuesFilled = false;
  fillTimeoutCount = 0;  
  overTemperatureCount = 0;
  pulseHeatCount = 0;
  waterLevelState = STARTUP_STATE;
  heatWhileFillCount = 0;
  heaterTimeoutCount = 0;
  inletValveClosedCount = 0;
  myInletValvePin = inletValvePin;
  SET_PIN_FOR_GPIO_OUTPUT(myInletValvePin);    
  myHeaterRelayPin = heaterRelayPin;
  SET_PIN_FOR_GPIO_OUTPUT(myHeaterRelayPin);  
  
  //By default, lower probe is the start heating probe, and the higher probe is the fill probe
  fillProbeADCArray = waterProbes[WATER_LEVEL_HIGH_PROBE_INDEX].adcValues;
  heatProbeADCArray = waterProbes[WATER_LEVEL_LOW_PROBE_INDEX].adcValues;
  lowerProbeADCArray = heatProbeADCArray;
  higherProbeADCArray = fillProbeADCArray;
  fillCompleteEvent = false;
  overFillProbeADCArray = NULL;  
  heatEnabled = true;
  fillEnabled = true;
  pulseHeatAllowed = PULSE_HEAT_ENABLE; 
  storedTempsWriteIndex = 0;
  temperatureADCArrayLoaded = false;
  turnElementOff();
  closeInletValve();
  //Keeps track of the locations of each TankManager. Used for C calls
  if(TankManagersWriteIndex == 0)
  {
      initializeTankManagersArray();
  }
  TankManagers[TankManagersWriteIndex] = this;
  TankManagersWriteIndex++;
    #if __DEBUG
    //Make sure array doesn't overflow
    if(TankManagersWriteIndex > MAX_NUMBER_OF_TANKS)
    {
        asm("bkpt");
    }
    #endif
  
    if(xTaskCreate(Manage_Fill_Task, "FillTask", TASK_FILL_VALVE_MANAGE_STACK_SIZE, this, TASK_TANK_MANAGE_PRIORITY, NULL) != pdPASS) 
    {	
        #if __DEBUG
            asm("bkpt");
        #endif
    }
  
    if(xTaskCreate(Manage_Temps_And_Heat_Task, "HeatTask", TASK_HEAT_TEMP_MANAGE_STACK_SIZE, this, TASK_TANK_MANAGE_PRIORITY, NULL) != pdPASS) 
    {
        #if __DEBUG
            asm("bkpt");
        #endif
        heatTaskCreationFailed = true;
    }
}

void TankManager::initializeTankManagersArray()
{
    for(int i = 0; i < MAX_NUMBER_OF_TANKS; i++)
    {
        TankManagers[i] = NULL;
    }
}

void TankManager::turnElementOn()
{
    SET_GPIO_HIGH(myHeaterRelayPin);
}
void TankManager::turnElementOff()
{
    SET_GPIO_LOW(myHeaterRelayPin);
}

void TankManager::openInletValve()
{
    SET_GPIO_HIGH(myInletValvePin);
}

bool TankManager::isFilling()
{
    return GET_GPIO_PIN_LEVEL(myInletValvePin);
}

bool TankManager::isHeating()
{
    return GET_GPIO_PIN_LEVEL(myHeaterRelayPin);
}

void TankManager::closeInletValve()
{
    SET_GPIO_LOW(myInletValvePin);
}

void TankManager::enableManualOutputControl()
{
    manualOutputControl = true;
    closeInletValve();
    turnElementOff();
}

void TankManager::disableManualOutputControl()
{
    manualOutputControl = false;
}

void TankManager::Manage_Fill_Task(void* parm_p)
{
    TankManager * tank = (TankManager *)parm_p;    
    while(1)
    {   
        if (tank->isFilling())                     
        {
            tank->inletValveClosedCount = 0;
        }
        else if(tank->inletValveClosedCount + TANK_TASK_RATE_MS < MAX_WATER_TIMER_T_VALUE)
        {
            tank->inletValveClosedCount += TANK_TASK_RATE_MS;
        }
         
        if(!tank->fillEnabled)
        {
            if(!tank->manualOutputControl)
            {
                tank->closeInletValve();
            }
            os_sleep(TANK_TASK_RATE_MS);
            continue;
        }
        if(tank->isOverflowDetected() && tank->isWaterLevelADCValuesFilled)
        {
            tank->waterStatus = WATER_ER1;                
        }
        if(tank->waterStatus == WATER_OK)
        {

            if(tank->waterLevelState == STARTUP_STATE)
            {
              tank->closeInletValve();              
              if(tank->isWaterLevelADCValuesFilled && tank->temperatureADCArrayLoaded)
              {
                    tank->waterLevelState = FILLED; //Assumption              
              }          
            }
            else if (tank->tankNeedsWater())
            {
              if(tank->hasTankBeenFilledOnce && tank->heatWhileFillCount < TIME_TURN_OFF_HEATER_DELAY_MS)
              {
                tank->heatWhileFillCount+=TANK_TASK_RATE_MS;
                tank->waterLevelState = REFILL_ALLOW_HEAT;
              }
              //if fill probe is not the heating probe and heat probe satisfied, turn on heater
              else if(tank->fillProbeADCArray != tank->heatProbeADCArray && tank->waterLevelHighEnoughForHeat())
              {
                  tank->waterLevelState = REFILL_ALLOW_HEAT;
              }
              else
              {
                tank->waterLevelState = FILL_NO_HEAT;
              }
              tank->openInletValve();
              tank->fillTimeoutCount+=TANK_TASK_RATE_MS;
              if(tank->fillTimeoutCount >= TIME_WATER_OVERFLOW_DEFAULT_MS)
              {
                tank->waterStatus = WATER_ER1;
                tank->fillTimeoutCount = TIME_WATER_OVERFLOW_DEFAULT_MS + 1;
                tank->closeInletValve();
              }
            }
            else
            {                
                tank->closeInletValve();
                tank->fillTimeoutCount = 0;
                tank->heatWhileFillCount = 0;
                if(tank->isWaterLevelADCValuesFilled)
                {
                    tank->hasTankBeenFilledOnce = true;
                }
                if(tank->waterLevelState != FILLED)
                {
                    tank->fillCompleteEvent = true;
                }
                tank->waterLevelState = FILLED;
            }
        }
        else
        {
            tank->closeInletValve();
        }
        
        WDogSetFlg(TASK_FILL_TANK_IDX);
        
        //This sets the speed of the task. Do not remove.
        os_sleep(TANK_TASK_RATE_MS);
    }
}

bool TankManager::lowProbeWaterDetected(void)
{
    if(getAverageFillProbeADCValue() >= WATER_FILL_LEVEL_LOW)
    {
        return false;
    }
    return true;
}

bool TankManager::highProbeWaterDetected(void)
{
    if(getAverageHeatWaterLevelProbeADCValue() >= WATER_FILL_LEVEL_LOW)
    {
        return false;
    }
    return true;
}

bool TankManager::initialFill()
{
    return !hasTankBeenFilledOnce;
}

bool TankManager::tankNeedsWater()
{
   //Based on the signal, water is needed
  if(getAverageFillProbeADCValue() >= WATER_FILL_LEVEL_LOW)
  {
    //Start filling immediately 
    if (!hasTankBeenFilledOnce)
    {
      return true;
    }
    continueFillingAfterProbeSatisfiedCount = 0;
    //Wait some time before filling
    beginFillingCount+=TANK_TASK_RATE_MS;
    if(beginFillingCount >= TIME_WATER_FILL_DELAY_MS)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  //Based on signal, don't need water
  else
  {
    beginFillingCount = 0;
    continueFillingAfterProbeSatisfiedCount+=TANK_TASK_RATE_MS;
    if(continueFillingAfterProbeSatisfiedCount <= TIME_CONT_FILL_AFTER_SEE_WATER_MS &&
      hasTankBeenFilledOnce)
    {
      return true;
    }
    else
    {
      //Reset counter while not filling anymore
      continueFillingAfterProbeSatisfiedCount = TIME_CONT_FILL_AFTER_SEE_WATER_MS + 1;
      return false;
    }
  }
  return false;
}

bool TankManager::waterLevelHighEnoughForHeat()
{
    if(getAverageHeatWaterLevelProbeADCValue() < WATER_FILL_LEVEL_LOW)
    {
        return true;
    }
    return false;
}

void TankManager::Manage_Temps_And_Heat_Task(void* parm_p)
{
    TankManager * tank = (TankManager *)parm_p;
    while(1)
    {
        tank->updateTemperatureVariables();
        tank->determineHeatMode();
        tank->manageHeater();        
        
        WDogSetFlg(TASK_TEMP_TANK_IDX);
        os_sleep(TANK_TASK_RATE_MS/portTICK_PERIOD_MS);
    }
}

void TankManager::determineHeatMode()
{
  savePreError4Temperature(currentTemperatureF);
  savePostError4Temperature(currentTemperatureF);
  
  if(currentTemperatureF <= LOWER_TEMP_LIMIT || currentTemperatureF >= UPPER_TEMP_LIMIT)
  {
    waterStatus = WATER_ER2;
    heatingStatus = NO_HEAT;
    doesTankNeedHeat = false;
    if(faultTemperatureF < 1.0)
    {
        faultTemperatureF = currentTemperatureF;
    }
    return;
  }
  if(currentTemperatureF >= tankSetpointF + OUTER_HYSTERSIS_OFF_F ||
    waterLevelState == FILL_NO_HEAT)
  {
    heatingStatus = NO_HEAT;
    doesTankNeedHeat = false;
  }
  else if ((isTankPulseHeatTempRange() && doesTankNeedHeat) && pulseHeatAllowed)
  {
    heatingStatus = PULSE_HEAT;
    doesTankNeedHeat = true;
  }
  else if(currentTemperatureF <  tankSetpointF - OUTER_HYSTERSIS_ON_F)
  {
    heatingStatus = FULL_HEAT;
    doesTankNeedHeat = true;
  }
}

bool TankManager::isTankPulseHeatTempRange()
{
  uint32_t TpulseF = tankSetpointF + OUTER_HYSTERSIS_OFF_F - PULSE_HEAT_TEMP_BEFORE_SETPOINT_F;
  uint32_t Tnow = currentTemperatureF;
  if (Tnow > TpulseF)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void TankManager::manageHeater()
{
    if(waterStatus != WATER_OK || !heatEnabled)
    {
        if(!manualOutputControl)
        {
            turnElementOff();
        }
        return;
    }
    if(heatEnabled)
    {
      if (heatingStatus == NO_HEAT ||
          heatingStatus == WAIT_FOR_HEAT_TOKEN)
      {
        turnElementOff();
        pulseHeatCount = 0;      
      }
      else if(heatingStatus == PULSE_HEAT)
      {
        pulseHeatCount++;
        if (pulseHeatCount <= (PULSE_HEAT_PERIOD_MS*PULSE_HEAT_DUTY_CYCLE)/100)
        {
          turnElementOn();
        }
        else if(pulseHeatCount > PULSE_HEAT_PERIOD_MS*PULSE_HEAT_DUTY_CYCLE/100 &&
        pulseHeatCount < PULSE_HEAT_PERIOD_MS)
        {
          turnElementOff();
        }
        else
        {
          pulseHeatCount = 0;
          doesTankNeedHeat = true;
        }    
      }
      else if(heatingStatus == FULL_HEAT)
      {
        turnElementOn();
        pulseHeatCount = 0;
        doesTankNeedHeat = true;
      }
    }
}

adc_values_t TankManager::getAverageFillProbeADCValue()
{
    volatile uint16_t avgADCValue = GET_AVERAGE_ADC_VALUE(fillProbeADCArray, MAX_WATER_LEVEL_ADC_STORE_COUNT);
    return avgADCValue;
}

adc_values_t TankManager::getAverageHeatWaterLevelProbeADCValue()
{
    volatile uint16_t avgADCValue = GET_AVERAGE_ADC_VALUE(heatProbeADCArray, MAX_WATER_LEVEL_ADC_STORE_COUNT);
    return avgADCValue;
}

adc_values_t TankManager::getAverageTempInADC()
{    
    volatile uint16_t avgADCValue = GET_AVERAGE_TEMP_VALUE(storedTempsInADCCounts, MAX_TEMP_STORE_COUNT);
    return avgADCValue;
}

void TankManager::saveTempAsADCValue(adc_values_t value)
{
    storedTempsInADCCounts[storedTempsWriteIndex] = value;
    storedTempsWriteIndex++;
    savedTempCount++;
    if(storedTempsWriteIndex >= MAX_TEMP_STORE_COUNT)
    {
        storedTempsWriteIndex = 0;        
    }
    if(!temperatureADCArrayLoaded)
    {
        if(savedTempCount > TEMP_COUNTS_BEFORE_STABLE)
        {
            temperatureADCArrayLoaded = true;
        }
    }
}

void  TankManager::saveWaterLevelValue(uint8_t probeIndex, adc_values_t value)
{
    if(probeIndex < MAX_NUMBER_OF_PROBES)
    {
        WaterProbe_t * probe = &(waterProbes[probeIndex]);
        probe->adcValues[probe->writeIndex] = value;
        probe->writeIndex++;
        if(probe->writeIndex >= MAX_WATER_LEVEL_ADC_STORE_COUNT)
        {
            probe->writeIndex = 0;
            isWaterLevelADCValuesFilled = true;
        }
    }   
}

void TankManager::updateTemperatureVariables()
{
    if(temperatureADCArrayLoaded)
    {        
        conditionTemperatureADCValue();        
        double resistance = getResistanceFromCount(conditionedADCValue);         
        currentTemperatureC = get61128TemperatureC(resistance);
        currentTemperatureF = getTemperatureF(currentTemperatureC);
        
        //Error 4 (Water Temperature not Heating Logic)
        if(     inletValveClosedCount < TIME_INLET_CLOSED_START_HEAT_TIMEOUT_COUNT ||
                heatingStatus == NO_HEAT || 
                heatingStatus == WAIT_FOR_HEAT_TOKEN 
            )
        {            
            heaterTimeoutCount = 0;
        }
        else if(heatingStatus == FULL_HEAT && waterStatus == WATER_OK)
        {           
            if(currentTemperatureF > temperatureToClearErrorFourF)
            {
                temperatureToClearErrorFourF = currentTemperatureF + HEATING_NEXT_GOAL_TEMP_F;
                heaterTimeoutCount = 0;
            }
            else
            {
                heaterTimeoutCount += TANK_TASK_RATE_MS;
                if(heaterTimeoutCount >= TIME_NO_HEAT_TIMEOUT_COUNT_MS)
                {
                    waterStatus = WATER_ER4;                    
                }
            }
        }
    }
}

bool TankManager::tankTemperatureGreaterEqualSetpoint()
{
    if(currentTemperatureF >= tankSetpointF)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TankManager::conditionTemperatureADCValue()
{
    double avgTemp = 0;
    for(int i = 0; i < MAX_TEMP_STORE_COUNT; i++)
    {
        avgTemp = avgTemp + storedTempsInADCCounts[i];        
    }
    avgTemp = avgTemp / MAX_TEMP_STORE_COUNT;    
    conditionedADCValue = avgTemp;    
}

bool TankManager::isOverflowDetected()
{
    if(overFillProbeADCArray == NULL && lowerProbeADCArray == NULL && higherProbeADCArray == NULL)
    {
        return false;
    }
    else if(lowerProbeADCArray != NULL && higherProbeADCArray != NULL && higherProbeADCArray != lowerProbeADCArray)
    {
        //Water detected adc ~= GND and no water detected adc ~= 2800
        int lowerProbeAvg = GET_AVERAGE_ADC_VALUE(lowerProbeADCArray, MAX_WATER_LEVEL_ADC_STORE_COUNT);
        int higherProbeAvg = GET_AVERAGE_ADC_VALUE(higherProbeADCArray, MAX_WATER_LEVEL_ADC_STORE_COUNT);
        if(higherProbeAvg <= WATER_FILL_LEVEL_LOW && lowerProbeAvg > WATER_FILL_LEVEL_LOW)
        {
            return true;
        }
        return false;
    }
    else
    {
        volatile uint16_t overfillAVGADCValue = GET_AVERAGE_ADC_VALUE(overFillProbeADCArray, MAX_WATER_LEVEL_ADC_STORE_COUNT);
        if(overfillAVGADCValue >= WATER_FILL_LEVEL_LOW)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void TankManager::setTankSetpointF(int setpointInF)
{
    if(setpointInF > MAX_TANK_SETPOINT_F)
    {
        tankSetpointF = MAX_TANK_SETPOINT_F;
    }
    else if(setpointInF < MIN_TANK_SETPOINT_F)
    {
        tankSetpointF = MIN_TANK_SETPOINT_F;
    }
    else
    {
        tankSetpointF = setpointInF;
    }
}

void TankManager::enableHeating(void)
{
    heatEnabled = true;
}

void TankManager::disableHeating(void)
{
    heatEnabled = false;
}

void TankManager::enableFilling(void)
{
    fillEnabled = true;
}

void TankManager::disableFilling(void)
{
    fillEnabled = false;
}

extern "C" void saveTempAsADCValue__c(int tankManagerCreatedNumber, adc_values_t value)
{
    if(tankManagerCreatedNumber >= MAX_NUMBER_OF_TANKS)
    {
        #if __DEBUG
            asm("bkpt");    
        #endif        
    }
    TankManager * tank = TankManager::TankManagers[tankManagerCreatedNumber];
    tank->saveTempAsADCValue(value);
}

extern "C" void saveWaterLevelAsADCValue__c(int tankManagerCreatedNumber, int probeIndex, adc_values_t value)
{
    if(tankManagerCreatedNumber >= MAX_NUMBER_OF_TANKS)
    {
        #if __DEBUG
        asm("bkpt");
        #endif        
    }
    TankManager * tank = TankManager::TankManagers[tankManagerCreatedNumber];
    tank->saveWaterLevelValue(probeIndex, value);
}

void TankManager::savePreError4Temperature(temperature_values_t temperature)
{
    if(waterStatus != WATER_ER2 && waterStatus == WATER_OK)
    {
        preErrorTempData[preErrorTempWriteIndex] = temperature;
        preErrorTempWriteIndex++;
        if(preErrorTempWriteIndex >= NUMBER_OF_PRE_ERROR_TEMPS)
        {
            preErrorTempWriteIndex = 0;
        }
    }
}

void TankManager::savePostError4Temperature(temperature_values_t temperature)
{
    if(waterStatus == WATER_ER2 && postErrotTempWriteIndex < NUMBER_OF_POST_ERROR_TEMPS)
    {
        postErrorTempData[postErrotTempWriteIndex] = temperature;
        postErrotTempWriteIndex++;
    }
}

bool TankManager::readTankFilledEventFlag(void)
{
    if(fillCompleteEvent)
    {
        fillCompleteEvent = false;
        return true;
    }
    return false;
}