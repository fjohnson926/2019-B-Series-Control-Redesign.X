/*
 * CoffeeBeverageDefaults.h
 *
 *  Created on: Oct 28, 2022
 *      Author: JTurner/FJohnson
 */

#ifndef SOURCE_COFFEEBEVERAGEDEFAULTS_H_
#define SOURCE_COFFEEBEVERAGEDEFAULTS_H_
//1.9L ~= 64 oz
const CoffeeSizeAttributes_t defaultCoffeeDecanterContainer[NUMBER_OF_SIZES] =
{  
  {
    {120,0,0,0,0,0,0,0}, //Pulse On
    {0,0,0,0,0,0,0,0}, //Pulse Off
  },
};

//2.2L ~= 74oz
const CoffeeSizeAttributes_t defaultCoffeeAirPotContainer[NUMBER_OF_SIZES] =
{
  {
    {130,0,0,0,0,0,0,0}, //Pulse On
    {0,0,0,0,0,0,0,0}, //Pulse Off
  },
};

//2.5L ~= 84oz
const CoffeeSizeAttributes_t defaultCoffeeGravityPotContainer[NUMBER_OF_SIZES] =
{
  {
    {150,0,0,0,0,0,0,0}, //Pulse On
    {0,0,0,0,0,0,0,0}, //Pulse Off
  },
};

#endif /* SOURCE_COFFEEBEVERAGEDEFAULTS_H_ */
