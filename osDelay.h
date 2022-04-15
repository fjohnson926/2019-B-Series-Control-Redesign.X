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

#ifndef _OS_DELAY_H    /* Guard against multiple inclusion */
#define _OS_DELAY_H

#include "atmel_start.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    void osSupportedDelayMs(uint32_t delayMs);
    void osSupportedDelayUs(uint32_t delayUs);
    
#ifdef __cplusplus
}
#endif

#endif /* _OS_DELAY_H */

/* *****************************************************************************
 End of File
 */
