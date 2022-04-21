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

#ifndef _RAMRECIPES_H    /* Guard against multiple inclusion */
#define _RAMRECIPES_H

#include "atmel_start.h"
#include "CoffeeBeverage.h"
#include "CoffeeBeverageDefaults.h"
#include "MachineConfiguration.h"

#define TOTAL_COFFEE_RECIPES 6 

#define RECIPE_ONE_INDEX 0  //FJ add for new recipes
#define RECIPE_TWO_INDEX 1  //FJ add for new recipes
#define RECIPE_THREE_INDEX 2 //FJ added
#define RECIPE_FOUR_INDEX 3 //FJ added 
#define RECIPE_FIVE_INDEX 4 //FJ added 
#define RECIPE_SIX_INDEX 5 //FJ added 

#define RIGHT_SIDE_RECIPE_INDEX 3 
#define LEFT_SIDE_RECIPE_INDEX 0  


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    CoffeeBeverage * getCoffeeBeveragePointer(int index);
    uint32_t * getCoffeeBeverageTableAddress();
    void getCoffeeBeveragesArrayFromNVRAM();
    void saveCoffeeBeveragesArrayToNVRAM();
    uint32_t getCoffeeBeverageTableSizeInBytes();
    void setCoffeeBeverageRecipesToDefault();
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _RAMRECIPES_H */

/* *****************************************************************************
 End of File
 */
