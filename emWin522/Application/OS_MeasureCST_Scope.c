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
File    : OS_MeasureCST_Scope.c
Purpose : Performance test program for OS
          This benchmark uses the LED.c module to set and clear a port
          pin. This allows measuring the context switch time with an oscilloscope.

          The context switch time is

            Time = (d - c) - (b - a)

             -----   --                   ---------------
                  | |  |                 |
                   -    -----------------
                  ^ ^  ^                 ^
                  a b  c                 d


          The time between c and d ist the context switch time, but
          note that the real context switch time is shorter, because the
          signal also contains the overhead of switching the LED on and
          off. The time of this overhead is also displayed on the oscilloscope
          as a small peak between a and b.
--------  END-OF-HEADER  ---------------------------------------------
*/



#include "RTOS.h"
#include "BSP.h"

static OS_STACKPTR int StackHP[128];  // Task stacks
static OS_TASK TCBHP;                 // Task-control-blocks

/*********************************************************************
*
*       HPTask
*/
static void HPTask(void) {
  while (1) {
    OS_Suspend(NULL);   // Suspend high priority task
    BSP_ClrLED(0);      // Stop measurement
  }
}

/*********************************************************************
*
*       MainTask
*/
void MainTask(void);
void MainTask(void) {
  OS_CREATETASK(&TCBHP, "HP Task", HPTask, 150, StackHP);
  OS_Delay(1);
  while (1) {
    OS_Delay(100);     // Syncronize to tick to avoid jitter
    //
    // Display measurement overhead
    //
    BSP_SetLED(0);
    BSP_ClrLED(0);
    //
    // Perform measurement
    //
    BSP_SetLED(0);     // Start measurement
    OS_Resume(&TCBHP); // Resume high priority task to force task switch
  }
}
