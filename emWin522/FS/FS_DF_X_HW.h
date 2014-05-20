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
File        : FS_DF_X_HW.h
Purpose     : Data Flash hardware layer
---------------------------END-OF-HEADER------------------------------
*/
#ifndef __FS_DF_X_HW_H__               // Avoid recursive and multiple inclusion
#define __FS_DF_X_HW_H__

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Global function prototypes
*
**********************************************************************
*/

/* Control line functions */
void          FS_DF_HW_X_EnableCS   (U8 Unit);
void          FS_DF_HW_X_DisableCS  (U8 Unit);
int           FS_DF_HW_X_Init       (U8 Unit);

/* Data transfer functions */
void          FS_DF_HW_X_Read (U8 Unit,       U8 * pData, int NumBytes);
void          FS_DF_HW_X_Write(U8 Unit, const U8 * pData, int NumBytes);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FS_DF_X_HW_H__

/*************************** End of file ****************************/
