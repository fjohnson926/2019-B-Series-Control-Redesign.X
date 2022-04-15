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

#ifndef _MENUCONFIG_H    /* Guard against multiple inclusion */
#define _MENUCONFIG_H
#include "SystemManager.h"
#include "ScreenMessages.h"

#ifdef __cplusplus
extern "C" {
#endif

    
#define NUMBER_OF_MENUS 16
    typedef struct MENU_INFORMATION
    {
        systemState_t goToState;
        const ScreenMessages * MenuLabel;
    }menuItem_t;

    
#ifdef __cplusplus
}
#endif

#endif /* _MENUCONFIG_H */

/* *****************************************************************************
 End of File
 */
