/* 
 * File:   ScreenMessages.cpp
 * Author: JTurner
 * 
 * Created on April 23, 2019, 1:38 PM
 */

#include "ScreenMessages.h"
#include "stddef.h"

ScreenMessages::ScreenMessages() {
    topString_p = NULL;
    bottomString_p = NULL;
    valid = false;
    screenTimeMs = DEFAULT_STATIC_PRINT_TIME_MS;   
}

ScreenMessages::ScreenMessages(const char * topMsg_p, const char * bottomMsg_p)
{
    topString_p = topMsg_p;
    bottomString_p = bottomMsg_p;
    topJustification = CENTER;
    bottomJustification = CENTER;
    screenTimeMs = DEFAULT_STATIC_PRINT_TIME_MS;
}

ScreenMessages::ScreenMessages(const char * topMsg_p, const char * bottomMsg_p, int screenPrintTimeMs)
{
    topString_p = topMsg_p;
    bottomString_p = bottomMsg_p;
    topJustification = CENTER;
    bottomJustification = CENTER;
    screenTimeMs = screenPrintTimeMs;
}

ScreenMessages & ScreenMessages::operator=(const ScreenMessages & source)
{
    topString_p = source.topString_p;
    bottomString_p = source.bottomString_p;
    valid = source.valid;
    screenTimeMs = source.screenTimeMs;    
}

