/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Electrolux

  @File Name
    debugFunctions.h

  @Summary
    Compilation of files for diagnosing problems with the system.
 */
/* ************************************************************************** */

#ifndef _DEBUGFUNCTIONS_H    /* Guard against multiple inclusion */
#define _DEBUGFUNCTIONS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "atmel_start.h"
#include "sprintfoverride.h"
#include "gm_SystemPinMap.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TASKS 20
#define MIN_STACK_SIZE_WORDS 10
#define TEA_IO_TICK_RATE_MS 50
#define TURNOFF_TIME_MS 1000
void verifyCPUClockSpeed(void);
void trapOnWatchdogReset(void);
void monitorTasks(void);
void TeaIOTest(int tick);
/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _DEBUGFUNCTIONS_H */

/* *****************************************************************************
 End of File
 */
