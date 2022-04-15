/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _STRINGMANIP_H    /* Guard against multiple inclusion */
#define _STRINGMANIP_H
#include "stdint.h"
#include "sprintfoverride.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    //returns copy count.
    int copyStringToBuffer(char * buffer, int maxBufferSize, const char * source,  int loadSourceIntoBufferIndex,  int sourceSize);
    int copyPartOfString(char * buffer, int maxBufferSize, const char * source, int bufferStartIndex,int sourceStartIndex, int length);
    int copyPartOfStringBySourceIndex(char * buffer, const char * source, int bufferStartIndex, int sourceStartIndex, int sourceEndIndex);
    void initializeString(char * buffer, int maxBufferSize, char value);
    void padHeadOfString(char * buffer, int maxBufferSize, char value, int size);
    void padTailOfString(char * buffer, int maxBufferSize, char value, int size);
    int u32ToString(char * buffer, int value);
    
    void printDateStandardFormat(char * buffer, uint8_t month, uint8_t day, int year);
    void printTimeStandardFormat(char * buffer, uint8_t hour, uint8_t minute, bool print24HourFormat);
    
    int getOffsetFromLeftForCenterJustification(const char * buffer, int stringSize, int displaySize);
    bool sameString(char * string1, char* string2);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _STRINGMANIP_H */

/* *****************************************************************************
 End of File
 */
