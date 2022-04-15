#include "Arrays.h"

double averageDouble(double * data, int numberOfValues)
{
    double average = 0;
    for(int i = 0; i < numberOfValues; i++)
    {
        average += data[i];
    }
    average /= numberOfValues;
    return average; 
}

void copyDoubleArray(double * destination, double * source, int numberOfValues)
{
    for(int i = 0; i < numberOfValues; i++)
    {
        destination[i] = source[i];
    }
}

void initializeDoubleArray(double * data, double value, int numberOfValues)
{
    for(int i = 0; i < numberOfValues; i++)
    {
        data[i] = value;
    }
}

void initializeIntArray(int * data, int value, int numberOfValues)
{
    for(int i = 0; i < numberOfValues; i++)
    {
        data[i] = value;
    }
}

uint16_t averageU16(uint16_t * data, int numberOfValues)
{
    uint16_t average = 0;
    for(int i = 0; i < numberOfValues; i++)
    {
        average += data[i];
    }
    average /= numberOfValues;
    return average; 
}

void initializeU16Array(uint16_t * data, uint16_t value, int numberOfValues)
{
    for(int i = 0; i < numberOfValues; i++)
    {
        data[i] = value;
    }
}

uint16_t getHighestFrequencyValue(uint16_t * array, int size)
{
    uint16_t frequencyTable[size];
    initializeU16Array(frequencyTable, 0, size);
    uint16_t amplitudeTable[size];
    initializeU16Array(amplitudeTable, 0, size);
    //i = index of stored values
    //a = write index of amplitude/frequency table
    int a = 0;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(amplitudeTable[j] == array[i])
            {
                frequencyTable[j]++;
                break;
            }
            if(j == size-1)
            {
                amplitudeTable[a] = array[i];
                frequencyTable[a] = 1;
                a++;
            }
        }
    }
    //return value of the highest frequent value;
    int highestFrequencyIndex = 0;    
    for(int i = 0; i < size; i++)
    {
        if(frequencyTable[i] > frequencyTable[highestFrequencyIndex])
        {
            highestFrequencyIndex = i;
        }        
    }
    return amplitudeTable[highestFrequencyIndex];
}