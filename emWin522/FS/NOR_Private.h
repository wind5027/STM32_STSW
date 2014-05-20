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
File        : NOR_Private.h
Purpose     : Private header file for the system-wide generic FLASH driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __NOR_PRIVATE_H__               // Avoid recursive and multiple inclusion
#define __NOR_PRIVATE_H__

#include "SEGGER.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       #define constants
*
**********************************************************************
*/
#ifndef   FS_NOR_LINE_SIZE
  #define FS_NOR_LINE_SIZE         4   // Size of a flash line. 4 for most external, bigger for some MCUs. LPC requires 16 !
#endif

#ifndef   FS_NOR_CAN_REWRITE
  #if FS_NOR_LINE_SIZE > 4
    #define FS_NOR_CAN_REWRITE     0   // Most flash chips can rewrite the same data, as long as 0-bits remain
  #else
    #define FS_NOR_CAN_REWRITE     1
  #endif
#endif

#ifndef   FS_NOR_FAR                  // Used for 8/16-bit CPUs only
  #define FS_NOR_FAR
#endif

/*********************************************************************
*
*       Macros, function replacement, interrupt handling
*
**********************************************************************
*/
#ifndef   FS_NOR_DI
  #define FS_NOR_DI()
#endif

#ifndef   FS_NOR_EI
  #define FS_NOR_EI()
#endif

#ifndef   FS_NOR_DELAY
  #define FS_NOR_DELAY()
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct FS_NOR_PROGRAM_HW {
  int  (*pfRead)         (U8 Unit, void * pDest, U32 SrcAddr, U32 NumBytes);
  int  (*pfEraseSector)  (U8 Unit, U32 BaseAddr, U32 SectorAddr);
  char (*pfWrite)        (U8 Unit, U32 BaseAddr, U32 SectorAddr, U32 DestAddr, const U16 FS_NOR_FAR * pSrc0, int NumItems);
} FS_NOR_PROGRAM_HW;

typedef void (FS_NOR_TEST_HOOK)(U8 Unit);

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
extern const FS_NOR_PROGRAM_HW FS_NOR_Program_Intel_1x16;
extern const FS_NOR_PROGRAM_HW FS_NOR_Program_AMD_1x16;
extern const FS_NOR_PROGRAM_HW FS_NOR_Program_IntelFast_1x16;
extern const FS_NOR_PROGRAM_HW FS_NOR_Program_AMDFast_1x16;

extern const FS_NOR_PROGRAM_HW FS_NOR_Program_Intel_2x16;
extern const FS_NOR_PROGRAM_HW FS_NOR_Program_AMD_2x16;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void FS_NOR_CFI_ReadCFI_1x16(U8 Unit, U32 BaseAddr, U32 Off, U8 * pData, unsigned NumItems);
void FS_NOR_CFI_ReadCFI_2x16(U8 Unit, U32 BaseAddr, U32 Off, U8 * pData, unsigned NumItems);
void FS__NOR_SetTestHook    (FS_NOR_TEST_HOOK * pfTestHook);
void FS__NOR_BM_SetTestHook (FS_NOR_TEST_HOOK * pfTestHook);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __NOR_PRIVATE_H__

/*************************** End of file ****************************/
