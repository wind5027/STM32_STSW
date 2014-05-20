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
File        : FS_OS.h
Purpose     : File system's OS Layer header file
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __FS_OS_H__               // Avoid recursive and multiple inclusion
#define __FS_OS_H__

#include "FS_ConfDefaults.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

void FS_X_OS_Lock   (unsigned LockIndex);
void FS_X_OS_Unlock (unsigned LockIndex);

void FS_X_OS_Init   (unsigned NumLocks);
void FS_X_OS_DeInit (void);

U32  FS_X_OS_GetTime(void);

int  FS_X_OS_Wait   (int Timeout);
void FS_X_OS_Signal (void);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FS_OS_H__

/*************************** End of file ****************************/
