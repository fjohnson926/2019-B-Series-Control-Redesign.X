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

#ifndef _NVPARMMANAGERHAL_H    /* Guard against multiple inclusion */
#define _NVPARMMANAGERHAL_H

#include "atmel_start.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    #define PARM_SZ 15
    #define RC_OK 0
    //For generic retrieval. See parmType for actual type stored in the 32 bits of data.
    typedef uint32_t eNvParm;
    typedef uint32_t eRc;
    
    //Parms
    #define PARMINDEX_LOW_TEMP_NO_BREW 0
    #define PARMINDEX_TANK_SETPOINT_F 1
    #define PARM_INDEX_ENERGY_SAVINGS_ENABLE 2
    #define PARM_INDEX_ENERGY_SAVINGS_AUTO_EXIT_ENABLE 3    
    #define PARM_INDEX_ENERGY_SAVINGS_EXIT_HOUR 4
    #define PARM_INDEX_ENERGY_SAVINGS_EXIT_MINUTE 5    
    #define PARM_INDEX_ENERGY_SAVINGS_IDLE_MINS 6
    #define PARM_INDEX_24HR_TIME_FORMAT_ENABLE 7
    #define PARM_INDEX_RESETABLEBREWCNT_RIGHTSINGLE 8
    #define PARM_INDEX_RESETABLEBREWCNT_LEFT 9
    #define PARM_INDEX_NONRESETABLEBREWCNT_RIGHTSINGLE 10
    #define PARM_INDEX_NONRESETABLEBREWCNT_LEFT 11
    #define PARM_INDEX_MODEL_LAST_MODEL_SWITCH 12
    #define PARM_INDEX_BREW_LOCKOUT_TIME_S 13
    
    //Default Values
    #define PARMDEFAULT_LOW_TEMP_NO_BREW true
    #define PARMDEFAULT_TANK_SETPOINT_F 200.0
    #define PARMDEFAULT_ENERGY_SAVINGS_ENABLE true
    #define PARMDEFAULT_ENERGY_SAVINGS_AUTO_EXIT false    
    #define PARMDEFAULT_ENERGY_SAVINGS_EXIT_HOUR 8
    #define PARMDEFAULT_ENERGY_SAVINGS_EXIT_MINUTE 30
    #define PARMDEFAULT_IDLE_MINS 300
    #define PARMDEFAULT_24HR_TIME_FORMAT false
    #define PARMDEFAULT_MODEL_SWITCH_CHANGE 0
    #define PARMDEFAULT_BREW_LOCKOUT_TIME_S 120

    //Not really a default value
    #define PARMDEFAULT_BREW_COUNT 0
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _NVPARMMANAGERHAL_H */

/* *****************************************************************************
 End of File
 */
