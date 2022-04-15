/* ************************************************************************** */
/** Descriptive File Name

  @Company
 Electrolux

  @File Name
    CommonScreenMessages.h

  @Summary
    Contains all the constant messages for the screen.

  @Description
    See summary
 */
/* ************************************************************************** */

#ifndef _COMMON_MESSAGES_H    /* Guard against multiple inclusion */
#define _COMMON_MESSAGES_H

#include "ScreenMessages.h"
#include "version.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif    
    const ScreenMessages GrindmasterMsg("B SERIES", "CONTROLS");
    const ScreenMessages VersionMsg("Version", VERSION_STR);
    const ScreenMessages EngineeringBuildMsg("ENGINEERING", "BUILD");
    const ScreenMessages UnknownModelMsg("MODEL UNKNOWN", "DEFAULTING B-6");
    const ScreenMessages AutomaticmodeMsg("RELIABILITY","TEST MODE");
    const ScreenMessages FillWaterTankMsg("PLEASE WAIT","FILLING TANK");
    const ScreenMessages SingleHeadDispensingMsg("MACHINE", "DISPENSING");
    const ScreenMessages SingleHeadCancelMsg("DISPENSE", "CANCELED");
    const ScreenMessages RightHeadDispensingMsg("RIGHT SIDE", "DISPENSING");
    const ScreenMessages RightHeadCancelMsg("RIGHT DISPENSE", "CANCELED");
    const ScreenMessages RightHeadReadyMsg("RIGHT SIDE", "READY");
    const ScreenMessages SingleHeadReadyMsg("MACHINE", "READY");
    const ScreenMessages LeftHeadCancelMsg("LEFT DISPENSE", "CANCELED");
    const ScreenMessages LeftHeadDispensingMsg("LEFT SIDE", "DISPENSING");
    const ScreenMessages LeftHeadReadyMsg("LEFT SIDE", "READY");
    const ScreenMessages TankErrorsIgnoredMsg("TANK", "ERRORS IGNORED");
    const ScreenMessages TankFillTimeoutMsg("TANK", "FILL ERROR");
    const ScreenMessages TankThermistorErrorMsg("TANK", "THERMISTOR ERROR");
    const ScreenMessages TankHeatErrorMsg("TANK", "HEAT ERROR");
    const ScreenMessages TouchControlCommErrorMsg("TOUCH ERROR", "COMM FAILED");
    const ScreenMessages TouchControlTouchErrorMsg("TOUCH ERROR", "TESTING FAILED");
    const ScreenMessages EditDateMsg("EDIT", "DATE", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages EditTimeMsg("EDIT", "TIME", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages EditTimeFormatMsg("EDIT", "TIME FORMAT", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages LowTempNoBrewSettingsMsg("LOW TEMP NO BREW", "SETTING", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages EditTankTempMsg("EDIT", "TANK TEMP", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages EditEnergySavingsMsg("EDIT", "ENERGY SETTINGS", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages EditCoffeeRecipesMsg("EDIT", "COFFEE RECIPES", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages TeachModeMsg("TEACH", "MODE", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages ShowBrewCountMsg("VIEW", "BREW COUNTS", DEFAULT_STATIC_MENU_PRINT_TIME);
    const ScreenMessages TouchSetupMsg("SENSOR SETUP", "DO NOT TOUCH UI");
    const ScreenMessages ProgramLockoutMsg("PROGRAM LOCKOUT", "ENABLED");
    const ScreenMessages DispenseLockoutMsgI("CANNOT ENTER", "MENUS");
    const ScreenMessages DispenseLockoutMsgII("DISPENSING IN", "PROGRESS");
    const ScreenMessages ResetToDefaultMsg("RESET", "SYSTEM PARMS");
    const ScreenMessages AutomaticTestModeMsg("AUTOMATIC", "TEST MODE");
    const ScreenMessages ViewTemperaturesMsg("VIEW", "TEMPERATURES");
    const ScreenMessages ViewWaterLevelMsg("VIEW", "WATER LEVELS");
    const ScreenMessages MenuBrewDripoutMsg("EDIT", "DRIPOUT TIME");
    const ScreenMessages BatteryWarningMsg("BATTERY", "LOW OR MISSING");
    
    const ScreenMessages ControlCoffeeOutputMsg("COFFEE", "OUTPUTS");
    const ScreenMessages ControlTeaOuputsMsg("TEA", "OUTPUTS");
    const ScreenMessages ControlTankOutputsMsg("TANK", "OUTPUTS");
    const ScreenMessages ControlLEDMsg("LED", "OUTPUTS");
    const ScreenMessages ReadTankInputMsg("TANK", "INPUTS");
    const ScreenMessages ReadReedInputMsg("REED SWITCH", "INPUTS");
    const ScreenMessages ReadDipSwitchMsg("DIP SWITCH", "INPUTS");
    const ScreenMessages ReadProgramSwitchMsg("ON BOARD", "INPUTS");
    const ScreenMessages TouchSensorTestMsg("TOUCH", "TEST");
    const ScreenMessages NVRAMTestMsg("SPI FLASH", "TEST");
    const ScreenMessages AllOutputsOnTest("ALL OUTPUTS", "ON TEST");
    
    const ScreenMessages PreError4TempsMsg("VIEW", "PREERR2 TEMPS");
    const ScreenMessages PostError4TempsMsg("VIEW", "POSTERR2 TEMPS");
    const ScreenMessages WaterLevelControlFaultMsg("WATER LEVEL", "CONTROL FAULT");
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
