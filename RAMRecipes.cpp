#include "RAMRecipes.h"
#include "sprintfoverride.h"
#include "SystemManager.h"

extern SystemManager System;

CoffeeBeverage CoffeeBeverages[TOTAL_COFFEE_RECIPES];

const CoffeeSizeAttributes_t testContainer[NUMBER_OF_SIZES] =
{  
  {
    {36,27,27,24,21,21,21,0}, //Pulse On
    {11,8,8,8,7,7,7,0}, //Pulse Off
  },
};


CoffeeBeverage * getCoffeeBeveragePointer(int index)
{
    if(index < TOTAL_COFFEE_RECIPES)
    {
        return &CoffeeBeverages[index];
    }
    else
    {
        return NULL;
    }
}

uint32_t getCoffeeBeverageTableSizeInBytes()
{
    return sizeof(CoffeeBeverages[0])*TOTAL_COFFEE_RECIPES;
}

uint32_t * getCoffeeBeverageTableAddress()
{
    return (uint32_t *)CoffeeBeverages;
}

void setCoffeeBeverageRecipesToDefault()
{
    const int recipeNameSize = 10;
    char coffeeRecipeName[recipeNameSize];
    for(int i = 0; i < TOTAL_COFFEE_RECIPES; i++)
    {
        
        sprintf(coffeeRecipeName, "Recipe%d",i);
        if(System.machineFeature->primaryContainer == CONTAINER_DECANTER)
        {
            CoffeeBeverage coffee(coffeeRecipeName,recipeNameSize,defaultCoffeeDecanterContainer);
            memcpy((void*)&CoffeeBeverages[i], (void*)&coffee, sizeof(coffee));
        }
        else if(System.machineFeature->primaryContainer == CONTAINER_2P2L_AIRPOT)
        {
            CoffeeBeverage coffee(coffeeRecipeName,recipeNameSize,defaultCoffeeAirPotContainer);
            memcpy((void*)&CoffeeBeverages[i], (void*)&coffee, sizeof(coffee));
        }
        else if(System.machineFeature->primaryContainer == CONTAINER_2P5L_AIRPOT)
        {
            CoffeeBeverage coffee(coffeeRecipeName,recipeNameSize,defaultCoffeeGravityPotContainer);
            memcpy((void*)&CoffeeBeverages[i], (void*)&coffee, sizeof(coffee));
        }
        else if(System.machineFeature->primaryContainer == CONTAINER_3GAL_TEA)
        {
            CoffeeBeverage coffee(coffeeRecipeName,recipeNameSize,defaultCoffeeDecanterContainer);
            memcpy((void*)&CoffeeBeverages[i], (void*)&coffee, sizeof(coffee));
        }     
    }
}