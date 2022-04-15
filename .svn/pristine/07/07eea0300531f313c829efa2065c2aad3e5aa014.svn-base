/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "StringManipulation.h"
#include "String.h"



int copyStringToBuffer(char * buffer, int maxBufferSize, const char * source,  int loadSourceIntoBufferIndex,  int sourceSize)
{
    int charactersCopied = 0;
    if(buffer && source)
    {   
        int writeIndex = loadSourceIntoBufferIndex;        
        int readIndex = 0;
        while(writeIndex < maxBufferSize && charactersCopied < sourceSize)
        {
            buffer[writeIndex] = source[readIndex];
            readIndex++;
            charactersCopied++;
            writeIndex++;
        }        
    }
    return charactersCopied;
}

void initializeString(char * buffer, int maxBufferSize, char value)
{
    for(int i = 0; i < maxBufferSize; i++)
    {
        buffer[i] = value;
    }
}

void padHeadOfString(char * buffer, int maxBufferSize, char value, int size)
{
    if(!buffer)
    {
        return;
    }
    for(int i = 0; i < size; i++)
    {
        if(i >= maxBufferSize)
        {
            break;
        }
        buffer[i] = value;
    }
}

void padTailOfString(char * buffer, int maxBufferSize, char value, int size)
{
    if(!buffer)
    {
        return;
    }
    for(int i = maxBufferSize; i > maxBufferSize-size; i--)
    {
        buffer[i] = value;
    }
}

int getOffsetFromLeftForCenterJustification(const char * buffer, int stringSize, int displaySize)
{
    if(stringSize >= displaySize)
    {
        return 0;
    }
    else
    {
        return (displaySize-stringSize)/2;
    }
}

int copyPartOfString(char * buffer, int maxBufferSize, char * source, int bufferStartIndex,int sourceStartIndex, int length)
{
    int writeCount = 0;
    if (!buffer || !source)
    {
        return 0;
    }
    int bufferWriteIndex = bufferStartIndex;
    int sourceReadIndex = sourceStartIndex;
    if (bufferWriteIndex+length > maxBufferSize)
    {
        return 0;
    }
    for(int i = 0; i < length; i++ )
    {
        buffer[bufferWriteIndex] = source[sourceReadIndex];
        bufferWriteIndex++;
        sourceReadIndex++;
        writeCount++;
    }
    return writeCount;            
}

int copyPartOfStringBySourceIndex(char * buffer, const char * source, int bufferStartIndex, int sourceStartIndex, int sourceEndIndex)
{
    int writeCount = 0;
    if (!buffer || !source)
    {
        return 0;
    }
    int bufferWriteIndex = bufferStartIndex;
    int sourceReadIndex = sourceStartIndex;

    for(int i = sourceStartIndex; i < sourceEndIndex; i++ )
    {
        buffer[bufferWriteIndex] = source[sourceReadIndex];
        bufferWriteIndex++;
        sourceReadIndex++;
        writeCount++;
    }
    return writeCount;   
}

void printDateStandardFormat(char * buffer,uint8_t month, uint8_t day, int year)
{
    int writeIndex = 0;
    writeIndex += u32ToString(&buffer[writeIndex],month);
    buffer[writeIndex] = '/';
    writeIndex++;
    writeIndex += u32ToString(&buffer[writeIndex],day);
    buffer[writeIndex] = '/';
    writeIndex++;
    writeIndex += u32ToString(&buffer[writeIndex], year);
    buffer[writeIndex] = 0;
}

void printTimeStandardFormat(char * buffer, uint8_t hour24hr, uint8_t minute, bool print24HourFormat)
{
    uint8_t hour = hour24hr;
    bool isPM;
    int writeIndex = 0;
    if(!print24HourFormat)
    {
        if(hour24hr == 0)
        {
            hour = 12;            
        }
        else if(hour24hr > 12  && hour24hr <= 23)
        {
            hour = hour24hr-12;
        }
        if(hour24hr <= 11)
        {
            isPM = false;
        }
        else
        {
            isPM = true;
        }        
    }
    writeIndex += u32ToString(&buffer[writeIndex],hour);
    buffer[writeIndex] = ':';
    writeIndex++;
    writeIndex += u32ToString(&buffer[writeIndex],minute);
    buffer[writeIndex] = ' ';
    writeIndex++;
    if(!print24HourFormat)
    {
        if(isPM)
        {
            buffer[writeIndex] = 'P';
        }
        else
        {
            buffer[writeIndex] = 'A';
        }
        writeIndex++;
        buffer[writeIndex] = 'M';
        writeIndex++;
    }
    buffer[writeIndex] = 0;
}

int u32ToString(char * buffer, int value)
{
    int storedValue = value;
    int testdigit = 1000000000;
    uint8_t writeIndex = 0;
    int testNumber = 1;
    uint8_t maxDigits = 10;
    uint8_t numberOfDigits = 10;
    if(value < 10)
    {
        buffer[writeIndex] = '0';
        writeIndex++;
        buffer[writeIndex] = value + '0';
        writeIndex++;
        return writeIndex;
    }
    
    //values > 9;
    for(int i = 0; i < maxDigits; i++)
    {        
        if(value/testdigit)
        {
            break;
        }
        testdigit /= 10;
        numberOfDigits--;
    }
    for(int i = 0; i < numberOfDigits-1; i++)
    {
        testNumber *= 10; 
    }    
    for(int i = 0; i < numberOfDigits; i++)
    {        
        buffer[writeIndex] = storedValue/testNumber + '0';
        storedValue = storedValue%testNumber;
        testNumber /= 10;
        writeIndex++;        
    }
    return writeIndex;    
}

bool sameString(char * string1, char* string2)
{
    if(strlen(string1) != strlen(string2))
    {
        return false;
    }
    
    for(int i = 0; i < strlen(string1); i++)
    {
        if(string1[i] != string2[i])
        {
            return false;
        }
    }    
    return true;    
}
