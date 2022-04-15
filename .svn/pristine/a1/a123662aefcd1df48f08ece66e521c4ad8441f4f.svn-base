/*
 * CoffeeBeverage.cpp
 *
 *  Created on: Sep 5, 2018
 *      Author: JTurner
 */

#include <CoffeeBeverage.h>
#include "CoffeeBeverageDefaults.h"

CoffeeBeverage::CoffeeBeverage() {
	// TODO Auto-generated constructor stub

}

CoffeeBeverage::CoffeeBeverage(const char * flavorName, int flavorNameSize, const CoffeeSizeAttributes_t * recipeData)
{
  assert(flavorNameSize <= NUMBER_OF_CHARACTERS_PER_RECIPE);
  memcpy(name,flavorName,flavorNameSize);
  int arraySize = sizeof(size);
  memcpy(size,recipeData,arraySize);
}

CoffeeBeverage & CoffeeBeverage::operator=(const CoffeeBeverage & source)
{
    for(int i = 0; i < strlen(source.name); i++)
    {
        this->name[i] = source.name[i];
    }
    for(int i = 0; i < NUMBER_OF_SIZES; i++)
    {
        for (int j = 0; j < NUMBER_OF_PULSES; j++)
        {
            this->size[i].pulseOnTimeSeconds[j] = source.size[i].pulseOnTimeSeconds[j];
            this->size[i].pulseOffTimeSeconds[j] = source.size[i].pulseOffTimeSeconds[j];
        }
    }
}

uint8_t CoffeeBeverage::getPulseCount(uint8_t sizeIndex)
{
    uint8_t count = 1;
    if (sizeIndex >= NUMBER_OF_SIZES)
    {
        return count;
    }
    while(count < NUMBER_OF_PULSES)
    {
        if(size[sizeIndex].pulseOnTimeSeconds[count] == 0)
        {
            break;
        }
        count++;
    }
    return count;    
}

