#include "SystemManager.h"
#include "NvParmManager.h"
#include "sprintfoverride.h"

static char line1buffer[20];
static char line2buffer[20];
static ScreenMessages SPIFlashStatusMsg(line1buffer, line2buffer, 500);
static bool initialized = false;

static void updateLineBuffers(bool bank1pass, bool bank2pass)
{
    if(bank1pass)
    {
        sprintf(line1buffer,"Bank1 = PASS");  
    }
    else
    {
        sprintf(line1buffer,"Bank1 = FAILED");  
    }
    if(bank2pass)
    {
        sprintf(line2buffer,"Bank2 = PASS");  
    }
    else
    {
        sprintf(line2buffer,"Bank2 = FAILED");  
    }    
}

void SystemManager::stateFCTSPIFlashTest(void)
{
    static bool test1Passed = false;
    static bool test2Passed = false;
    
    if(!initialized)
    {
        initialized = true;       
        test1Passed = false;
        test2Passed = false;
        const bool lowTempNoBrew = false;
        const int tankSetpoint = 195;
        
        for(int i = 0; i < 2; i++)
        {
            bool readLowTempNoBrew = true;
            int readTankSetpoint = 190;
            initializeParmTables__c();
          
            NvParmManager::setNvParm(PARMINDEX_TANK_SETPOINT_F, tankSetpoint);    
            NvParmManager::setNvParm(PARMINDEX_LOW_TEMP_NO_BREW, lowTempNoBrew);             
            //Write to SPIFLASH
            NVBlobs->flushNvBlob(PARMS_READ_INDEX);
            //Read from SPIFLASH
            NVBlobs->loadNvBlob(PARMS_READ_INDEX);        

            readTankSetpoint = NvParmManager::getNvParm(PARMINDEX_TANK_SETPOINT_F).u.unsigned_parm;
            readLowTempNoBrew = NvParmManager::getNvParm(PARMINDEX_LOW_TEMP_NO_BREW).u.bool_parm;

            if(lowTempNoBrew == readLowTempNoBrew && tankSetpoint == readTankSetpoint)
            {
                if(i == 0)
                {
                    test1Passed = true;
                }
                else if(i == 1)
                {
                    test2Passed = true;
                }
            }
        }
        //Reset Parms to default
        initializeParmTables__c();
        NVBlobs->flushNvBlob(PARMS_READ_INDEX);        
        updateLineBuffers(test1Passed, test2Passed);
        myUI->Screen->showMessageNow(&SPIFlashStatusMsg);
    }
    
    if(releasedTouchValue == TOUCH_CANCEL)
    {
        myUI->Screen->resetScreen();
        initialized = false;
        changeState(FCT_MAIN_MENU);
        return;
    }
    
    if(!myUI->Screen->isBusy())
    {   
        updateLineBuffers(test1Passed, test2Passed);
        myUI->Screen->addMessageToQueue(&SPIFlashStatusMsg);
    }    
}