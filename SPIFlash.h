/* 
 * File:   SPIFlash.h
 * Author: JTurner
 *
 * Created on June 27, 2019, 10:21 AM
 */

#ifndef SPIFLASH_H
#define	SPIFLASH_H

#include "SPIFlashHal.h"

#define BANK_A true
#define BANK_B false

//Indicates completely erased
#define ERASED_STATE 0xFFFFFFFFu
#define ERASED_BYTE  0xFFu

/* Flash commands */
#define K_CMD_READ_STATUS1             	0x05
#define K_CMD_READ_STATUS2             	0x35
#define K_CMD_READ_STATUS3             	0x15
#define K_LENGTH_CMD_READ_STATUS        2
#define K_INDEX_RSP_READ_STATUS         1
#define K_BIT_BUSY_STATUS				0x01

#define K_CMD_READ_DATA                	0x03	// starts reading 24-bit address at location sent
/* include only the command and address, add with length of data to read */
#define K_LENGTH_CMD_READ_DATA        	5
#define K_INDEX_READ_DATA_START         4		// first data index read on receiveBuffer

#define K_CMD_PAGE_PROGRAM             	0x02
#define K_LENGTH_BYTE_WRITE_DATA        5		// this value can be changed depending on how many address needs to be written, can be up to a page of 256 + 4 cmd
#define K_INDEX_WRITE_DATA_START        4		// fill data to write on this index on sentBuffer

// 4K Erase
#define K_CMD_SECTOR_ERASE             	0x20
#define K_LENGTH_SECTOR_ERASE           4
// 32K Erase
#define K_CMD_HALF_BLOCK_ERASE         	0x52
//#define K_LENGTH_HALF_BLOCK_ERASE       4		// share with sector erase length
// 64K Erase
#define K_CMD_FULL_BLOCK_ERASE         	0xD8
//#define K_LENGTH_FULL_BLOCK_ERASE       4		// share with sector erase length

// Complete chip erase
#define K_CMD_CHIP_ERASE               	0x60
#define K_LENGTH_CHIP_ERASE             1

#define K_CMD_WRITE_ENABLE             	0x06
#define K_LENGTH_WRITE_ENABLE           1

#define K_CMD_WRITE_DISABLE            	0x04
#define K_LENGTH_WRITE_DISABLE          1


#define K_INDEX_CMD			0
#define K_INDEX_ADDR0		3
#define K_INDEX_ADDR1		2
#define K_INDEX_ADDR2		1
#define FLASH_SECTOR_SIZE	4096
#define SPI_FLASH_PAGE_SIZE   256

#define MAX_READ_BYTES 10
#define K_BUFFER_SIZE		(K_LENGTH_CMD_READ_DATA + MAX_READ_BYTES)


#define FLASHSPI_CFG_SIZE_SECTOR 0x1000

//Defines state of the nonvolatile storage
typedef enum NVSTATE_ENUM
{
  //Status unknown
  NVSTATE_UNKNOWN,

  //Nonvolatile storage completely erased
  NVSTATE_ERASED,
  
  //Both banks have data
  NVSTATE_TWO_COPIES,
  
  //The nonvolatile storage is fatally failed
  //NOTE: Should do extensive testing before setting
  //       this state, possibly over multiple resets?
  NVSTATE_FAILED,
  
  //The nonvolatile storage is in an invalid state
  NVSTATE_INVALID,

  //The nonvolatile storage is readable and writable
  NVSTATE_READY,
  
  //TODO: Older data format recognized
  NVSTATE_OLD_FORMAT,
  
  //Sizer
  NVSTATE_SZ
  
} eNvState;

typedef struct BLOB_ATTRIBUTES
{
    //Bank A Parms
    uint32_t A_bank;
    uint32_t B_bank;
    uint32_t blobSizeInBytes;
    uint32_t * pointerToRAMData = NULL;
    uint32_t activeSignature;
    uint32_t blob_write_count_low;
    uint32_t blob_write_count_high;
    bool writeToBank    = BANK_A;
    bool moreRecentBank = BANK_B;
    bool blob_dirty;
    eNvState blob_nvState;    
} blobAttribs_t;

class Blob
{
    public:
        Blob();
        Blob(uint32_t A_bank, uint32_t B_bank, uint32_t blobSizeInBytes, uint32_t * pointerToRamData, uint32_t activeSignature);
        Blob & operator=(const Blob & source);
        uint32_t A_bank;
        uint32_t B_bank;
        uint32_t blobSizeInBytes;
        uint32_t * pointerToRAMData;
        uint32_t activeSignature;
        uint32_t blob_write_count_low;
        uint32_t blob_write_count_high;
        bool writeToBank    = BANK_A;
        bool moreRecentBank = BANK_B;
        bool blob_dirty;
        eNvState blob_nvState;
        bool hasavalidbank;
    private:        

};

class SPIFlash {
public:
    SPIFlash();
    void addBlob(Blob blob);
    void initNVBlobs();
    bool flushNvBlob(int index);
    void loadNvBlob(int index);
    bool isAtLeastOneBankValid(int index);
private:
    int numberOfBlobs;
    int blobWriteIndex;
    void storeBlobInformation(Blob blob);
    void determineNVState();
    Blob allBlobs[NUMBER_OF_BLOBS];
    bool blobsInitialized;
    uint8_t sendBuffer[K_BUFFER_SIZE];
    uint8_t receiveBuffer[K_BUFFER_SIZE];
    void buildAddress(uint32_t addressOffset);
    parm_type readParm(uint32_t addr);
    uint8_t readByte(uint32_t addressOffset, uint8_t * buff);
    void eraseSector(uint32_t addressOffset, uint32_t length);
    status_t enableWrite(void);
    bool isBusy(void);
    bool isErased(uint32_t addr);
    void writeNvUint32(uint32_t addr, uint32_t data);    
    void writeByte(const uint32_t addressOffset, uint8_t data);
};

#endif	/* SPIFLASH_H */

