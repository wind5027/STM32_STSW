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
File    : OS_Start_2Tasks.c
Purpose : Skeleton program for OS
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.h"

OS_STACKPTR int _Stack[128];          // Task stacks
OS_TASK         _TCB;                 // Task-control-blocks


/*********************************************************************
*
*       HPTask
*/
static void HPTask(void) {
  while (1) {
    OS_Delay (10);
  }
}

/*********************************************************************
*
*       MainTask
*/
void MainTask(void);
void MainTask(void) {
  OS_CREATETASK(&_TCB, "HPTask", HPTask, 150, _Stack);
  while (1) {
    OS_Delay (50);
  }
}

/*************************** End of file ****************************/
