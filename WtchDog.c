
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "rtos_port.h"
#include "WtchDog.h" 

uint8_t         WtchDogEnable;
WDogFlags_t     WtchDogStcIdx;

void WDogInit       ( void ){
    
    WtchDogEnable       = false;
    WtchDogStcIdx.raw   = STC_ARRAY_RES_MASK;
    
    WtchDog_init(WATCH_DOG_RESET_MAX_MS);
}

void WtchDog_enable ( uint8_t enable ){
    WtchDogEnable = (enable != 0);
}

void WDogSetFlg    ( WDogTaskIdx_e taskIdx ){
    switch(taskIdx){
        case TASK_ADC_CONVERS:
        case TASK_OS_SCREEN_MNG:
        case TASK_TOUCH_CHIP:
        case TASK_FILL_TANK_IDX:       
        case TASK_TEMP_TANK_IDX:
        case TASK_SYS_MANAGER:    
        case TASK_RGH_DISPENSER:   
        case TASK_LFT_DISPENSER:
        case TASK_TEA_DISPENSER:
        case TASK_WARM_0:
        case TASK_WARM_1:
        case TASK_WARM_2:
        case TASK_WARM_3:
        case TASK_WARM_4:
        case TASK_WARM_5:
            WtchDogStcIdx.raw |= taskIdx;
        break;
        
        default:
            #if __DEBUG
                asm("bkpt");    
            #endif
        break;
    }
}

void WDogSysRes ( void ){
    NVIC_SystemReset();
}

void WDogProc       ( void ){
    if(!WtchDogEnable){
        WtchDog_Reset();
    }
    else if ( (WtchDogStcIdx.raw & STC_ARRAY_MASK_VAL) == STC_ARRAY_MASK_VAL ){       
        WtchDog_Reset();
        WtchDogStcIdx.raw &= STC_ARRAY_RES_MASK;
    }
}

void WtchDog_Reset ( void ){
    while (WDT->SYNCBUSY.reg);
    WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
}

void WtchDog_init( uint32_t maxPeriodMS ){
    uint32_t cycles;
    uint8_t bits;
    
    if ((maxPeriodMS >= 16000) || !maxPeriodMS) {
        cycles = 16384;
        bits = 0xB;
    } 
    else {
        cycles = (maxPeriodMS * 1024L + 500) / 1000; // ms -> WDT cycles
        if (cycles >= 8192)         bits = 0xA;
        else if (cycles >= 4096)    bits = 0x9;
        else if (cycles >= 2048)    bits = 0x8;
        else if (cycles >= 1024)    bits = 0x7;
        else if (cycles >= 512)     bits = 0x6;
        else if (cycles >= 256)     bits = 0x5;
        else if (cycles >= 128)     bits = 0x4;
        else if (cycles >= 64)      bits = 0x3;
        else if (cycles >= 32)      bits = 0x2;
        else if (cycles >= 16)      bits = 0x1;
        else                        bits = 0x0;
    }
    
    // One-time initialization of watchdog timer.
    OSC32KCTRL->OSCULP32K.reg = 0x00;                       // Disable all
    OSC32KCTRL->OSCULP32K.reg = OSC32KCTRL_OSCULP32K_EN1K;  // Enable 1K
    
    WDT->CTRLA.reg = 0x00; // Disable watchdog for config
    while (WDT->SYNCBUSY.reg);
    
    WDT->INTENCLR.reg   = 1;                // Disable early warning interrupt
    WDT->CONFIG.reg     = (0x0F & bits);    // Set period for chip reset
    while (WDT->SYNCBUSY.reg);              // Sync CTRL write
    
    WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;   // clear watchDog counter
    WDT->CTRLA.reg = WDT_CTRLA_ENABLE;      // Start watchdog now!
    while (WDT->SYNCBUSY.reg);
}