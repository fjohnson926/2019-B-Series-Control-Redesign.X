/* 
 * File:   Calendar.cpp
 * Author: JTurner
 * 
 * Created on June 11, 2019, 10:38 AM
 */

#include "Calendar.h"

struct calendar_descriptor CALENDAR_0;

Calendar::Calendar() 
{
    
}

void Calendar::setTimeFormat(bool enableFormat24Hr)
{
    timeFormat24HourEnable = enableFormat24Hr;
}

bool Calendar::is24TimeFormatEnabled()
{
    return timeFormat24HourEnable;
}

uint8_t Calendar::getDay()
{
    struct calendar_date_time date_time;
    calendar_get_date_time(&CALENDAR_0, &date_time);
    return date_time.date.day;
}

uint8_t Calendar::getMonth()
{
    struct calendar_date_time date_time;
    calendar_get_date_time(&CALENDAR_0, &date_time);
    return date_time.date.month;
}

int Calendar::getYear()
{
    struct calendar_date_time date_time;
    calendar_get_date_time(&CALENDAR_0, &date_time);
    return date_time.date.year;
}

uint8_t Calendar::getHour()
{
    struct calendar_date_time date_time;
    calendar_get_date_time(&CALENDAR_0, &date_time);
    return date_time.time.hour;
}
uint8_t Calendar::getMinute()
{
    struct calendar_date_time date_time;
    calendar_get_date_time(&CALENDAR_0, &date_time);
    return date_time.time.min;
}

void Calendar::setDate(uint8_t month, uint8_t day, int year)
{
    struct calendar_date_time date_time;
    date_time.date.month = month;
    date_time.date.day = day;
    date_time.date.year = year;
    calendar_set_date(&CALENDAR_0,&date_time.date);
}

void Calendar::setTime(uint8_t hour, uint8_t minute)
{
    struct calendar_date_time date_time;
    date_time.time.hour = hour;
    date_time.time.min = minute;
    date_time.time.sec = 0;    
    calendar_set_time(&CALENDAR_0,&date_time.time);    
}

static struct calendar_alarm alarm;

static void alarm_cb(struct calendar_descriptor *const descr)
{
	
}


