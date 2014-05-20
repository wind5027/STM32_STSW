/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2003-2013     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : MMC_MCI_HW.h
Purpose     : Generic MMC/SD card driver for Atmel generic MCI controller
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __MMC_MCI_HW_H__               // Avoid recursive and multiple inclusion
#define __MMC_MCI_HW_H__

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

typedef struct {
  U32 BaseAddr;
  U32 Mode;
} MCI_INFO;

typedef void(ISR_FUNC)(void);

void FS_MCI_HW_EnableClock         (U8 Unit, unsigned OnOff);
void FS_MCI_HW_EnableISR           (U8 Unit, ISR_FUNC * pISRHandler);
U32  FS_MCI_HW_GetMClk             (U8 Unit);
void FS_MCI_HW_GetMCIInfo          (U8 Unit, MCI_INFO * pInfo);
void FS_MCI_HW_Init                (U8 Unit);
int  FS_MCI_HW_IsCardPresent       (U8 Unit);
U8   FS_MCI_HW_IsCardWriteProtected(U8 Unit);

void FS_MCI_HW_CleanDCacheRange(void * p, unsigned NumBytes);
void FS_MCI_HW_InvalidateDCache(void * p, unsigned NumBytes);

U32  FS_MCI_HW_GetTransferMem(U32 * pPAddr, U32 * pVAddr);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __MMC_MCI_HW_H__

/*************************** End of file ****************************/

