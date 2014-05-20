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
File        : NAND_X_HW.h
Purpose     : Generic header file for the file system's NAND 2k driver
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
None.
---------------------------END-OF-HEADER------------------------------
*/
#ifndef __NAND_X_HW_H__               // Avoid recursive and multiple inclusion
#define __NAND_X_HW_H__

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

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/

void FS_NAND_HW_X_DisableCE       (U8 Unit);
void FS_NAND_HW_X_EnableCE        (U8 Unit);

void FS_NAND_HW_X_SetAddrMode     (U8 Unit);
void FS_NAND_HW_X_SetCmdMode      (U8 Unit);
void FS_NAND_HW_X_SetDataMode     (U8 Unit);

int  FS_NAND_HW_X_WaitWhileBusy   (U8 Unit, unsigned us);

void FS_NAND_HW_X_Init_x8         (U8 Unit);
void FS_NAND_HW_X_Init_x16        (U8 Unit);
void FS_NAND_HW_X_Read_x8  (U8 Unit,       void * pBuffer, unsigned NumBytes);
void FS_NAND_HW_X_Write_x8 (U8 Unit, const void * pBuffer, unsigned NumBytes);
void FS_NAND_HW_X_Read_x16 (U8 Unit,       void * pBuffer, unsigned NumBytes);
void FS_NAND_HW_X_Write_x16(U8 Unit, const void * pBuffer, unsigned NumBytes);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __NAND_X_HW_H__

/*************************** End of file ****************************/
