/* 
 * File:   EnergySavingsManager.h
 * Author: JTurner
 *
 * Created on September 9, 2019, 12:12 PM
 */

#ifndef ENERGYSAVINGSMANAGER_H
#define	ENERGYSAVINGSMANAGER_H

#include "atmel_start.h"

#define MIN_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS 60
#define MAX_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS 300
#define DEFAULT_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS 300

class EnergySavingsManager 
{
public:
    EnergySavingsManager();
    void clearCounter();
    uint8_t getWakeupHr();
    uint8_t getWakeupMin();
    void incrementCounter(int value);
    bool isEnergySavingsEnabled();
    bool isAutoWakeupEnabled();
    bool setAutoWakeUpEnable(bool value);
    void setAutoWakeupHr(uint8_t hour);
    void setAutoWakeupMin(uint8_t minute);    
    bool setEnergySavingsEnable(bool value);
    void setIdleTimeBeforeSleep(int minutes);
    bool timeToSleep();
    bool timeToWakeUp(uint8_t currentHour, uint8_t currentMinute);    
private:
    bool energySavingsEnabled;
    bool autoWakeupEnabled;
    int sleepCounterMs;
    int idleTimeMinutes;
    uint8_t autoWakeupHr;
    uint8_t autoWakeupMin;
};

#endif	/* ENERGYSAVINGSMANAGER_H */

