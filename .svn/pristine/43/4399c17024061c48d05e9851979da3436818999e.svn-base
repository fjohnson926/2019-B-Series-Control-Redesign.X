/* 
 * File:   SPIFlash.cpp
 * Author: JTurner
 * 
 * Created on June 27, 2019, 10:21 AM
 */

#include "SPIFlash.h"

SPI_STATIC_VARIABLES();
Blob::Blob()
{
    A_bank = 0;
    B_bank = 0;
    blobSizeInBytes = 0;
    pointerToRAMData = NULL;
    activeSignature = 0;
    blob_write_count_low = 0;
    blob_write_count_high = 0;
    writeToBank = BANK_A;
    moreRecentBank = BANK_B;
    blob_dirty = false;
    blob_nvState = NVSTATE_UNKNOWN;
}
Blob::Blob(uint32_t A_bank, uint32_t B_bank, uint32_t blobSizeInBytes, uint32_t * pointerToRamData, uint32_t activeSignature)
{
    //Store and load in 4 byte "words". Will need to pad or remove some data, or will have problems loading from or saving to NVRAM
    if(blobSizeInBytes%4)
    {
        #if __DEBUG
            asm("bkpt");    
        #endif
    }
    this->A_bank = A_bank;
    this->B_bank = B_bank;
    this->blobSizeInBytes = blobSizeInBytes;
    this->pointerToRAMData = pointerToRamData;
    this->activeSignature = activeSignature;
    this->blob_write_count_low = 0;
    this->blob_write_count_high = 0;
    this->writeToBank = BANK_A;
    this->moreRecentBank = BANK_B;
    this->blob_dirty = false;
    this->blob_nvState = NVSTATE_UNKNOWN;
    this->hasavalidbank = false;
}

Blob & Blob::operator=(const Blob & source)
{
    this->A_bank = source.A_bank;
    this->B_bank = source.B_bank;
    this->blobSizeInBytes = source.blobSizeInBytes;
    this->pointerToRAMData = source.pointerToRAMData;
    this->activeSignature = source.activeSignature;
    this->blob_write_count_low = source.blob_write_count_low;
    this->blob_write_count_high = source.blob_write_count_high;
    this->writeToBank = source.writeToBank;
    this->moreRecentBank = source.moreRecentBank;
    this->blob_dirty = source.blob_dirty;
    this->blob_nvState = source.blob_nvState;
    this->hasavalidbank = source.hasavalidbank;
}

SPIFlash::SPIFlash() 
{   
  for(int i = 0; i < K_BUFFER_SIZE; i++)
  {
    sendBuffer[i] = K_CMD_WRITE_DISABLE;
    receiveBuffer[i] = 0;
  }  
}

void SPIFlash::addBlob(Blob blob)
{
    storeBlobInformation(blob);
    blobsInitialized = false;
}

void SPIFlash::storeBlobInformation(Blob blob)
{
    if(blobWriteIndex < NUMBER_OF_BLOBS)
    {
        allBlobs[blobWriteIndex] = blob;
        determineNVState();
        
        //----- Look for recoverable issues and correct before proceeding -----

      //Do we have two valid banks?
      if(allBlobs[blobWriteIndex].blob_nvState == NVSTATE_TWO_COPIES)
      {
        //Erase the older one
        if(allBlobs[blobWriteIndex].moreRecentBank == allBlobs[blobWriteIndex].B_bank)
        {
          //B is newer, Erase Bank A
          eraseSector(allBlobs[blobWriteIndex].A_bank, FLASHSPI_CFG_SIZE_SECTOR);
        }
        else
        {
          //A is newer, Erase Bank B
          eraseSector(allBlobs[blobWriteIndex].B_bank, FLASHSPI_CFG_SIZE_SECTOR);
        }
        //The nv parms are now ready
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_READY;
      }

      //If invalid, need to erase the corrupted data
      else if(allBlobs[blobWriteIndex].blob_nvState == NVSTATE_INVALID)
      {
        //Erase both banks - whatever is there is not recoverable
        eraseSector(allBlobs[blobWriteIndex].A_bank, FLASHSPI_CFG_SIZE_SECTOR);    //  FlashSpi_Drv_EraseSector(BANK_A_ADDR, FLASHSPI_CFG_SIZE_SECTOR);
        eraseSector(allBlobs[blobWriteIndex].B_bank, FLASHSPI_CFG_SIZE_SECTOR);    //  FlashSpi_Drv_EraseSector(BANK_B_ADDR, FLASHSPI_CFG_SIZE_SECTOR);
        //NV parms are now erased
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_ERASED;
      }

      //----- All possible recovery attempted -----

      //If ready, can read blob from nonvolatile storage
      if (allBlobs[blobWriteIndex].blob_nvState == NVSTATE_READY)
      {
        uint32_t addr;
        //A Bank has good data?
        if(allBlobs[blobWriteIndex].writeToBank  == BANK_B)
        {
          addr = allBlobs[blobWriteIndex].A_bank; //Data in A
        }
        else
        {
          addr = allBlobs[blobWriteIndex].B_bank; //Data in B
        }
        //Offset by signature byte
        addr += sizeof(uint32_t);

        //Get the stored write count
        allBlobs[blobWriteIndex].blob_write_count_low = readParm(addr);
        allBlobs[blobWriteIndex].blob_write_count_high = allBlobs[blobWriteIndex].blob_write_count_low;

        //OK, data is valid to read whenever loadBlob() is called
      }
      //If both banks are erased, likely first write at factory
      else if (allBlobs[blobWriteIndex].blob_nvState == NVSTATE_ERASED)
      {
        //Do initial write in Bank A
        allBlobs[blobWriteIndex].writeToBank = BANK_A;
      }    
    }
    blobWriteIndex++;
}

void SPIFlash::initNVBlobs()
{
    
}

void SPIFlash::determineNVState()
{   
    //Temporary parameter values
    uint32_t sigAstart, sigAend, cntAstart, cntAend; //A bank parameters
    uint32_t sigBstart, sigBend, cntBstart, cntBend; //B bank parameters
    uint32_t addr;
    uint32_t A_bank, B_bank, blobSz, activeSig;
    //Pointers back to globals for each NvBlob
    eNvState* nvState_p;
    bool* writeToBank_p;
    bool* moreRecentBank_p;

    bool bankAerased = false;
    bool bankBerased = false;
    bool bankAhasValidData = false;
    bool bankBhasValidData = false;
    bool bankAinvalid = false;
    bool bankBinvalid = false;

    //Read signatures from flash copy A
    addr = allBlobs[blobWriteIndex].A_bank; //Point to base address with signature
    sigAstart = readParm(addr);
    addr += sizeof(uint32_t); //Offset by size of the signature
    cntAstart = readParm(addr);
    addr += sizeof(uint32_t) + allBlobs[blobWriteIndex].blobSizeInBytes; //Offset by size of count value and the blob
    sigAend   = readParm(addr);
    addr += sizeof(uint32_t); //Offset by size of the signature
    cntAend   = readParm(addr);

    //Read signatures from flash copy B
    addr = allBlobs[blobWriteIndex].B_bank; //Point to base address with signature
    sigBstart = readParm(addr);
    addr += sizeof(uint32_t); //Offset by size of the signature
    cntBstart = readParm(addr);
    addr += sizeof(uint32_t) + allBlobs[blobWriteIndex].blobSizeInBytes; //Offset by size of count value and the blob
    sigBend   = readParm(addr);
    addr += sizeof(uint32_t); //Offset by size of the signature
    cntBend   = readParm(addr);

    //Recipe Bank A erased?
    bankAerased = isErased(allBlobs[blobWriteIndex].A_bank);

    //Recipe Bank B erased?
    bankBerased = isErased(allBlobs[blobWriteIndex].B_bank);

    //----------------------------------------
    //Process based on latest revision
    //----------------------------------------

    //Does Bank A appear to have valid data?
    if( (sigAstart == allBlobs[blobWriteIndex].activeSignature) &&
      (sigAend == allBlobs[blobWriteIndex].activeSignature) &&
      (cntAstart == cntAend) )
    {
        bankAhasValidData = true;
        allBlobs[blobWriteIndex].hasavalidbank = true;
    }

    //Does Bank B appear to have valid data?
    if( (sigBstart == allBlobs[blobWriteIndex].activeSignature) &&
      (sigBend == allBlobs[blobWriteIndex].activeSignature) &&
      (cntBstart == cntBend) )
    {
        bankBhasValidData = true;
        allBlobs[blobWriteIndex].hasavalidbank = true;
    }

    //Does Bank A have data but it's not valid?
    if(!bankAhasValidData && !bankAerased)
    {
        bankAinvalid = true;
    }

    //Does Bank B have data but it's not valid?
    if(!bankBhasValidData && !bankBerased)
    {
        bankBinvalid = true;
    }

    //Is the NV storage completely erased?
    if ( bankAerased && bankBerased )
    {
        allBlobs[blobWriteIndex].writeToBank = BANK_A;
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_ERASED;
    }
    //Bank A has data and Bank B is erased?
    else if( bankAhasValidData && bankBerased )
    {
        allBlobs[blobWriteIndex].writeToBank = BANK_B;
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_READY;
    }
    //Bank B has data and Bank A is erased?
    else if( bankAerased && bankBhasValidData )
    {
        allBlobs[blobWriteIndex].writeToBank = BANK_A;
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_READY;
    }
    //If both banks have data, determine which is
    // the more recent data
    else if( bankAhasValidData && bankBhasValidData )
    {
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_TWO_COPIES;
    //Determine which is newer
    if(cntBend > cntAend)
    {
      allBlobs[blobWriteIndex].moreRecentBank = BANK_B; //B is more recent
      allBlobs[blobWriteIndex].writeToBank    = BANK_A; //A is scratch
    }
    else
    {
      allBlobs[blobWriteIndex].moreRecentBank  = BANK_A; //A is more recent
      allBlobs[blobWriteIndex].writeToBank    = BANK_B; //B is scratch
    }
    }
    //Does Bank A have data but Bank B has invalid data?
    else if(bankAhasValidData && bankBinvalid)
    {
        //Treat it as having two copies, but mark the valid as latest
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_TWO_COPIES;
        allBlobs[blobWriteIndex].moreRecentBank = BANK_A; //Keep A
        allBlobs[blobWriteIndex].writeToBank    = BANK_B; //Write over B
    }
    //Does Bank B have data but Bank A has invalid data?
    else if(bankBhasValidData && bankAinvalid)
    {
        //Treat it as having two copies, but mark the valid as latest
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_TWO_COPIES;
        allBlobs[blobWriteIndex].moreRecentBank = BANK_B; //Keep B
        allBlobs[blobWriteIndex].writeToBank    = BANK_A; //Write over A
    }
    //Not a valid state
    else
    {
        allBlobs[blobWriteIndex].blob_nvState = NVSTATE_INVALID;
    }
}

parm_type SPIFlash::readParm(uint32_t addr)
{
    volatile parm_type parm = 0; //Parameter value to build
    volatile uint32_t  data = 0; //Buffers read. Was trashing parm w/o it
    volatile uint8_t   databyte; //Value of read byte

    //Get MS byte and add it in
    readByte(addr, (uint8_t*)&data);
    //FlashSpi_Drv_ReadArrayU8(addr, 1, (uint8_t*)&data);
    databyte = data;
    parm = (parm_type)(databyte << 24);
    //Get next byte and add it in
    readByte(addr + 1, (uint8_t*)&data);
    //FlashSpi_Drv_ReadArrayU8(addr + 1, 1, (uint8_t*)&data);
    databyte = data;
    parm |= (parm_type)(databyte << 16);
    //Get next byte and add it in
    readByte(addr + 2, (uint8_t*)&data);
    //FlashSpi_Drv_ReadArrayU8(addr + 2, 1, (uint8_t*)&data);
    databyte = data;
    parm |= (parm_type)(databyte << 8);
    //Get last byte and add it in
    readByte(addr + 3, (uint8_t*)&data);
    //FlashSpi_Drv_ReadArrayU8(addr + 3, 1, (uint8_t*)&data);
    databyte = data;
    parm |= (parm_type)databyte;

    return parm;    
}

uint8_t SPIFlash::readByte(uint32_t addressOffset, uint8_t * buff)
{
    uint8_t temp = 0;
	status_t result = 1;
	sendBuffer[K_INDEX_CMD] = K_CMD_READ_DATA;
	buildAddress(addressOffset);
	sendBuffer[4] = temp;
	SPI_TRANSFER(sendBuffer,receiveBuffer,K_LENGTH_CMD_READ_DATA);
	temp = receiveBuffer[K_INDEX_READ_DATA_START];
	*buff = temp;
}

void SPIFlash::buildAddress(uint32_t addressOffset)
{
	uint16_t	page_addr;
	uint16_t	byte_addr;

	page_addr = addressOffset / SPI_FLASH_PAGE_SIZE;
	byte_addr = addressOffset % SPI_FLASH_PAGE_SIZE;

	sendBuffer[K_INDEX_ADDR2] = page_addr >> 8;
	sendBuffer[K_INDEX_ADDR1] = page_addr;
	sendBuffer[K_INDEX_ADDR0] = byte_addr;
}

void SPIFlash::eraseSector(uint32_t addressOffset, uint32_t length)
{
	uint32_t actual = 0;
	uint32_t num_sectors;
	status_t result = 1;

	num_sectors = length / FLASH_SECTOR_SIZE;	// find out how many sectors to erase

	//Account for any possible remainder if not on a sector boundary
	if( (num_sectors * FLASH_SECTOR_SIZE) != length)
	{
	  num_sectors++; //Need one additional sector
	}

	for (actual = 0; actual < num_sectors; actual++)
	{

		// if pass length is whole 64K block
		if (((num_sectors%16) == 0) && ((num_sectors - actual) >=16))
		{
			// Erase a whole 64K block
			sendBuffer[K_INDEX_CMD] = K_CMD_FULL_BLOCK_ERASE;
			actual += 15;
		}
		// if pass length is whole 32K block
		else if (((num_sectors%8) == 0) && ((num_sectors - actual) >= 8))
		{
			// Erase a whole 32K block
			sendBuffer[K_INDEX_CMD] = K_CMD_HALF_BLOCK_ERASE;

		}
		else	// erase 4K block
		{
			sendBuffer[K_INDEX_CMD] = K_CMD_SECTOR_ERASE;
		}

		buildAddress(addressOffset);

		enableWrite();	//Enable write to the chip (disabled automatically after each page write)

        SPI_TRANSFER(sendBuffer,NULL,K_LENGTH_SECTOR_ERASE);
		while(isBusy())
		{
			//GPIO_DRV_SetPinOutput(LED_RED);	// while chip is busy then turn red led on
		}
	}
}

status_t SPIFlash::enableWrite(void)
{
    status_t result = 1;
    uint8_t txBuffer[K_LENGTH_WRITE_ENABLE];
    txBuffer[K_INDEX_CMD] = K_CMD_WRITE_ENABLE;
    SPI_TRANSFER(txBuffer,NULL,K_LENGTH_WRITE_ENABLE);
    return result;
}

bool SPIFlash::isBusy(void)
{
	status_t result = 1;
	bool status;
	uint8_t txBuffer[K_LENGTH_CMD_READ_STATUS]; //2 bytes
	txBuffer[K_INDEX_CMD] = K_CMD_READ_STATUS1;
    SPI_TRANSFER(txBuffer, receiveBuffer, K_LENGTH_CMD_READ_STATUS);
	status = (receiveBuffer[K_INDEX_RSP_READ_STATUS] & K_BIT_BUSY_STATUS) == 1;
	return status;
}

bool SPIFlash::isErased(uint32_t addr)
{
    parm_type data; //Data read at each parm location
    int start, end;

    //Set correct base address  
    start = 0;
    end   = allBlobs[blobWriteIndex].blobSizeInBytes;

    //Loop through all defined parm/word slots and insure all are blank
    for(int i = start; i <= end; i++, addr += sizeof(parm_type))
    {
        //Read the parameter
        data = readParm(addr);
        //See if not erased
        if(data != ERASED_STATE)
        {
          return false; //Found something not erased!
        }
    }
  //Not able to find anything not erased
  return true;
}


bool SPIFlash::flushNvBlob(int index)
{
    if(index > blobWriteIndex)
    {
        return false;
    }
    
    uint32_t size; //Size in 32-bit words
    uint32_t addr; //Address in NV storage
    uint32_t sig;  //Signature
    uint32_t cnt;  //Write count
    uint32_t A_bank, B_bank;
    bool writeToBank;
    bool dirty;
    
    size = allBlobs[index].blobSizeInBytes/4;
    sig  = allBlobs[index].activeSignature;
    cnt = allBlobs[index].blob_write_count_low;
    A_bank = allBlobs[index].A_bank;
    B_bank = allBlobs[index].B_bank;
    writeToBank = allBlobs[index].writeToBank;
    dirty = allBlobs[index].blob_dirty;
    //Get address for correct bank
    if(allBlobs[index].writeToBank == BANK_A)
    {
      //A bank start
      addr = allBlobs[index].A_bank;
    }
    else //Data in B bank
    {
      //B bank start
      addr = allBlobs[index].B_bank;
    }     

    //Write out the signature
    writeNvUint32(addr, sig);
    addr += sizeof(uint32_t); //Offset the address by the signature
    //Write out the start write count
    writeNvUint32(addr, cnt);
    addr += sizeof(uint32_t); //Offset the address by the count size

    //Have the two addresses and size, read from RAM and write to NV storage
    for(int i=0; i < size; i++)
    {
      //Read RAM and write to NV storage
      writeNvUint32(addr, allBlobs[index].pointerToRAMData[i]);
      //Increment addr by size of a 32-bit word
      addr += sizeof(uint32_t);
    }

    //Write out the end signature
    writeNvUint32(addr, sig);
    addr += sizeof(uint32_t); //Offset the address by the signature
    //Write out the end write count
    writeNvUint32(addr, cnt);

    //JDT -- When using erase command on what should be B_Bank, A_Bank gets erased
    //----- Erase the older bank and make it the new write bank -----
    if(allBlobs[index].writeToBank == BANK_A)
    {
      //Just wrote to Bank A, so erase Bank B
      eraseSector(allBlobs[index].B_bank, FLASHSPI_CFG_SIZE_SECTOR);
      //Bank B is now the bank to write to
      allBlobs[index].writeToBank = BANK_B;
    }
    else //Just wrote to Bank B
    {
      //Just wrote to Bank B, so erase Bank A
      eraseSector(allBlobs[index].A_bank, FLASHSPI_CFG_SIZE_SECTOR);
      //Bank A is now the bank to write to
      allBlobs[index].writeToBank = BANK_A;
    }

    //Data in memory is no longer "dirty" and in need of a good flushing
    allBlobs[index].blob_dirty = false;

    return true;
}

void SPIFlash::writeNvUint32(uint32_t addr, uint32_t data)
{
  uint8_t   databyte; //Value of read byte

  //Write out most significant byte
  databyte = ((data & 0xFF000000) >> 24);
  writeByte(addr, databyte);
  //Write out next byte
  databyte = ((data & 0x00FF0000) >> 16);
  writeByte(addr + 1, databyte);
  //Write out the following byte
  databyte = ((data & 0x0000FF00) >> 8);
  writeByte(addr + 2, databyte);
  //Write out the least significant byte
  databyte = data & 0x000000FF;
  writeByte(addr + 3, databyte);
}

void SPIFlash::writeByte(const uint32_t addressOffset, uint8_t data)
{
	status_t result = 1;
	uint8_t txByte;
	txByte = data;
	sendBuffer[K_INDEX_CMD] = K_CMD_PAGE_PROGRAM;
	buildAddress(addressOffset);
	sendBuffer[K_INDEX_WRITE_DATA_START] = txByte;
	enableWrite();	//Enable write to the chip (disabled automatically after each page write)
    SPI_TRANSFER(sendBuffer, NULL, K_LENGTH_BYTE_WRITE_DATA);
	while(isBusy())
	{
		
	}
}

void SPIFlash::loadNvBlob(int index)
{
    if(index > blobWriteIndex)
    {
        return;
    }
    uint32_t size; //Size in 32-bit words
    uint32_t addr; //Address in NV storage    
    size = allBlobs[index].blobSizeInBytes/4;
    //Get address for correct bank
    if(allBlobs[index].writeToBank == BANK_B)
    {
      //A bank start + offset for two 32-bit parms
      addr = allBlobs[index].A_bank + (2*sizeof(uint32_t));
    }
    else //Data in B bank
    {
      //B bank start + offset for two 32-bit parms
      addr = allBlobs[index].B_bank + (2*sizeof(uint32_t));
    }
    //Have the two addresses and size, read from storage and copy into RAM
    for(int i=0; i < size; i++)
    {
    //Read parm and write to RAM
    allBlobs[index].pointerToRAMData[i] = readParm(addr);
    //Increment addr by size of a 32-bit word
    addr += sizeof(uint32_t);
    }
}

bool SPIFlash::isAtLeastOneBankValid(int index)
{
    if(index > blobWriteIndex)
    {
        #if __DEBUG
            asm("bkpt");    
        #endif
    }
    return allBlobs[index].hasavalidbank;
}
        