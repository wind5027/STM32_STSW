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
File        : MMC_X_HW.h
Purpose     : MMC hardware layer
---------------------------END-OF-HEADER------------------------------
*/
#ifndef __MMC_X_HW_H__               // Avoid recursive and multiple inclusion
#define __MMC_X_HW_H__

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*     Defines, configurable
*
**********************************************************************
*/
#ifndef   FS_MMC_REPORT_HW_ERRORS
  #define FS_MMC_REPORT_HW_ERRORS  0
#endif

/*********************************************************************
*
*     Global function prototypes
*
**********************************************************************
*/

//
// Control line functions
//
void FS_MMC_HW_X_EnableCS        (U8 Unit);
void FS_MMC_HW_X_DisableCS       (U8 Unit);
//
// Medium status functions
//
int  FS_MMC_HW_X_IsPresent       (U8 Unit);
int  FS_MMC_HW_X_IsWriteProtected(U8 Unit);

//
// Operation condition detection & adjusting
//
U16  FS_MMC_HW_X_SetMaxSpeed     (U8 Unit, U16 MaxFreq);
int  FS_MMC_HW_X_SetVoltage      (U8 Unit, U16 Vmin,   U16 Vmax);
//
// Data transfer functions
//
#if FS_MMC_REPORT_HW_ERRORS
  int  FS_MMC_HW_X_Read          (U8 Unit,       U8 * pData, int NumBytes);
  int  FS_MMC_HW_X_Write         (U8 Unit, const U8 * pData, int NumBytes);
#else
  void FS_MMC_HW_X_Read          (U8 Unit,       U8 * pData, int NumBytes);
  void FS_MMC_HW_X_Write         (U8 Unit, const U8 * pData, int NumBytes);
#endif

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __MMC_X_HW_H__

/*************************** End of file ****************************/
