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
File        : FS_ConfigIO.c
Purpose     : I/O Configuration routines for Filesystem
---------------------------END-OF-HEADER------------------------------
*/

#include <stdio.h>
#include "FS.h"

/*********************************************************************
*
*       _puts
*
*  Function description
*    Local (static) replacement for puts.
*    The reason why puts is not used is that puts always appends a NL
*    character, which screws up our formatting.
*/
static void _puts(const char * s) {
  char c;

  for (;;) {
    c = *s++;
    if (c == 0) {
      break;
    }
    putchar(c);
  }
}

/*********************************************************************
*
*       FS_X_Panic
*
*  Function description
*    Referred in debug builds of the file system only and
*    called only in case of fatal, unrecoverable errors.
*/
void FS_X_Panic(int ErrorCode) {
  char ac[20];
  SEGGER_BUFFER_DESC BufferDesc;

  BufferDesc.pBuffer    = ac;
  BufferDesc.BufferSize = sizeof(ac);
  BufferDesc.Cnt        = 0;
  SEGGER_PrintInt(&BufferDesc, ErrorCode, 10, 0);
  _puts("FS panic: ");
  _puts(ac);
  _puts("\n");
  while (1) {
    ;
  }
}

/*********************************************************************
*
*      Logging: OS dependent
*
*  Note
*       Logging is used in higher debug levels only. The typical target
*       build does not use logging and does therefore not require any of
*       the logging routines below. For a release build without logging
*       the routines below may be eliminated to save some space.
*       (If the linker is not function aware and eliminates unreferenced
*       functions automatically)
*/
void FS_X_Log(const char * s) {
  _puts(s);
}

void FS_X_Warn(const char * s) {
  _puts("FS warning: ");
  _puts(s);
  _puts("\n");
}

void FS_X_ErrorOut(const char * s) {
  _puts("FS error: ");
  _puts(s);
  _puts("\n");
}

/*************************** End of file ****************************/
