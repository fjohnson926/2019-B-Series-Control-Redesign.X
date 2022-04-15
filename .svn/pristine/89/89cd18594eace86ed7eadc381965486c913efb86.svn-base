/* 
 * File:   ScreenMessages.h
 * Author: JTurner
 *
 * Created on April 23, 2019, 1:38 PM
 */

#ifndef SCREENMESSAGES_H
#define	SCREENMESSAGES_H

#define DEFAULT_STATIC_PRINT_TIME_MS 3000
#define DEFAULT_STATIC_MENU_PRINT_TIME 3000 //0xFFFFFFFF

typedef enum MESSAGE_TYPE
{
    STATIC_MSG,
    SCROLLING_MSG,
    ACTIVE_MSG            
}messageType_t;

typedef enum JUSTIFICATION
{
    RIGHT,
    CENTER,
    LEFT           
}textJustification_t;

class ScreenMessages {
public:
    ScreenMessages();
    ScreenMessages(const char * topMsg_p, const char * bottomMsg_p);
    ScreenMessages(const char * topMsg_p, const char * bottomMsg_p, int screenPrintTimeMs);
    ScreenMessages & operator=(const ScreenMessages & source);
    messageType_t messageType;
    const char * topString_p;
    textJustification_t topJustification;
    const char * bottomString_p;
    textJustification_t bottomJustification;
    int priorityLevel;
    bool shownOnScreen;
    bool valid;
    int screenTimeMs;
};

#endif	/* SCREENMESSAGES_H */

