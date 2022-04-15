/* 
 * File:   Calendar.h
 * Author: JTurner
 *
 * Created on June 11, 2019, 10:38 AM
 */

#ifndef CALENDAR_H
#define	CALENDAR_H
#include "hal_calendar.h"
#include "hpl_calendar.h"
#include <utils_list.h>

extern struct calendar_descriptor CALENDAR_0;
#ifdef __cplusplus
extern "C"{
#endif    

#ifdef __cplusplus
}
#endif    

#ifdef __cplusplus


class Calendar {
public:
    Calendar();
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getMonth();
    uint8_t getDay();
    int getYear();
    bool isAM();
    bool is24TimeFormatEnabled();
    void setTimeFormat(bool enableFormat24Hr);
    void setDate(uint8_t month, uint8_t day, int year);
    void setTime(uint8_t hour, uint8_t minute);    
private:
    bool timeFormat24HourEnable;
};
#endif
#endif	/* CALENDAR_H */

