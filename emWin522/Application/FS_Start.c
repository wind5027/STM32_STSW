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
File    : FS_Start.c
Purpose : Start application for file system.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include "FS.h"
#include "FS_Int.h"


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       MainTask
*/
void MainTask(void);      // Forward declaration to avoid "no prototype" warning
void MainTask(void) {
  U32          v;
  FS_FILE    * pFile;
  char         ac[256];
  const char * sVolName = "";

  printf("Start\n");
  //
  // Initialize file system
  //
  FS_Init();
  //
  // Check if low-level format is required
  //
  FS_FormatLLIfRequired(sVolName);
  //
  // Check if volume needs to be high level formatted.
  //
  if (FS_IsHLFormatted(sVolName) == 0) {
    printf("High-level format\n");
    FS_Format(sVolName, NULL);
  }
  printf("Running sample on \"%s\"\n", sVolName);
  v = FS_GetVolumeFreeSpaceKB(sVolName);
  if (v <= 0x7fff) {
    printf("  Free space: %lu kbytes\n", v);
  } else {
    v >>= 10;
    printf("  Free space: %lu MBytes\n", v);
  }
  sprintf(ac, "%s\\File.txt", sVolName);
  printf("  Write test data to file %s\n", ac);
  pFile = FS_FOpen(ac, "w");
  if (pFile) {
    FS_Write(pFile, "Test", 4);
    FS_FClose(pFile);
  } else {
    printf("Could not open file: %s to write.\n", ac);
  }
  v = FS_GetVolumeFreeSpaceKB(sVolName);
  if (v <= 0x7fff) {
    printf("  Free space: %lu kbytes\n", v);
  } else {
    v >>= 10;
    printf("  Free space: %lu MBytes\n", v);
  }
  printf("Finished\n");
  while (1) {
    ;
  }
}

/*************************** End of file ****************************/
