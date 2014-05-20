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
File        : FS_CheckDisk.c
Purpose     : Sample program demonstrating disk checking functionality.
---------------------------END-OF-HEADER------------------------------
*/

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "FS_API.h"
#include "Global.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define VOLUME_NAME       ""
#define MAX_RECURSION     5

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static U32 _aBuffer[2000];   // The more space is used the faster the disk checking can run.
static int _NumErrors;

/*********************************************************************
*
*       _OnError
*/
static int _OnError(int ErrCode, ...) {
  va_list      ParamList;
  const char * sFormat;
  int          c;
  char         ac[1000];

  sFormat = FS_FAT_CheckDisk_ErrCode2Text(ErrCode);
  if (sFormat) {
    va_start(ParamList, ErrCode);
    vsprintf(ac, sFormat, ParamList);
    FS_X_Log(ac);
    FS_X_Log("\n");
  }
  if (ErrCode != FS_ERRCODE_CLUSTER_UNUSED) {
    FS_X_Log("  Do you want to repair this? (y/n/a) ");
  } else {
    FS_X_Log("  * Convert lost cluster chain into file (y)\n"
             "  * Delete cluster chain                 (d)\n"
             "  * Do not repair                        (n)\n"
             "  * Abort                                (a) ");
    FS_X_Log("\n");
  }
  ++_NumErrors;
  c = getchar();
  FS_X_Log("\n");
  if ((c == 'y') || (c == 'Y')) {
    return 1;
  } else if ((c == 'a') || (c == 'A')) {
    return 2;
  } else if ((c == 'd') || (c == 'D')) {
    return 3;
  }
  return 0;     // Do not fix.
}

/*********************************************************************
*
*       MainTask
*/
void MainTask(void);      // Forward declaration to avoid "no prototype" warning
void MainTask(void) {
  FS_X_Log("Start\n");
  FS_Init();
  _NumErrors = 0;
  while (FS_CheckDisk(VOLUME_NAME, _aBuffer, sizeof(_aBuffer), MAX_RECURSION, _OnError) == 1) {
    ;
  }
  if (_NumErrors == 0) {
    FS_X_Log("No errors found.\n");
  }
  FS_X_Log("Finished\n");
  while (1) {
    ;
  }
}

/*************************** End of file ****************************/

