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

#ifndef _BLOBS_H    /* Guard against multiple inclusion */
#define _BLOBS_H


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
    
#include "SPIFlash.h"
    
    //Blob Information
    #define PARMS_READ_INDEX 0
    #define PARMS_BANK_A_ADDRESS 0xFF0000
    #define PARMS_BANK_B_ADDRESS 0xFF1000
    #define PARMS_SIGNATURE 0x00000009

    #define COFFEE_RECIPE_BLOB_INDEX 1
    #define COFFEE_RECIPE_BANK_A_ADDRESS 0xFF2000
    #define COFFEE_RECIPE_BANK_B_ADDRESS 0xFF3000
    #define COFFEE_RECIPE_SIGNATURE 0x00000002
    
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _BLOBS_H */

/* *****************************************************************************
 End of File
 */
