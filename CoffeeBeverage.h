/*
 * CoffeeBeverage.h
 *
 *  Created on: Sep 5, 2018
 *      Author: JTurner
 *      CoffeeBeverage is the basic storage unit for a given recipe
 */

#ifndef SOURCE_COFFEEBEVERAGE_H_
#define SOURCE_COFFEEBEVERAGE_H_

#include <stdint.h>
#include <string.h>
#include <assert.h>

#define NUMBER_OF_CHARACTERS_PER_RECIPE 20
#define NUMBER_OF_PULSES 8
#define NUMBER_OF_SIZES 3 //FJ changed to 3


typedef struct COFFEE_SIZE_ATTRIBUTES
{
    uint8_t pulseOnTimeSeconds[NUMBER_OF_PULSES];
    uint8_t pulseOffTimeSeconds[NUMBER_OF_PULSES];
	uint8_t padding[2];
}CoffeeSizeAttributes_t;

#if defined __cplusplus
class CoffeeBeverage {
public:
	CoffeeBeverage();
    CoffeeBeverage & operator=(const CoffeeBeverage & source);
	CoffeeBeverage(const char * flavorName, int size, const CoffeeSizeAttributes_t * recipeData);
    uint8_t getPulseCount(uint8_t size = 0);    //Change from 0 to 2
	char name[NUMBER_OF_CHARACTERS_PER_RECIPE];
	CoffeeSizeAttributes_t size[NUMBER_OF_SIZES];    
};

//Should only have one recipe to modify.
uint8_t getPulseCount(CoffeeBeverage * beverage, uint8_t size = 0); // change from 0 to 2

#endif

#endif /* SOURCE_COFFEEBEVERAGE_H_ */
