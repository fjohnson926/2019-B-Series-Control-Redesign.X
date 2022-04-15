
#ifndef WTCHDOG_H
#define	WTCHDOG_H

#ifdef	__cplusplus
extern "C" {
#endif
    #include "stdint.h"

    #define WATCH_DOG_RESET_MAX_MS       ( 10000 )

    #define WDOG_MASK_VAL_NO_TASK   ( 0x00000000 )

    #define WDOG_MASK_VAL_SYSTMNG   ( 0x00000001 )
    #define WDOG_MASK_VAL_ADCCOMV   ( 0x00000002 )
    #define WDOG_MASK_VAL_SCREEN    ( 0x00000004 )
    #define WDOG_MASK_VAL_TOUCH     ( 0x00000008 )
    #define WDOG_MASK_VAL_FILLTANK  ( 0x00000010 )
    #define WDOG_MASK_VAL_TEMPTANK  ( 0x00000020 )
    #define WDOG_MASK_VAL_RGHDISP   ( 0x00000040 )
    #define WDOG_MASK_VAL_LFTDISP   ( 0x00000080 )

    #define WDOG_MASK_VAL_TEADISP   ( 0x00000100 )
    #define WDOG_MASK_VAL_WARMER0   ( 0x00000200 )
    #define WDOG_MASK_VAL_WARMER1   ( 0x00000400 )
    #define WDOG_MASK_VAL_WARMER2   ( 0x00000800 )
    #define WDOG_MASK_VAL_WARMER3   ( 0x00001000 )
    #define WDOG_MASK_VAL_WARMER4   ( 0x00002000 )
    #define WDOG_MASK_VAL_WARMER5   ( 0x00004000 )

    #define EXEC_ARRAY_RESET_MASK   ( 0x80000000 )
    #define STC_ARRAY_RES_MASK      ( 0x00000000 )
    #define STC_ARRAY_MASK_VAL      (   WDOG_MASK_VAL_SYSTMNG    \
                                    |	WDOG_MASK_VAL_ADCCOMV    \
                                    |	WDOG_MASK_VAL_SCREEN     \
                                    |	WDOG_MASK_VAL_TOUCH      \
                                    |	WDOG_MASK_VAL_FILLTANK   \
                                    |	WDOG_MASK_VAL_TEMPTANK   \
                                    |	WDOG_MASK_VAL_RGHDISP    \
                                    |	WDOG_MASK_VAL_LFTDISP    \
                                    |	WDOG_MASK_VAL_TEADISP    \
                                    |	WDOG_MASK_VAL_WARMER0    \
                                    |	WDOG_MASK_VAL_WARMER1    \
                                    |	WDOG_MASK_VAL_WARMER2    \
                                    |	WDOG_MASK_VAL_WARMER3    \
                                    |	WDOG_MASK_VAL_WARMER4    \
                                    |   WDOG_MASK_VAL_WARMER5    )

    typedef union{
        uint32_t raw;
        struct{
            uint32_t SystemMngrTask     :1;
            uint32_t AdcConvertTask     :1;
            uint32_t OperScreenTask     :1;
            uint32_t TouchChipTask      :1;
            uint32_t FillTankTask       :1;
            uint32_t TempTankTask       :1;
            uint32_t RghDispTask        :1;
            uint32_t LftDispTask        :1;

            uint32_t TeaDispTask        :1;
            uint32_t Warm0Task          :1;
            uint32_t Warm1Task          :1;
            uint32_t Warm2Task          :1;
            uint32_t Warm3Task          :1;
            uint32_t Warm4Task          :1;
            uint32_t Warm5Task          :1;
            uint32_t FreeB7             :1;
            
            uint32_t FreeC              :8;
            uint32_t freeD              :8;
        }fld;
    }WDogFlags_t;
    
    typedef enum {
	    TASK_NO_IDX         = WDOG_MASK_VAL_NO_TASK,
        TASK_SYS_MANAGER    = WDOG_MASK_VAL_SYSTMNG,
        TASK_ADC_CONVERS    = WDOG_MASK_VAL_ADCCOMV,
        TASK_OS_SCREEN_MNG  = WDOG_MASK_VAL_SCREEN,
        TASK_TOUCH_CHIP     = WDOG_MASK_VAL_TOUCH,
        TASK_FILL_TANK_IDX  = WDOG_MASK_VAL_FILLTANK,
        TASK_TEMP_TANK_IDX  = WDOG_MASK_VAL_TEMPTANK,
        TASK_RGH_DISPENSER  = WDOG_MASK_VAL_RGHDISP,
        TASK_LFT_DISPENSER  = WDOG_MASK_VAL_LFTDISP,
        TASK_TEA_DISPENSER  = WDOG_MASK_VAL_TEADISP,
        TASK_WARM_0         = WDOG_MASK_VAL_WARMER0,
        TASK_WARM_1         = WDOG_MASK_VAL_WARMER1,
        TASK_WARM_2         = WDOG_MASK_VAL_WARMER2,
        TASK_WARM_3         = WDOG_MASK_VAL_WARMER3,
        TASK_WARM_4         = WDOG_MASK_VAL_WARMER4,
        TASK_WARM_5         = WDOG_MASK_VAL_WARMER5,
    }WDogTaskIdx_e;

    void WDogProc       ( void );
    void WDogInit       ( void );
    void WDogSysRes  ( void );
    void WDogSetFlg     ( WDogTaskIdx_e taskIdx );
    
    void WtchDog_init   ( uint32_t maxPeriodMS );
    void WtchDog_enable ( uint8_t enable );
    void WtchDog_Reset  ( void );

    
#ifdef	__cplusplus
}
#endif

#endif	/* WTCHDOG_H */

