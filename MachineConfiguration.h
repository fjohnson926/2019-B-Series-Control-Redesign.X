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

#ifndef _MACHINE_CONFIG_H    /* Guard against multiple inclusion */
#define _MACHINE_CONFIG_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include "WarmerManager.h"
#include "UIManager.h"
#include "Led.h"

extern WarmerManager Warmers;
extern UIManager UI;

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
#define RELAY_HEATING 1
#define SS_RELAY 0
#define ONE_HEAD 1
#define TWO_HEADS 2    
#define WARMER1 1
#define WARMER2 1
#define WARMER3 1
#define WARMER4 1
#define WARMER5 1
#define WARMER6 1
#define NOWARMER0 0
#define NOWARMER1 0
#define NOWARMER2 0
#define NOWARMER3 0
#define NOWARMER4 0
#define NOWARMER5 0
#define SUGAR_PUMP 1
#define NO_SUGAR_PUMP 0
#define DILUTOR 1
#define NO_DILUTOR 0
    
#define NO_WARMER_AVAILABLE 0
    
    typedef enum _containerTypes_
    {
        CONTAINER_DECANTER,               
        CONTAINER_3GAL_TEA,
        CONTAINER_2P2L_AIRPOT,
        CONTAINER_2P5L_AIRPOT,
    }container_t;
    
    typedef enum _brewerTypes_
    {
        BREWER_TYPE_COFFEE,
        BREWER_TYPE_TEA,
        BREWER_TYPE_HOT_WATER,
        BREWER_TYPE_URN               
    }brewer_t;
    
    typedef struct _warmermappings_
    {
        Warmer * warmer_p;
        uint16_t touchValue;
        Led * LED_p;
        const char * upperLabel;
        const char * lowerLabel;
    }warmerConfig_t;
    
    typedef enum _tanktype_
    {
        TANK_TYPE_B6,
        TANK_TYPE_B3,                
    }TankType_t;
    
    typedef struct _machineconfig_
    {
        const char * modelInfo;        
        container_t primaryContainer;
        TankType_t tankType;
        brewer_t brewerType;
        int numberOfHeads;
        bool hasHeatRelay;
        warmerConfig_t mappedWarmer1;
        warmerConfig_t mappedWarmer2;
        warmerConfig_t mappedWarmer3;
        warmerConfig_t mappedWarmer4;
        warmerConfig_t mappedWarmer5;
        warmerConfig_t mappedWarmer6;
        bool hasSugarPump;
        bool hasDilutor;
    } machineConfig_t;

const machineConfig_t machineFeaturesB6 = 
                    {
                        "B-6",
                        CONTAINER_DECANTER,
                        TANK_TYPE_B6,
                        BREWER_TYPE_COFFEE,
                        TWO_HEADS,
                        RELAY_HEATING,
                        {Warmers.myWarmer1, (1<<CS12_BIT), UI.LEDs->warmer02Led, "UPPER REAR", "RIGHT WARMER"},
                        {Warmers.myWarmer2, (1<<CS10_BIT), UI.LEDs->warmer04Led, "UPPER FRONT","RIGHT WARMER"},
                        {Warmers.myWarmer3, (1<<CS09_BIT), UI.LEDs->warmer06Led, "LOWER FRONT", "RIGHT WARMER"},
                        {Warmers.myWarmer4, (1<<CS05_BIT), UI.LEDs->warmer01Led, "UPPER REAR","LEFT WARMER"},
                        {Warmers.myWarmer5, (1<<CS07_BIT), UI.LEDs->warmer03Led, "UPPER FRONT", "LEFT WARMER"},
                        {Warmers.myWarmer6, (1<<CS08_BIT), UI.LEDs->warmer05Led, "LOWER FRONT", "LEFT WARMER"},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };

const machineConfig_t machineFeaturesB3 = 
                    {
                        "B-3",
                        CONTAINER_DECANTER,
                        TANK_TYPE_B3,
                        BREWER_TYPE_COFFEE,
                        ONE_HEAD,
                        RELAY_HEATING,
                        {Warmers.myWarmer1, (1<<CS05_BIT), UI.LEDs->warmer01Led, "UPPER REAR", "WARMER"},
                        {Warmers.myWarmer2, (1<<CS07_BIT), UI.LEDs->warmer03Led, "UPPER FRONT", "WARMER"},
                        {Warmers.myWarmer3, (1<<CS08_BIT), UI.LEDs->warmer05Led, "LOWER FRONT", "WARMER"},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };

const machineConfig_t machineFeaturesBID = 
                    {
                        "B-ID",
                        CONTAINER_DECANTER,
                        TANK_TYPE_B3,
                        BREWER_TYPE_COFFEE,
                        ONE_HEAD,
                        RELAY_HEATING,
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };

const machineConfig_t machineFeaturesB3WL = 
                    {
                        "B-3WL",
                        CONTAINER_DECANTER,
                        TANK_TYPE_B3,
                        BREWER_TYPE_COFFEE,
                        ONE_HEAD,
                        RELAY_HEATING,
                        {Warmers.myWarmer1, (1<<CS05_BIT), UI.LEDs->warmer01Led, "UPPER REAR","LEFT WARMER"},
                        {Warmers.myWarmer2, (1<<CS07_BIT), UI.LEDs->warmer03Led, "LOWER FRONT", "LEFT WARMER"},
                        {Warmers.myWarmer3, (1<<CS10_BIT), UI.LEDs->warmer04Led, "LOWER FRONT", "RIGHT WARMER"},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };

const machineConfig_t machineFeaturesB3WR = 
                    {
                        "B-3WR",
                        CONTAINER_DECANTER,
                        TANK_TYPE_B3,
                        BREWER_TYPE_COFFEE,
                        ONE_HEAD,
                        RELAY_HEATING,
                        {Warmers.myWarmer1, (1<<CS12_BIT), UI.LEDs->warmer02Led, "UPPER REAR", "RIGHT WARMER"},
                        {Warmers.myWarmer2, (1<<CS10_BIT), UI.LEDs->warmer04Led, "LOWER FRONT","RIGHT WARMER"},
                        {Warmers.myWarmer3, (1<<CS07_BIT), UI.LEDs->warmer03Led, "LOWER FRONT", "LEFT WARMER"},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };

const machineConfig_t machineFeaturesBSAP = 
                    {
                        "B-SAP",
                        CONTAINER_2P2L_AIRPOT,
                        TANK_TYPE_B3,
                        BREWER_TYPE_COFFEE,
                        ONE_HEAD,
                        RELAY_HEATING,
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };

const machineConfig_t machineFeaturesBDAP = 
                    {
                        "B-DAP",
                        CONTAINER_2P2L_AIRPOT,
                        TANK_TYPE_B6,
                        BREWER_TYPE_COFFEE,
                        TWO_HEADS,
                        RELAY_HEATING,
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };


const machineConfig_t machineFeaturesBSGP = 
                    {
                        "B-SGP",
                        CONTAINER_2P5L_AIRPOT,
                        TANK_TYPE_B6,
                        BREWER_TYPE_COFFEE,
                        ONE_HEAD,
                        RELAY_HEATING,
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };

const machineConfig_t machineFeaturesBDGP = 
                    {
                        "B-DGP",
                        CONTAINER_2P5L_AIRPOT,
                        TANK_TYPE_B6,
                        BREWER_TYPE_COFFEE,
                        TWO_HEADS,
                        RELAY_HEATING,
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };




const machineConfig_t machineFeaturesNonSweetTea = 
                    {
                        "Tea No Sweet Pump",
                        CONTAINER_DECANTER,
                        TANK_TYPE_B6,
                        BREWER_TYPE_TEA,
                        ONE_HEAD,
                        RELAY_HEATING,
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        {NULL, 99, NULL, NULL, NULL},
                        NO_SUGAR_PUMP,
                        NO_DILUTOR
                    };

  


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MACHINE_CONFIG_H */

/* *****************************************************************************
 End of File
 */
