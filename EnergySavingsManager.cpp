/* 
 * File:   EnergySavingsManager.cpp
 * Author: JTurner
 * 
 * Created on September 9, 2019, 12:12 PM
 */

#include "EnergySavingsManager.h"

EnergySavingsManager::EnergySavingsManager() 
{
    energySavingsEnabled = true;
    autoWakeupEnabled = true;
    sleepCounterMs = 0;
    idleTimeMinutes = 1;
    autoWakeupHr = 12;
    autoWakeupMin = 10;
}

void EnergySavingsManager::clearCounter()
{
    sleepCounterMs = 0;
}

uint8_t EnergySavingsManager::getWakeupHr()
{
    return autoWakeupHr;
}

uint8_t EnergySavingsManager::getWakeupMin()
{
    return autoWakeupMin;
}

bool EnergySavingsManager::isEnergySavingsEnabled()
{
    return energySavingsEnabled;
}

bool EnergySavingsManager::isAutoWakeupEnabled()
{
    return autoWakeupEnabled;
}


void EnergySavingsManager::incrementCounter(int value)
{
    if(sleepCounterMs + value <= 0xFFFFFFFF)
    {
        sleepCounterMs += value;
    }
}

bool EnergySavingsManager::setEnergySavingsEnable(bool value)
{
    energySavingsEnabled = value;
}

void EnergySavingsManager::setIdleTimeBeforeSleep(int minutes)
{
    if(minutes >= MIN_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS && minutes <= MAX_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS)
    {
        idleTimeMinutes = minutes;
    }
    else
    {
        idleTimeMinutes = DEFAULT_IDLE_TIME_MINS_BEFORE_ENERGY_SAVINGS;
    }
}

bool EnergySavingsManager::setAutoWakeUpEnable(bool value)
{
    autoWakeupEnabled = value;
}

void EnergySavingsManager::setAutoWakeupHr(uint8_t hour)
{
    if(hour < 24 && hour >= 0)
    {
        autoWakeupHr = hour;
    }
}

void EnergySavingsManager::setAutoWakeupMin(uint8_t minute)
{
    if(minute < 60 && minute >= 0)
    {
        autoWakeupMin = minute;
    }
}

bool EnergySavingsManager::timeToSleep()
{
    if(energySavingsEnabled && (sleepCounterMs >= idleTimeMinutes*60*1000))
    {
        return true;
    }
    return false;
}

bool EnergySavingsManager::timeToWakeUp(uint8_t currentHour, uint8_t currentMinute)
{
    if(energySavingsEnabled && autoWakeupEnabled && autoWakeupHr == currentHour && autoWakeupMin == currentMinute)
    {
        return true;
    }
    return false;
}