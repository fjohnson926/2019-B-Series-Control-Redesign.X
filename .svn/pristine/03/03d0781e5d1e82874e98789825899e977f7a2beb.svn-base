/* 
 * File:   TankManager.h
 * Author: JTurner
 *
 * Created on May 15, 2019, 9:15 AM
 */

#ifndef TANKMANAGER_H
#define	TANKMANAGER_H

#include "TankManagerHal.h"

#define MAX_WATER_LEVEL_ADC_STORE_COUNT 10
#define MAX_TEMP_STORE_COUNT 11
#define INIT_TEMP_ARRAY_VALUE 3525
#define ADC_OFFSET 22

/*The Tank manager object is responsible for managing the tanks.*/
typedef enum
{
  WATER_OK,
  WATER_NOT_OK,
  // The water full detection has failed and the fill valve has been on for
  //  at least X minutes.
  WATER_ER1,
  // OR WATER_ER3 - The thermistor id detected open, shorted OR
  //                 the water is boiling
  WATER_ER2,
  //Not used. Here to fill out the enum
  WATER_ER3,
  //The heater has been on for X minutes, but no rise in water
  // temperature detected.
  WATER_ER4,
  //Upper probe sees water, but lower does not
  WATER_ER5
} WaterStatus_t;

typedef enum
{
  REFILL_ALLOW_HEAT,
  FILL_NO_HEAT,
  FILLED,
  STARTUP_STATE,
}FillState_t;

typedef enum
{
  FULL_HEAT,
  PULSE_HEAT,
  NO_HEAT,
  WAIT_FOR_HEAT_TOKEN,
}HeatingState_t;

typedef struct PROBE_DATA
{
    uint16_t writeIndex;
    adc_values_t adcValues[MAX_WATER_LEVEL_ADC_STORE_COUNT];
}WaterProbe_t;

/* Don't look at declarations if a c file */
#ifdef __cplusplus

class TankManager {
public:
    TankManager(uCPin inletValvePin, uCPin heaterRelayPin);    
    static void Manage_Temps_And_Heat_Task(void * parm_p);
    static void Manage_Fill_Task(void * parm_p);
    
    bool fillTaskCreationFailed;
    bool heatTaskCreationFailed;
    float approximateWaterVolForDispensingML;
    
    adc_values_t getAverageFillProbeADCValue();
    adc_values_t getAverageHeatWaterLevelProbeADCValue();
    adc_values_t getAverageTempInADC();    
    
    void enableHeating(void);
    void disableHeating(void);
    void enableFilling(void);
    void disableFilling(void);
    
    bool lowProbeWaterDetected(void);
    bool highProbeWaterDetected(void);
    
    bool tankNeedsWater();
    bool initialFill();
    bool waterLevelHighEnoughForHeat();
    
    void updateTemperatureVariables();
    adc_values_t conditionedADCValue;    
    void conditionTemperatureADCValue();    
    void saveTempAsADCValue(adc_values_t value);
    bool tankTemperatureGreaterEqualSetpoint();
    
    
    void saveWaterLevelValue(uint8_t probeIndex, adc_values_t value);
    
    
    /*This array keeps track of all TankManager objects. Tank Managers
     are loaded starting at index 0 in the order they were created*/
    static TankManager * TankManagers[MAX_NUMBER_OF_TANKS];
    WaterStatus_t waterStatus;
    temperature_values_t currentTemperatureF;
    temperature_values_t faultTemperatureF;
    temperature_values_t tankSetpointF;
    void setTankSetpointF(int setpointInF);
    
    bool isHeating();
    bool isFilling();
    void enableManualOutputControl();
    void  disableManualOutputControl();
    
    void turnElementOn();
    void turnElementOff();
    void openInletValve();
    void closeInletValve();
    
    int preErrorTempWriteIndex;
    int preErrorTempData[NUMBER_OF_PRE_ERROR_TEMPS];
    int postErrotTempWriteIndex;
    int postErrorTempData[NUMBER_OF_POST_ERROR_TEMPS];
    
    bool hasTankBeenFilledOnce;
    
    bool readTankFilledEventFlag(void);
    bool isWaterLevelADCValuesFilled;
    bool temperatureADCArrayLoaded;
    
private:

    //Physical/Electrical PINs
    uCPin myInletValvePin;  
    uCPin myHeaterRelayPin;
    //Status variables  
    bool fillCompleteEvent;
    bool doesTankNeedHeat;    
    FillState_t waterLevelState;
    HeatingState_t heatingStatus;
    //Temperatures
    temperature_values_t currentTemperatureC;  
    temperature_values_t temperatureToClearErrorFourF;  
    uint32_t ADCCalculatedTemperatureFTimesTempScalar; //value from ADC -- averages    
    water_timer_t adcStabilizeCount;
    water_timer_t fillTimeoutCount;
    water_timer_t beginFillingCount;
    water_timer_t continueFillingAfterProbeSatisfiedCount;
    water_timer_t overTemperatureCount;
    water_timer_t pulseHeatCount;
    water_timer_t heatWhileFillCount;
    water_timer_t heaterTimeoutCount;
    water_timer_t inletValveClosedCount;
    int parameterIndex;
    bool heatEnabled;
    bool fillEnabled;
    /*There are two probe arrays that contain ADC information
     INDEX_01 contains ADC information for LVL on 348-00080
     INDEX_02 contains ADC information for LVL2 on 348-00080
     Assign an array to fill, heat, or overfill array*/
    WaterProbe_t waterProbes[MAX_NUMBER_OF_PROBES];
    adc_values_t * fillProbeADCArray;
    adc_values_t * heatProbeADCArray;
    adc_values_t * overFillProbeADCArray;
    adc_values_t * lowerProbeADCArray;
    adc_values_t * higherProbeADCArray;
    adc_values_t storedTempsInADCCounts[MAX_TEMP_STORE_COUNT];
    int storedTempsWriteIndex; 
    int savedTempCount;
    static int TankManagersWriteIndex;  
    void initializeTankManagersArray();    
    void determineHeatMode();
    void manageHeater();
    bool isTankPulseHeatTempRange();
    bool isOverflowDetected();    
    bool pulseHeatAllowed;
    bool manualOutputControl;

    void savePreError4Temperature(temperature_values_t temperature);
    void savePostError4Temperature(temperature_values_t temperature);
};
/* Don't look at declarations if a c file */
#endif

#ifdef __cplusplus
extern "C"{
#endif    
//Load in temperature and water level adc values using these functions
void saveTempAsADCValue__c(int tankManagerCreatedNumber, adc_values_t value);
void saveWaterLevelAsADCValue__c(int tankManagerCreatedNumber, int probeIndex, adc_values_t value);

#ifdef __cplusplus
}
#endif    
#endif	/* TANKMANAGER_H */

