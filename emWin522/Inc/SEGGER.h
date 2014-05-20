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
File    : SEGGER.h
Purpose : Global types etc & general purpose utility functions
---------------------------END-OF-HEADER------------------------------
*/

#ifndef SEGGER_H            // Guard against multiple inclusion
#define SEGGER_H

#include "Global.h"         // Type definitions: U8, U16, U32, I8, I16, I32

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Function-like macros
*
**********************************************************************
*/

#define SEGGER_COUNTOF(a)          (sizeof((a))/sizeof((a)[0]))
#define SEGGER_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define SEGGER_MAX(a,b)            (((a) > (b)) ? (a) : (b))

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  char *pBuffer;
  int   BufferSize;
  int   Cnt;
} SEGGER_BUFFER_DESC;

/*********************************************************************
*
*       Utility functions
*
**********************************************************************
*/

void SEGGER_ARM_memcpy   (void *pDest, const void *pSrc, int NumBytes);
void SEGGER_memcpy       (void *pDest, const void *pSrc, int NumBytes);
void SEGGER_StoreChar    (SEGGER_BUFFER_DESC *p, char c);
void SEGGER_PrintUnsigned(SEGGER_BUFFER_DESC *pBufferDesc, U32 v, unsigned Base, int NumDigits);
void SEGGER_PrintInt     (SEGGER_BUFFER_DESC *pBufferDesc, I32 v, unsigned Base, unsigned NumDigits);
int  SEGGER_snprintf     (char *pBuffer, int BufferSize, const char *sFormat, ...);


#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
