/*
 * NvParmManager.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: thall
 */

#include "NvParmManager.h"

//Storage for NV Parms
NvParm NvParmManager::parms[PARM_SZ];
bool NvParmManager::resetParm[PARM_SZ];
NvParm NvParmManager::parmDefaults[PARM_SZ];
bool NvParmManager::init = false;

/**
 * Get NV Parm
 */
NvParm NvParmManager::getNvParm(eNvParm parm)
{
  //If not initialized, then do an init before read
  if(!init)
  {
    NvParmManager::osInit(NULL);
  }
  //Simple retrieve from in-memory cache
  if(parm < PARM_SZ)
  {
    return parms[parm];
  }
  else //GACK! Invalid parm. Just BARF!
  {
    __asm__("bkpt 0");    
  }
}

/**
 * Get NV Parm, indexed into a parameter array block
 */
NvParm NvParmManager::getNvParm(eNvParm parm, uint32_t index)
{
  //If not initialized, then do an init before read
  if(!init)
  {
    NvParmManager::osInit(NULL);
  }

  //Simple retrieve from in-memory cache
  if((parm + index) < PARM_SZ)
  {
    return parms[parm + index];
  }
  else //GACK! Invalid parm. Just BARF!
  {
    __asm__("bkpt 0");    
  }
}

/**
 * Set NV Parm, passing an unsigned int
 */
eRc NvParmManager::setNvParm(eNvParm parm, unsigned int value)
{
  //Trap any bad, bad, bad parm indexes
  if(parm >= PARM_SZ)
  {
    __asm__("bkpt 0");    
  }

  //Only process if new value different than old
  if(value != parms[parm].u.unsigned_parm)
  {
    //Can copy like types at bit level
    parms[parm].u.unsigned_parm = value;    
  }

  return RC_OK;
}

/**
 * Set NV Parm, unsigned int, indexed
 */
eRc NvParmManager::setNvParm(eNvParm parm, uint32_t index, unsigned int value)
{
  //Pass along to lower level routine
  setNvParm(parm + index, value);
}

/**
 * Set NV Parm
 */
eRc NvParmManager::setNvParm(eNvParm parm, NvParm value)
{
  //Trap any bad, bad, bad parm indexes
  if(parm >= PARM_SZ)
  {
    __asm__("bkpt 0");    
  }

  //Only process if new value different than old
  if(value.u.unsigned_parm != parms[parm].u.unsigned_parm)
  {
    //Can copy like types at bit level
    parms[parm].u.unsigned_parm = value.u.unsigned_parm;
  }

  return RC_OK;
}

/**
 * Set NV Parm, indexed into a parameter array block
 */
eRc NvParmManager::setNvParm(eNvParm parm, uint32_t index, NvParm value)
{
  //Call lower level routine
  setNvParm(parm + index, value);
}


/**
 * Update logical NV PARMS with data read from SPI Flash.
 *
 * NOTE: Is done BEFORE nearly all otehr osInit() calls to insure the
 *        parms are available for use by them.
 */
void NvParmManager::osInit(void* parm_p)
{
  //If already initialized, can skip
  if(init)
  {
    return;
  }

  //Will be initialized shortly
  init = true;
}

/**
 * C and C++ accessible function
 * Set parameters to logical default values.
 * NOTE: Unless NV Storage has failed, at a minimum the
 *        the non-resettable values must be restored.
 */
void initializeParmTables__c()
{
    for(int i = 0; i < PARM_SZ; i++)
    {
        NvParmManager::parms[i] = NvParmManager::parmDefaults[i];
    }
}

void resetParmsToDefaultValues__c()
{
    for(int i = 0; i < PARM_SZ; i++)
    {
        if(NvParmManager::resetParm[i])
        {
            NvParmManager::parms[i] = NvParmManager::parmDefaults[i];
        }
    }
}

void NvParmManager::clearResetOnDefaultParm(eNvParm parm)
{
    if(parm < PARM_SZ)
    {
        resetParm[parm] = false;
    }
}

//Place parm into parm table, by index
eRc NvParmManager::mapParm(eNvParm parm, NvParm defaultValue)
{
  //Trap any bad, bad, bad parm indexes
  if(parm >= PARM_SZ)
  {
    __asm__("bkpt 0");    
  }
  parmDefaults[parm] = defaultValue;
  resetParm[parm] = true;
}

//Place an int parm into parm table, by index
eRc NvParmManager::mapParm(eNvParm parm, int defaultValue)
{
  //Trap any bad, bad, bad parm indexes
  if(parm >= PARM_SZ)
  {
    __asm__("bkpt 0");    
  }
  parms[parm].parmType = UNIT_INTEGER;
  parms[parm].u.integer_parm = defaultValue;
  parmDefaults[parm].parmType = UNIT_INTEGER;
  parmDefaults[parm].u.integer_parm = defaultValue;
  resetParm[parm] = true;
}

//Place a float parm into parm table, by index
eRc NvParmManager::mapParm(eNvParm parm, float defaultValue)
{
  //Trap any bad, bad, bad parm indexes
  if(parm >= PARM_SZ)
  {
    __asm__("bkpt 0");    
  }
  parms[parm].parmType = UNIT_FLOAT;
  parms[parm].u.float_parm = defaultValue;
  parmDefaults[parm].parmType = UNIT_FLOAT;
  parmDefaults[parm].u.float_parm = defaultValue;
  resetParm[parm] = true;
}

//Place a bool parm into parm table, by index
eRc NvParmManager::mapParm(eNvParm parm, bool defaultValue)
{
  //Trap any bad, bad, bad parm indexes
  if(parm >= PARM_SZ)
  {
    __asm__("bkpt 0");    
  }
  parms[parm].parmType = UNIT_BOOL;
  parms[parm].u.bool_parm = defaultValue;
  parmDefaults[parm].parmType = UNIT_BOOL;
  parmDefaults[parm].u.bool_parm = defaultValue;
  resetParm[parm] = true;
}

 uint32_t * NvParmManager::getParmTableAddress(void)
{
    return (uint32_t *)parms;
}

//Check to see if the parms have been initialized
bool NvParmManager::hasParmsBeenInit(void)
{
  return init;
}

uint32_t NvParmManager::getParmTableSizeInBytes()
{
    uint32_t sizeInBytes = sizeof(parms);
    int test = sizeof(NvParm);
    return sizeInBytes;
}

