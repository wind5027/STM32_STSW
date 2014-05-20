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
File    : Main.c
Purpose : Generic SEGGER application start
--------  END-OF-HEADER  ---------------------------------------------
*/

#define MAIN_C

#include "RTOS.h"
#include "BSP.h"

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
void MainTask(void);

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static OS_STACKPTR int Stack0[1024]; /* Task stacks         */
static OS_TASK TCB0;                 /* Task-control-blocks */

/********************************************************************* 
* 
*       main()
* 
* Function description 
*   Application entry point
*/ 
int main(void) {
  OS_IncDI();                      /* Initially disable interrupts  */
  OS_InitKern();                   /* Initialize OS                 */
  OS_InitHW();                     /* Initialize Hardware for OS    */
  BSP_Init();                      /* Initialize BSP module         */
  BSP_SetLED(0);
  OS_CREATETASK(&TCB0, "MainTask", MainTask, 100, Stack0);
  OS_Start();
  return 0;
}

/****** End Of File *************************************************/
