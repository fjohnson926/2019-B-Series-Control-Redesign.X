/*
 * NvParmManager.hpp
 *
 *  Created on: Aug 27, 2015
 *      Author: thall
 */

#ifndef NVPARMMANAGER_HPP_
#define NVPARMMANAGER_HPP_

#include "NvParmManagerHal.h"

typedef enum PARM_UNITS_ENUM
{
  UNIT_INTEGER,
  UNIT_UNSIGNED_INT,
  UNIT_PERCENT,         //Integer percent 0-100
  UNIT_FLOAT,
  UNIT_ENUM,            //Application specific enum
  UNIT_BITFIELDS,       //Application specific bitfields
  UNIT_BOOL,            //Boolean type
  UNIT_CHAR,            //Single character type
  UNIT_4CHARS,          //4 chars in 32 bits, MSB to LSB
  //UNIT_5CHARS,         // JTQ 4 chars plus a /0 termination   for passcode
  UNIT_8NIBS,           //8 Nibbles, such as 0-9, MSNibble to LSNibble

  UNITS_SZ
} eParmUnits;

//----- C accessible function(s) -----
#ifdef __cplusplus
  extern "C"
#endif
//Set parameters to logical default values
void initializeParmTables__c();
void resetParmsToDefaultValues__c();

//For C++ includes only
#ifdef __cplusplus

/**
 * Individual Non-volatile parameter.
 */
class NvParm
{
public:

  eParmUnits parmType;

  union U
  {
    int      integer_parm;
    unsigned unsigned_parm;
    unsigned int_percent_parm; //0-100 integer percent
    float    float_parm;
    unsigned enum_parm;        //Application defined
    unsigned bitfields_parm;   //Application defined bitfields
    bool     bool_parm;        //Boolean value
    //char     char5_parm;       // added JTQ
  } u;

  //Constructors
  NvParm()
  {
    parmType = UNIT_UNSIGNED_INT;
    u.unsigned_parm = 0;
  }
  NvParm(int value)
  {
    parmType = UNIT_INTEGER;
    u.integer_parm = value;
  }
  NvParm(float value)
  {
    parmType = UNIT_FLOAT;
    u.float_parm = value;
  }
  NvParm(eParmUnits units, unsigned value)
  {
    parmType = units;
    u.unsigned_parm = value;
  }
  NvParm(eParmUnits units, int value)
  {
    parmType = units;
    u.integer_parm = value;
  }
  NvParm(eParmUnits units, float value)
  {
    parmType = units;
    u.float_parm = value;
  }
};

/**
 * Non-Volatile Parameters class interface
 */

class NvParmManager
{
private:
  static bool init;

public:
  //Storage for NV Parms
  static NvParm parms[]; //Public for access by C routine
  //Default settings table for NVParms
  static NvParm parmDefaults[PARM_SZ];
  //Reset parm when set. Doesn't reset parm when clear
  static bool resetParm[];  
  static void clearResetOnDefaultParm(eNvParm parm);
  //Get NV Parm
  static NvParm getNvParm(eNvParm parm);
  //Get NV Parm, indexed into a parameter array block
  static NvParm getNvParm(eNvParm parm, uint32_t index);
  //Set NV Parm
  static eRc setNvParm(eNvParm parm, NvParm value);
  //Set NV Parm, indexed into a parameter array block
  static eRc setNvParm(eNvParm parm, uint32_t index, NvParm value);
  //Set NV Parm, unsigned int
  static eRc setNvParm(eNvParm parm, unsigned int value);
  //Set NV Parm, unsigned int, indexed
  static eRc setNvParm(eNvParm parm, uint32_t index, unsigned int value);
  //Interface to call on system start to init parms
  static void osInit(void* parm_p);
  //Check to see if the parms have been initilized
  static bool hasParmsBeenInit(void);
  //Map parm to table
  static eRc mapParm(eNvParm parm, NvParm defaultValue);
  //Map parm to table
  static eRc mapParm(eNvParm parm, int defaultValue);
  //Map a float parm to the NVParm table
  static eRc mapParm(eNvParm parm, float defaultValue);
  //Map a bool parm to the NVParm table
  static eRc mapParm(eNvParm parm, bool defaultValue);
  //Get parm table as a 32 bit pointer
  static uint32_t * getParmTableAddress(void);
  //Get the size of the parms table 
  static uint32_t getParmTableSizeInBytes();
};

//C++ only
#endif

#endif /* NVPARMMANAGER_HPP_ */
