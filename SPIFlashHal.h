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

#ifndef _SPIFLASHHAL_H    /* Guard against multiple inclusion */
#define _SPIFLASHHAL_H

#include "atmel_start.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
typedef uint32_t parm_type;
typedef int32_t status_t;

#include "SoftSPI.h"
#include "gm_SystemPinMap.h"

#define SPI_STATIC_VARIABLES() SoftSPI SPIBusForFlash(SPI_FLASH_DI_PIN, SPI_FLASH_DO_PIN, SPI_FLASH_CS_PIN, SPI_FLASH_CLK_PIN);
#define NUMBER_OF_BLOBS 2    
#define SPI_TRANSFER(outBuf,inBuf,sz) SPIBusForFlash.transferData(outBuf, inBuf, sz)



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SPIFLASHHAL_H */

/* *****************************************************************************
 End of File
 */
