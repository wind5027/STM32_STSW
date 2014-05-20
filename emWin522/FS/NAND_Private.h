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
File        : NAND_Private.h
Purpose     : Private header file for the NAND flash driver.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __NAND_PRIVATE_H__               // Avoid recursive and multiple inclusion
#define __NAND_PRIVATE_H__

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef void (FS_NAND_TEST_HOOK)(U8 Unit);

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
int  FS__NAND_IsONFISupported(U8 Unit);
int  FS__NAND_ReadONFIPara   (U8 Unit, void * pPara);
void FS__NAND_SetTestHook    (FS_NAND_TEST_HOOK * pfTestHook);
void FS__NAND_UNI_SetTestHook(FS_NAND_TEST_HOOK * pfTestHook);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __NAND_PRIVATE_H__

/*************************** End of file ****************************/
